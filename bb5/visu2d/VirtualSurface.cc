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

#include "VirtualSurface.hh"

VirtualSurface::VirtualSurface()
  : name_("unitialized")
{
}

VirtualSurface::VirtualSurface(const std::string& name, SDL_Surface* surf)
  : Surface(surf),
    name_(name)
{
}

VirtualSurface::VirtualSurface(const std::string& name, int width, int height)
  : Surface(width, height),
    name_(name)
{
}

VirtualSurface::~VirtualSurface()
{
}

void VirtualSurface::addChild(Surface* child)
{
  child->parent_ = this;
  child_list_.push_back(child);
  updateChildZOrder();
}

void VirtualSurface::removeChild(Surface* child)
{
  SurfaceList::iterator it;
  it = std::find(child_list_.begin(), child_list_.end(), child);
  if (it != child_list_.end())
    {
      (*it)->parent_ = NULL;
      child_list_.erase(it);
    }
}

void VirtualSurface::updateChildZOrder()
{
  std::sort(child_list_.begin(), child_list_.end(), Surface::ZSort());
}

void VirtualSurface::update()
{
  std::for_each(child_list_.begin(), child_list_.end(), std::mem_fun(&Surface::update));

  // Propagate invalidated surface to the parent.
  if (parent_ != NULL)
    {
      RectList::iterator rit;
      for (rit = invalidated_surf_.begin(); rit != invalidated_surf_.end(); rit++)
        parent_->invalidate(*rit + getPos());
    }
}

void VirtualSurface::render()
{
  SurfaceList::iterator it;
  RectList::iterator rit;

  LOG5("+++ Rendering `" << name_ << "'");

  std::for_each(child_list_.begin(), child_list_.end(),
                std::mem_fun(&Surface::render));
  LOG5("End rendering childs of `" << name_ << "'");

  for (rit = invalidated_surf_.begin(); rit != invalidated_surf_.end(); rit++)
    {
      LOG6("+ Managing invalidated surface: " << *rit);
      for (it = child_list_.begin(); it != child_list_.end(); ++it)
        {
          Rect child_surf((*it)->getRenderRect());
          LOG6("  To child: " << **it);
          child_surf &= *rit;
          if (child_surf.w > 0 && child_surf.h > 0)
            {
              LOG6("* Render merged, blit to: " << child_surf);
              LOG6("* child blit from: " << child_surf - (*it)->getPos());
              (*it)->blit(*this, child_surf, child_surf - (*it)->getPos());
            }
        }
    }

  // Should be the top level VirtualSurface, screen. Flip.
  if (parent_ == NULL
      && SDL_GetVideoSurface() == surf_
      && !invalidated_surf_.empty())
    {
      LOG4("Flip screen.");
      SDL_Flip(surf_);
    }

  invalidated_surf_.clear();
  LOG5("+++ End rendering `" << name_ << "'");
}

void VirtualSurface::invalidate(const Rect& zone)
{
  // FIXME: merge invalidated_surf_'s.
  invalidated_surf_.push_back(zone);
}

void VirtualSurface::invalidate(const Point& pos, const Point& size)
{
  invalidate(Rect(pos.x, pos.y, size.x, size.y));
}
