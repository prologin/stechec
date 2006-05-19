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

#include "ResourceCenter.hh"
#include "VirtualSurface.hh"
#include "Surface.hh"

Surface::Surface()
  : surf_(NULL),
    filename_(""),
    rect_(0, 0, -1, -1),
    zoom_(1.),
    angle_(0.),
    z_(0),
    redraw_all_(true),
    parent_(NULL)
{
}

Surface::Surface(SDL_Surface* surf, double zoom, double angle, const std::string& filename)
  : surf_(surf),
    filename_(filename),
    rect_(0, 0, -1, -1),
    zoom_(zoom),
    angle_(angle),
    z_(0),
    redraw_all_(true),
    parent_(NULL)
{
  if (surf_ != NULL)
    {
      rect_.w = surf->w;
      rect_.h = surf->h;
      surf_->refcount++;
    }
}

Surface::Surface(const std::string filename, double zoom, double angle)
  : rect_(0, 0, -1, -1),
    z_(0),
    redraw_all_(true),
    parent_(NULL)
{
  Surface ref(ResourceCenter::getInst()->getImage(filename, zoom, angle));
  surf_ = ref.surf_;
  filename_ = ref.filename_;
  rect_.w = surf_->w;
  rect_.h = surf_->h;
  zoom_ = ref.zoom_;
  angle_ = ref.angle_;
  surf_->refcount++;
}

Surface::Surface(int width, int height)
  : rect_(0, 0, width, height),
    zoom_(1.),
    angle_(0.),
    z_(0),
    redraw_all_(true),
    parent_(NULL)
{
  SDL_Surface* ref_surface = SDL_GetVideoSurface();
  assert(ref_surface != NULL);

  surf_ = SDL_CreateRGBSurface(SDL_HWSURFACE,
                               width,
                               height,
                               ref_surface->format->BitsPerPixel,
                               ref_surface->format->Rmask,
                               ref_surface->format->Gmask,
                               ref_surface->format->Bmask,
                               ref_surface->format->Amask);
  if (surf_ == NULL)
    PRINT_AND_THROW(SDLError, "Can't create surface");
}

Surface::Surface(const Surface& s)
{
  surf_ = s.surf_;
  filename_ = s.filename_;
  rect_ = s.rect_;
  zoom_ = s.zoom_;
  angle_ = s.angle_;
  z_ = s.z_;
  redraw_all_ = s.redraw_all_;
  parent_ = s.parent_;
  if (surf_ != NULL)
    surf_->refcount++;
}

Surface& Surface::operator=(const Surface& s)
{
  SDL_FreeSurface(surf_);
  surf_ = s.surf_;
  filename_ = s.filename_;
  rect_ = s.rect_;
  zoom_ = s.zoom_;
  angle_ = s.angle_;
  z_ = s.z_;
  redraw_all_ = s.redraw_all_;
  parent_ = s.parent_;
  if (surf_ != NULL)
    surf_->refcount++;
  return *this;
}

Surface::~Surface()
{
  if (surf_)
    SDL_FreeSurface(surf_);
}

Point Surface::getPos() const
{
  return Point(rect_.x, rect_.y);
}

Point Surface::getSize() const
{
  return Point(rect_.w, rect_.h);
}

const Rect& Surface::getRect() const
{
  return rect_;
}

double Surface::getZoom() const
{
  return zoom_;
}

double Surface::getAngle() const
{
  return angle_;
}

int Surface::getZ() const
{
  return z_;
}

SDL_Surface* Surface::getSDLSurface()
{
  return surf_;
}

void Surface::setPos(const Point& pos)
{
  rect_.x = pos.x;
  rect_.y = pos.y;
}

void Surface::setPos(int x, int y)
{
  rect_.x = x;
  rect_.y = y;
}

void Surface::setSize(const Point& size)
{
  rect_.w = size.x;
  rect_.h = size.y;
}

void Surface::setZoom(double zoom)
{
  Surface ref(ResourceCenter::getInst()->getImage(filename_, zoom, angle_));
  SDL_FreeSurface(surf_);
  surf_ = ref.surf_;
  rect_.w = surf_->w;
  rect_.h = surf_->h;
  zoom_ = zoom;
}

void Surface::setAngle(double angle)
{
  // FIXME: reload image.
  angle_ = angle;
}

void Surface::setZ(int z)
{
  z_ = z;
  if (parent_ != NULL)
    parent_->updateChildZOrder();
}

Rect Surface::getRenderRect() const
{
  return rect_;
}

void Surface::update()
{
  if (redraw_all_)
    {
      if (parent_ != NULL)
        parent_->invalidate(rect_);
      redraw_all_ = false;
    }
}

void Surface::blit(Surface& to)
{
  SDL_Rect tor = { rect_.x, rect_.y, 0, 0 };
  if (SDL_BlitSurface(surf_, NULL, to.getSDLSurface(), &tor) != 0)
    PRINT_AND_THROW(SDLError, "Blit failed");
}

void Surface::blit(Surface& to, const Rect& to_rect, const Rect& from_rect)
{
  SDL_Rect tor = { to_rect.x, to_rect.y, 0, 0 };
  SDL_Rect fromr = { from_rect.x, from_rect.y, from_rect.w, from_rect.h };
  if (SDL_BlitSurface(surf_, &fromr, to.getSDLSurface(), &tor) != 0)
    PRINT_AND_THROW(SDLError, "Blit failed");
}

bool operator< (const Surface& lhs, const Surface& rhs)
{
  if (lhs.filename_ < rhs.filename_)
    return true;
  if (lhs.filename_ > rhs.filename_)
    return false;
  if (lhs.zoom_ < rhs.zoom_)
    return true;
  if (lhs.zoom_ > rhs.zoom_)
    return false;
  return lhs.angle_ < rhs.angle_;
}


std::ostream& operator<< (std::ostream& os, const Surface& s)
{
  os << "Dump surface `" << s.filename_ << "'\n";
  os << "  rect: " << s.rect_ << "\n";
  os << "  zoom: " << s.zoom_ << " angle: " << s.angle_ << " z: " << s.z_ << "\n";
  os << "  ptr: " << s.surf_ << " redraw_all: " << s.redraw_all_;
  return os;
}
