/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#ifndef SPRITE_HH_
# define SPRITE_HH_

# include <SDL.h>
# include <SDL_rotozoom.h>

class Sprite
{
public:

  Sprite()
    : orig_surf_(NULL),
      surf_(NULL),
      zoom_(1.),
      must_redraw_(false)
  {
    screen_ = NULL;
  }

  virtual ~Sprite()
  {
    //SDL_FreeSurface(surf_);
  }

  void setScreen(SDL_Surface* screen)
  {
    screen_ = screen;
  }

  virtual void setPos(const Point& pos)
  {
    pos_ = pos;
    must_redraw_ = true;
  }
  
  void setSurf(SDL_Surface* surf)
  {
    orig_surf_ = surf_ = surf;
    surf_->refcount++;
    if (zoom_ != 1.)
      setZoom(zoom_);
    must_redraw_ = true;
  }

  void setZoom(double zoom)
  {
    if (orig_surf_ != NULL)
      {
        changeSurface(); // Hook for derived class, in case they changed surf_.
        SDL_FreeSurface(surf_);
        surf_ = rotozoomSurface(orig_surf_, 0., 1. / zoom, SMOOTHING_OFF);
        assert(surf_ != NULL);
        changeSurfaceAfter();
      }
    zoom_ = zoom;
    must_redraw_ = true;
  }

  // Should not changes the background. Only restore it.
  virtual void preUpdate()
  {
  }

  // Return true if we have changed some things, ie. SDL_Flip must be called.
  virtual bool update(bool force = false)
  {
    bool redraw = must_redraw_ || force;

    must_redraw_ = false;
    if (redraw)
      blit(pos_);
    return redraw;
  }

  void blit(const Point& dst)
  {
    assert(surf_ != NULL && screen_ != NULL);
    SDL_Rect rect = { dst.x, dst.y, 0, 0};    
    if (SDL_BlitSurface(surf_, NULL, screen_, &rect) != 0)
      ERR("Error while blitting the surface : " << SDL_GetError());
  }

protected:
  virtual void changeSurface() {}
  virtual void changeSurfaceAfter() {}

  SDL_Surface*  screen_;
  SDL_Surface*  orig_surf_;
  SDL_Surface*  surf_;
  Point         pos_;
  double        zoom_;
  bool          must_redraw_;
};

#endif /* !SPRITE_HH_ */
