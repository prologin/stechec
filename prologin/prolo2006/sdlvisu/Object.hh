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

#ifndef OBJECT_HH_
# define OBJECT_HH_

# include "Sprite.hh"

class SDLObject : public Sprite
{
public:
  SDLObject()
    : back_(NULL),
      have_back_(false),
      is_moving_(false),
      force_redraw_(false)
  {
  }

  ~SDLObject()
  {
    if (back_ != NULL)
      SDL_FreeSurface(back_);
  }
  
  void move(const Point& pos)
  {
    if (pos == pos_)
      return;

    // Teleport to destination, if we were moving.
    if (is_moving_)
      pos_ = move_to_;

    move_from_ = pos_;
    move_to_ = pos;
    last_updated_ = SDL_GetTicks();
    is_moving_ = true;
  }

  virtual void setPos(const Point& pos)
  {
    is_moving_ = false;
    Sprite::setPos(pos);
    //restoreBackground();
    saveBackground();
  }

  virtual void preUpdate()
  {
    if (is_moving_)
      {
        const double velocity = 0.000025;
        const unsigned now = SDL_GetTicks();
        const unsigned diff = now - last_updated_;

        Point tmp_pos((move_to_ - move_from_) * (diff * velocity) + pos_);
        LOG3("MOVING: from " << move_from_ << " to " << move_to_ << "  tmp " << tmp_pos);
        LOG3("diff " << diff * velocity << " pt: " << (move_to_ - move_from_)
             << "   res: " << (move_to_ - move_from_) * (diff * velocity));
        if (move_from_.distance(move_to_) < move_from_.distance(tmp_pos))
          {
            is_moving_ = false;
            pos_ = move_to_;
          }
        else
          pos_ = tmp_pos;

        restoreBackground();
        saveBackground();
 
        last_updated_ = now;
        force_redraw_ = true;
      }
  }
  
  virtual bool update(bool force = false)
  {
    force = force || force_redraw_;
    force_redraw_ = false;
    return Sprite::update(force);
  }

private:

  virtual void changeSurfaceAfter()
  {
    if (back_ == NULL || back_->w != surf_->w)
      {
        if (back_ != NULL)
          SDL_FreeSurface(back_);

        back_ =
          SDL_CreateRGBSurface(SDL_HWSURFACE,
                               surf_->w,
                               surf_->h,
                               screen_->format->BitsPerPixel,
                               screen_->format->Rmask,
                               screen_->format->Gmask,
                               screen_->format->Bmask,
                               screen_->format->Amask);
        assert(back_ != NULL);
        back_pos_.w = back_->w;
        back_pos_.h = back_->h;
      }
  }
  
  void saveBackground()
  {
    back_pos_.x = pos_.x;
    back_pos_.y = pos_.y;
    if (SDL_BlitSurface(screen_, &back_pos_, back_, NULL) != 0)
      ERR("Error while blitting the surface : " << SDL_GetError());
    have_back_ = true;
  }
  
  void restoreBackground()
  {
    if (!have_back_)
      return;
    
    if (SDL_BlitSurface(back_, NULL, screen_, &back_pos_) != 0)
      ERR("Error while blitting the surface : " << SDL_GetError());
    have_back_ = false;
  }

  SDL_Surface* back_;
  SDL_Rect back_pos_;
  bool  have_back_;
  
  Point move_from_;
  Point move_to_;
  unsigned last_updated_;
  bool is_moving_;
  bool force_redraw_;
};

#endif /* !OBJECT_HH_ */
