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
    orig_rect_(0, 0, -1, -1),
    zoom_(1.),
    angle_(0.),
    z_(0),
    inherit_alpha_(false),
    enabled_(true),
    show_(true),
    redraw_all_(false),
    parent_(NULL)
{
}

Surface::Surface(SDL_Surface* surf, double zoom, double angle, const std::string& filename)
  : surf_(surf),
    filename_(filename),
    rect_(0, 0, -1, -1),
    orig_rect_(0, 0, -1, -1),
    zoom_(zoom),
    angle_(angle),
    z_(0),
    inherit_alpha_(false),
    enabled_(true),
    show_(true),
    redraw_all_(true),
    parent_(NULL)
{
  if (surf_ != NULL)
    {
      rect_.w = surf->w;
      rect_.h = surf->h;
      orig_rect_ = rect_ * zoom;
      surf_->refcount++;
    }
}

Surface::Surface(const std::string filename, double zoom, double angle)
  : surf_(NULL),
    rect_(0, 0, -1, -1),
    orig_rect_(0, 0, -1, -1),
    z_(0),
    inherit_alpha_(false),
    enabled_(true),
    show_(true),
    redraw_all_(true),
    parent_(NULL)
{
  load(filename, zoom, angle);
}

Surface::Surface(int width, int height)
  : surf_(NULL),
    rect_(0, 0, width, height),
    orig_rect_(0, 0, width, height),
    zoom_(1.),
    angle_(0.),
    z_(0),
    inherit_alpha_(false),
    enabled_(true),
    show_(true),
    redraw_all_(true),
    parent_(NULL)
{
  create(width, height);
}

Surface::Surface(const Surface& s)
{
  surf_ = s.surf_;
  filename_ = s.filename_;
  rect_ = s.rect_;
  orig_rect_ = s.orig_rect_;
  zoom_ = s.zoom_;
  angle_ = s.angle_;
  z_ = s.z_;
  inherit_alpha_ = s.inherit_alpha_;
  enabled_ = s.enabled_;
  show_ = s.show_;
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
  orig_rect_ = s.orig_rect_;
  zoom_ = s.zoom_;
  angle_ = s.angle_;
  z_ = s.z_;
  inherit_alpha_ = s.inherit_alpha_;
  enabled_ = s.enabled_;
  show_ = s.show_;
  redraw_all_ = s.redraw_all_;
  parent_ = s.parent_;
  if (surf_ != NULL)
    surf_->refcount++;
  return *this;
}

Surface::~Surface()
{
  if (surf_ != NULL)
    SDL_FreeSurface(surf_);
  if (parent_ != NULL)
    parent_->removeChild(this);
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

Rect Surface::getScreenRect() const
{
  if (parent_ != NULL)
    {
      Rect parent_dec(parent_->getScreenRect());
      return Rect(parent_dec.x + rect_.x, parent_dec.y + rect_.y, rect_.w, rect_.h);
    }
  return rect_;
}

Rect Surface::getRenderRect() const
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
  if (pos.x == rect_.x && pos.y == rect_.y)
    return;

  if (parent_ != NULL)
    parent_->invalidate(rect_);
  rect_.x = pos.x;
  rect_.y = pos.y;
  orig_rect_.x = (int)(pos.x * zoom_);
  orig_rect_.y = (int)(pos.y * zoom_);  
  if (parent_ != NULL)
    parent_->invalidate(rect_);
}

void Surface::setPos(int x, int y)
{
  setPos(Point(x, y));
}

void Surface::setSize(const Point& size)
{
  rect_.w = size.x;
  rect_.h = size.y;
  orig_rect_.w = (int)(size.x * zoom_);
  orig_rect_.h = (int)(size.y * zoom_);
}

void Surface::setZoom(double zoom)
{
  // Avoid some round error.
  Rect orig(orig_rect_);

  if (filename_ != "")
    load(filename_, zoom, angle_);
  else
    {
      create((int)(orig_rect_.w / zoom),
             (int)(orig_rect_.h / zoom));
      zoom_ = zoom;
    }
  orig_rect_ = orig;
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

void Surface::setInheritAlpha(bool enabled)
{
  inherit_alpha_ = enabled;
}

void Surface::enable()
{
  if (!enabled_)
    redraw_all_ = true;
  enabled_ = true;
}

void Surface::disable()
{
  if (enabled_ && parent_ != NULL)
    parent_->invalidate(rect_);
  enabled_ = false;
}

bool Surface::isEnabled() const
{
  return enabled_;
}

void Surface::show()
{
  if (!show_)
    redraw_all_ = true;
  show_ = true;
}

void Surface::hide()
{
  if (show_)
    redraw_all_ = true;
  show_ = false;
}

bool Surface::isShown() const
{
  return show_;
}

void Surface::create(int width, int height, SDL_Surface* ref_surface)
{
  if (surf_ != NULL)
    SDL_FreeSurface(surf_);

  if (ref_surface == NULL)
    ref_surface = SDL_GetVideoSurface();
  // Happen if it is called before SDL_SetVideoMode().
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

  rect_.w = width;
  rect_.h = height;
  orig_rect_.w = (int)(width * zoom_);
  orig_rect_.h = (int)(height * zoom_);
  redraw_all_ = true;
}

void Surface::load(const std::string filename, double zoom, double angle)
{
  if (surf_ != NULL)
    SDL_FreeSurface(surf_);
    
  Surface ref(ResourceCenter::getInst()->getImage(filename, zoom, angle));
  surf_ = ref.surf_;
  filename_ = ref.filename_;
  rect_.w = surf_->w;
  rect_.h = surf_->h;
  zoom_ = ref.zoom_;
  angle_ = ref.angle_;
  surf_->refcount++;
  orig_rect_.w = (int)(rect_.w * zoom_);
  orig_rect_.h = (int)(rect_.h * zoom_);
  redraw_all_ = true;
}

void Surface::free()
{
  if (surf_ != NULL)
    SDL_FreeSurface(surf_);
  if (parent_ != NULL)
    parent_->removeChild(this);
  surf_ = NULL;
  parent_ = NULL;
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

void Surface::blitAlpha(SDL_Surface *src_surf, SDL_Surface *dst_surf,
			SDL_Rect* src_rect, unsigned dst_x, unsigned dst_y)
{
  SDL_PixelFormat* src_fmt = src_surf->format;
  SDL_PixelFormat* dst_fmt = dst_surf->format;

  unsigned src_x = 0;
  unsigned src_y = 0;
  unsigned width = 0;
  unsigned height = 0;

  if (src_rect)
    {
      src_x = src_rect->x;
      src_y = src_rect->y;
      width = src_rect->w;
      height = src_rect->h;
    }
  if (width == 0)
    width = src_surf->w;
  if (height == 0)
    height = src_surf->h;
  if (width > dst_surf->w - dst_x)
    width = dst_surf->w - dst_x;
  if (height > dst_surf->h - dst_y)
    height = dst_surf->h - dst_y;

  // Some checks...
  if (src_fmt->BitsPerPixel != 32 || dst_fmt->BitsPerPixel != 32)
    return;
  if (dst_fmt->Amask == 0)
    {
      WARN("No alpha mask on destination surface");
      return;
    }
  
  LOG6("Set transparency surf name %1 size x: %2 y: %3", filename_, width, height);
  LOG6("r:%1 g:%2 b:%3 a:%4", (int)src_fmt->Rshift, (int)src_fmt->Gshift, (int)src_fmt->Bshift, (int)src_fmt->Ashift);
  LOG6("r:%1 g:%2 b:%3 a:%4", (int)dst_fmt->Rshift, (int)dst_fmt->Gshift, (int)dst_fmt->Bshift, (int)dst_fmt->Ashift);

  SDL_LockSurface(surf_);
  SDL_LockSurface(dst_surf);
  Uint32* p_src = (Uint32*)surf_->pixels + surf_->w * src_y + src_x;
  Uint32* p_dst = (Uint32*)dst_surf->pixels + dst_surf->w * dst_y + dst_x;
  Uint32 src;
  Uint32 dst;

  for (unsigned y = 0; y < height; y++)
    {
      for (unsigned x = 0; x < width; x++)
	{
	  src = (*p_src & src_fmt->Amask) >> src_fmt->Ashift;
	  dst = (*p_dst & dst_fmt->Amask) >> dst_fmt->Ashift;
	  *p_dst = *p_dst | (src << dst_fmt->Ashift);
	  p_src++;
	  p_dst++;
	}
      p_src += surf_->w - width;
      p_dst += dst_surf->w - width;
    }
  SDL_UnlockSurface(dst_surf);
  SDL_UnlockSurface(surf_);

}

void Surface::blit(Surface& to)
{
  SDL_Rect tor = { rect_.x, rect_.y, 0, 0 };
  if (SDL_BlitSurface(surf_, NULL, to.getSDLSurface(), &tor) != 0)
    PRINT_AND_THROW(SDLError, "Blit failed");

  if (inherit_alpha_)
    blitAlpha(surf_, to.getSDLSurface(), NULL, tor.x, tor.y);
}

void Surface::blit(Surface& to, const Rect& to_rect, const Rect& from_rect)
{
  SDL_Rect tor = { to_rect.x, to_rect.y, 0, 0 };
  SDL_Rect fromr = { from_rect.x, from_rect.y, from_rect.w, from_rect.h };
  if (SDL_BlitSurface(surf_, &fromr, to.getSDLSurface(), &tor) != 0)
    PRINT_AND_THROW(SDLError, "Blit failed");

  if (inherit_alpha_)
    blitAlpha(surf_, to.getSDLSurface(), &fromr, tor.x, tor.y);
}

bool Surface::ZSort::operator() (const Surface* lhs, const Surface* rhs)
{
  return lhs->z_ < rhs->z_;
}

bool Surface::ImgSort::operator() (const Surface& lhs, const Surface& rhs)
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
  os << "  ptr: " << s.surf_ << " redraw_all: " << s.redraw_all_
     << " inherit_alpha: " << s.inherit_alpha_;
  return os;
}
