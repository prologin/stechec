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
# include "Surface.hh"

class Square : public Surface
{
public:

  Square()
    : api_(NULL)
  {
  }

  ~Square()
  {
  }

  void setGameInfo(Api* api, const Point& game_pos)
  {
    api_ = api;
    game_pos_ = game_pos;

    // FIXME: kludge, highly order dependant.
    black_surf_.create(128, 128);
    blit(black_surf_);
    boxRGBA(black_surf_.getSDLSurface(), 0, 0, 128, 128, 0, 0, 0, 170);
    black_surf_.hide();
    parent_->addChild(&black_surf_);
  }

  virtual void setPos(const Point& pos)
  {
    black_surf_.setPos(pos);
    Surface::setPos(pos);
  }

  virtual void setZoom(double zoom)
  {
    Surface::setZoom(zoom);

    blit(black_surf_, Rect(), Rect(0, 0, getRect().w, getRect().h));
    boxRGBA(black_surf_.getSDLSurface(), 0, 0,
            getRect().w, getRect().h, 0, 0, 0, 170);
  }

  virtual void setZ(int z)
  {
    black_surf_.setZ(z - 1);
    Surface::setZ(z);
  }

  virtual void update()
  {
    assert(api_ != NULL);
#if 0
    // Switch from hidden to visible
    if (api_->visible(game_pos_.x, game_pos_.y) == 0 && !black_surf_.isShown())
      {
        black_surf_.show();
        hide();
      }

    // Switch from visible to hidden
    if (api_->visible(game_pos_.x, game_pos_.y) != 0 && black_surf_.isShown())
      {
        black_surf_.hide();
        show();
      }
    #endif
    Surface::update();
  }

private:

  Surface       black_surf_;    ///< Fog of war.
  Point         game_pos_;
  Api*          api_;
};

#endif /* !SQUARE_HH_ */
