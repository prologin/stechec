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

#ifndef SQUARE_HH_
# define SQUARE_HH_

# include <SDL_gfxPrimitives.h>

# include "Api.hh"
# include "Sprite.hh"

class Square : public Sprite
{
public:

  Square()
    : black_surf_(NULL),
      visible_surf_(NULL),
      is_black_(false),
      api_(NULL)
  {
  }

  ~Square()
  {
    if (black_surf_)
      SDL_FreeSurface(black_surf_);
  }

  void setGameInfo(Api* api, const Point& game_pos)
  {
    api_ = api;
    game_pos_ = game_pos;
  }
  
  virtual bool update(bool force = false)
  {
    assert(api_ != NULL && surf_ != NULL);

    // Switch from hidden to visible
    if (api_->visible(game_pos_.x, game_pos_.y) == 0 && !is_black_)
      {
        LOG4("background, switch to black: " << game_pos_);
        // Load the fog of war'ed surface, if not already done.
        if (black_surf_ == NULL)
          {
            black_surf_ =
              SDL_CreateRGBSurface(SDL_HWSURFACE,
                                     surf_->w,
                                     surf_->h,
                                     screen_->format->BitsPerPixel,
                                     screen_->format->Rmask,
                                     screen_->format->Gmask,
                                     screen_->format->Bmask,
                                     screen_->format->Amask);
              SDL_BlitSurface(surf_, NULL, black_surf_, NULL);
              boxRGBA(black_surf_, 0, 0, surf_->w, surf_->h, 0, 0, 0, 170);
          }
        visible_surf_ = surf_;
        surf_ = black_surf_;
        is_black_ = true;
        force = true;
      }

    // Switch from visible to hidden
    if (api_->visible(game_pos_.x, game_pos_.y) != 0 && is_black_)
      {
        LOG4("background, switch to visible: " << game_pos_);
        surf_ = visible_surf_;
        is_black_ = false;
        force = true;
      }
    
    return Sprite::update(force);
  }

private:
  virtual void changeSurface()
  {
    // surface changed... switch to visible, thus update() can
    // switch to black again.
    if (black_surf_ != NULL)
      SDL_FreeSurface(black_surf_);
    surf_ = visible_surf_;
    black_surf_ = NULL;
    is_black_ = false;
  }

  SDL_Surface*  black_surf_; ///< Fog of war.
  SDL_Surface*  visible_surf_;
  bool          is_black_;

  Point         game_pos_;
  Api*          api_;
};

#endif /* !SQUARE_HH_ */
