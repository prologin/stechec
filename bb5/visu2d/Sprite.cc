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
#include "Sprite.hh"

Sprite::Sprite()
  : nb_anim_width_(1),
    nb_anim_height_(1),
    current_anim_(0),
    is_animing_(false),
    is_moving_(false)
{
}

Sprite::Sprite(SDL_Surface* surf, double zoom, double angle)
  : Surface(surf, zoom, angle),
    nb_anim_width_(1),
    nb_anim_height_(1),
    current_anim_(0),
    is_animing_(false),
    is_moving_(false)
{
}

Sprite::Sprite(const std::string filename, double zoom, double angle)
  : Surface(filename, zoom, angle),
    nb_anim_width_(1),
    nb_anim_height_(1),
    current_anim_(0),
    is_animing_(false),
    is_moving_(false)
{
}


Sprite::~Sprite()
{
}

void Sprite::splitNbFrame(int nb_frame_width, int nb_frame_height)
{
  if (surf_ == NULL)
    PRINT_AND_THROW(Exception, "Surface not loaded yet. Can't set animation");
  if (surf_->w % nb_frame_width != 0)
    PRINT_AND_THROW(Exception, "Bad image width (" << surf_->w << "), can't "
                    "split into `"<< nb_frame_width << "' animations.");
  if (surf_->h % nb_frame_height != 0)
    PRINT_AND_THROW(Exception, "Bad image width (" << surf_->h << "), can't "
                    "split into `"<< nb_frame_height << "' animations.");

  nb_anim_width_ = nb_frame_width;
  nb_anim_height_ = nb_frame_height;
  setSize(Point(surf_->w / nb_anim_width_, surf_->h / nb_anim_height_));
}

void Sprite::splitSizeFrame(int size_frame_width, int size_frame_height)
{
  nb_anim_width_ = surf_->w / size_frame_width;
  nb_anim_height_ = surf_->h / size_frame_height;
  setSize(Point(surf_->w / nb_anim_width_, surf_->h / nb_anim_height_));
}

void Sprite::move(const Point& to, double velocity)
{
  if (getPos() == to)
    return;

  // Teleport to destination, if we were moving.
  if (is_moving_)
    {
      parent_->invalidate(getRect());
      parent_->invalidate(move_to_, getSize());
      setPos(move_to_);
    }

  move_from_ = getPos();
  move_to_ = to;
  current_pos_ = DPoint(move_from_.x, move_from_.y);
  last_updated_ = SDL_GetTicks();
  is_moving_ = true;
  velocity_ = velocity * 0.00001;
}

void Sprite::move(int to_x, int to_y, double velocity)
{
  move(Point(to_x, to_y), velocity);
}

void Sprite::anim(int delay, bool loop_forever)
{
  if (nb_anim_width_ == 1 && nb_anim_height_ == 1)
    {
      WARN("No animation set, cannot animate sprite !");
      return;
    }
  anim_delay_ = delay;
  current_anim_ = 0;
  last_anim_updated_ = SDL_GetTicks();
  anim_loop_ = loop_forever;
  is_animing_ = true;
}

void Sprite::stopAnim()
{
  is_animing_ = false;
}

void Sprite::setFrame(int frame)
{
  if (frame < 1 || frame > nb_anim_width_ * nb_anim_height_ )
    PRINT_AND_THROW(Exception, "Frame out of bound: " << frame
                    << " ([1.." << nb_anim_width_ * nb_anim_height_ << "])");
  current_anim_ = frame - 1;
}

void Sprite::setZoom(double zoom)
{
  Surface::setZoom(zoom);
  splitNbFrame(nb_anim_width_, nb_anim_width_);
}

void Sprite::update()
{
  if (is_moving_)
    {
      const unsigned now = SDL_GetTicks();
      const unsigned diff = now - last_updated_;
      last_updated_ = SDL_GetTicks();
      
      DPoint tmp_pos(DPoint(move_to_ - move_from_) * (diff * velocity_) + current_pos_);
      current_pos_ = tmp_pos;

      LOG6("MOVING: from " << move_from_ << " to " << move_to_ << "  tmp " << tmp_pos);
      LOG6("diff " << diff << " diff * velo: " << diff * velocity_
           << "   res: " << DPoint(move_to_ - move_from_) * (diff * velocity_));

      if (move_from_.distance(move_to_) < move_from_.distance(current_pos_))
        {
          parent_->invalidate(getRect() | Rect(move_to_.x, move_to_.y, getSize().x, getSize().y));
          is_moving_ = false;
          setPos(move_to_);
        }
      else
        {
          parent_->invalidate(getRect() | Rect(current_pos_.x, current_pos_.y,
                                               getSize().x, getSize().y));
          setPos(current_pos_);
        }
    }

  if (is_animing_ && SDL_GetTicks() >= last_anim_updated_ + anim_delay_)
    {
      last_anim_updated_ = SDL_GetTicks();
      current_anim_ = (current_anim_ + 1) % (nb_anim_width_ * nb_anim_height_);
      if (current_anim_ == 0 && !anim_loop_)
        is_animing_ = false;
      redraw_all_ = true;
    }

  Surface::update();
}

void Sprite::blit(Surface& to)
{
  Rect anim_rect;
  anim_rect.x = (current_anim_ % nb_anim_width_) * (surf_->w / nb_anim_width_);
  anim_rect.y = (current_anim_ / nb_anim_width_) * (surf_->h / nb_anim_height_);
  anim_rect.w = getRect().w;
  anim_rect.h = getRect().h;
  Surface::blit(to, getRect(), anim_rect);
}

void Sprite::blit(Surface& to, const Rect& to_rect, const Rect& from_rect)
{
  Rect anim_rect;
  anim_rect.x = (current_anim_ % nb_anim_width_) * (surf_->w / nb_anim_width_);
  anim_rect.y = (current_anim_ / nb_anim_width_) * (surf_->h / nb_anim_height_);
  anim_rect.w = 0;
  anim_rect.h = 0;
  Surface::blit(to, to_rect, from_rect + anim_rect);
}
