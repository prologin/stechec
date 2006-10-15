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

#include <algorithm>
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
      child->parent_ = NULL;
      invalidate(child->getRect());
      child_list_.erase(it);
    }
  else
    WARN("Removing child: not found: %1", *child);
}

int VirtualSurface::getNbChild() const
{
  return child_list_.size();
}


void VirtualSurface::updateChildZOrder()
{
  std::sort(child_list_.begin(), child_list_.end(), Surface::ZSort());
}

void VirtualSurface::show()
{
  if (!isShown())
    invalidate(Point(0, 0), getSize());
  Surface::show();
}


void VirtualSurface::setZoom(double zoom)
{
  // Propagate setZoom on all children.
  Surface::setZoom(zoom);
  for_all(child_list_, std::bind2nd(std::mem_fun(&Surface::setZoom), zoom));

  // Adjust child position.
  SurfaceList::iterator it;
  for (it = child_list_.begin(); it != child_list_.end(); ++it)
    {
      Rect orig((*it)->orig_rect_); // Save to avoid setPos() round error.
      (*it)->setPos((int)(orig.x / zoom),
                    (int)(orig.y / zoom));
      (*it)->orig_rect_ = orig;
    }
}

void VirtualSurface::update()
{
  for_all(child_list_, std::mem_fun(&Surface::update));
  
  // Propagate invalidated surface to the parent.
  if (parent_ != NULL)
    {
      RectList::iterator rit;
      for (rit = invalidated_surf_.begin(); rit != invalidated_surf_.end(); rit++)
        parent_->invalidate(*rit + getPos());
    }

  Surface::update();
}

void VirtualSurface::render()
{
  SurfaceList::iterator it;
  RectList::iterator rit;

  if (!isShown())
    {
      LOG5("+ Not rendering `%1`, is hidden.", name_);
      invalidated_surf_.clear();
      return;
    }

  LOG5("+++ Rendering `%1`", name_);

  std::for_each(child_list_.begin(), child_list_.end(),
                std::mem_fun(&Surface::render));
  LOG5("End rendering childs of `%1`", name_);

  for (rit = invalidated_surf_.begin(); rit != invalidated_surf_.end(); rit++)
    {
      LOG6("+ Managing invalidated surface: %1", *rit);
      for (it = child_list_.begin(); it != child_list_.end(); ++it)
        if ((*it)->isShown())
          {
            Rect child_surf((*it)->getRenderRect());
            LOG6("  To child: %1", **it);
            child_surf &= *rit;
            if (child_surf.w > 0 && child_surf.h > 0)
              {
                LOG6("* Render merged, blit to: %1", child_surf);
                LOG6("* child blit from: %1", child_surf - (*it)->getPos());
                (*it)->blit(*this, child_surf, child_surf - (*it)->getPos());
              }
          }
    }

  // Should be the top level VirtualSurface, screen. Flip.
  if (parent_ == NULL
      && SDL_GetVideoSurface() == surf_
      && !invalidated_surf_.empty())
    {
      LOG5("Flip screen.");
      SDL_Flip(surf_);
    }

  invalidated_surf_.clear();
  LOG5("+++ End rendering `%1`", name_);
}

void VirtualSurface::invalidate(const Rect& zone)
{
  LOG6("Add invalidated rect: %1 for parent `%2`", zone, name_);
  // FIXME: merge invalidated_surf_'s.
  invalidated_surf_.push_back(zone);
}

void VirtualSurface::invalidate(const Point& pos, const Point& size)
{
  invalidate(Rect(pos.x, pos.y, size.x, size.y));
}
