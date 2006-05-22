/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
**
** Copyright (C) 2006 The TBT Team.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** The TBT Team consists of people listed in the `AUTHORS' file.
*/

#include "VirtualScrollableSurface.hh"

VirtualScrollableSurface::VirtualScrollableSurface(const std::string& name,
                                                   Input& input,
                                                   const Point& real_size,
                                                   const Point& virtual_size)
  : VirtualSurface(name, virtual_size.x, virtual_size.y),
    inp_(input),
    real_size_(real_size),
    vpos_(0, 0),
    dec_(0, 0)
{
  last_updated_ = SDL_GetTicks();
}

VirtualScrollableSurface::~VirtualScrollableSurface()
{
}


Rect VirtualScrollableSurface::getRealRect() const
{
  return Rect(getPos(), real_size_);
}

Point VirtualScrollableSurface::getAbsolutePos() const
{
  if (parent_ != NULL)
    return parent_->getAbsolutePos() + getPos() - vpos_;
  else
    return getPos() - vpos_;
}

void VirtualScrollableSurface::update()
{
  // FIXME: must be directly attached to screen to work.
  const Rect rect(getPos(), getSize());

  // Screen borders.
  const Rect left(rect.x, rect.y, 20, real_size_.y);
  const Rect right(rect.x + real_size_.x - 20, rect.y, 20, real_size_.y);
  const Rect up(rect.x, rect.y, real_size_.x, 20);
  const Rect bottom(rect.x, rect.y + real_size_.y - 20, real_size_.x, 20);

  // Update screen motion by mouse
  dec_ = Point(0, 0);
  if (left.inside(inp_.mouse_))
    dec_.x = -1;
  if (right.inside(inp_.mouse_))
    dec_.x = 1;
  if (up.inside(inp_.mouse_))
    dec_.y = -1;
  if (bottom.inside(inp_.mouse_))
    dec_.y = 1;

  // Update vscreen position.
  const unsigned now = SDL_GetTicks();
  const unsigned diff = now - last_updated_;
  const double velocity = 0.25;
  const Point last_pos(vpos_);

  if (dec_.x)
    vpos_.x += (int)(velocity * diff * dec_.x);
  if (dec_.y)
    vpos_.y += (int)(velocity * diff * dec_.y);
  last_updated_ = now;

  if (vpos_.x > rect.w - real_size_.x)
    vpos_.x = rect.w - real_size_.x;
  if (vpos_.y > rect.h - real_size_.y)
    vpos_.y = rect.h - real_size_.y;
  if (vpos_.x < 0)
    vpos_.x = 0;
  if (vpos_.y < 0)
    vpos_.y = 0;

  // Ask redraw if something moved.
  if (vpos_ != last_pos)
    {
      Rect inv(getPos(), real_size_);
      parent_->invalidate(inv);

      // Update children absolute position.
      SurfaceList::iterator it;
      for (it = child_list_.begin(); it != child_list_.end(); ++it)
        (*it)->setPos((*it)->getPos());
    }


  // Render children
  for_all(child_list_, std::mem_fun(&Surface::update));

  // Propagate invalidated surface to the parent.
  if (parent_ != NULL)
    {
      RectList::iterator rit;
      for (rit = invalidated_surf_.begin(); rit != invalidated_surf_.end(); rit++)
        parent_->invalidate(*rit + getPos() - vpos_);
    }
}

void VirtualScrollableSurface::blit(Surface& to)
{
  Surface::blit(to, getRect(),
                Rect(vpos_.x, vpos_.y, real_size_.x, real_size_.y));
}

void VirtualScrollableSurface::blit(Surface& to,
                                    const Rect& to_rect,
                                    const Rect& from_rect)
{
  LOG6("VSS: blit from: " << from_rect + vpos_);
  Surface::blit(to, to_rect, from_rect + vpos_);
}
