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

#include "VirtualScreen.hh"
#include "Square.hh"

VirtualScreen::VirtualScreen(SDL_Surface* screen, int virtual_width, int virtual_height)
  : width_(virtual_width),
    height_(virtual_height),
    zoom_(-1),
    dec_x_(0),
    dec_y_(0),
    cur_x_(0),
    cur_y_(0),
    screen_(screen),
    vscreen_(NULL)
{
  last_updated_ = SDL_GetTicks();
  setZoom(2); // init surface.
}

void VirtualScreen::setZoom(int zoom)
{
  if (zoom_ == zoom)
    return;

  zoom_ = zoom;
  if (vscreen_ != NULL)
    SDL_FreeSurface(vscreen_);

  vscreen_ =
    SDL_CreateRGBSurface(SDL_HWSURFACE,
                         width_ / zoom_,
                         height_ / zoom_,
                         screen_->format->BitsPerPixel,
                         screen_->format->Rmask,
                         screen_->format->Gmask,
                         screen_->format->Bmask,
                         screen_->format->Amask);
  assert(vscreen_ != NULL);
  cur_x_ = 0;
  cur_y_ = 0;
}

void VirtualScreen::mouseMotion(int x, int y)
{
  // Screen borders.
  const Rect left(0, 0, 20, 640);
  const Rect right(780, 0, 31, 640);
  const Rect up(0, 0, 800, 20);
  const Rect bottom(0, 620, 800, 31);

  Point mouse(x, y);

  dec_x_ = 0;
  dec_y_ = 0;
  if (left.inside(mouse))
    dec_x_ = -1;
  if (right.inside(mouse))
    dec_x_ = 1;
  if (up.inside(mouse))
    dec_y_ = -1;
  if (bottom.inside(mouse))
    dec_y_ = 1;
}

bool VirtualScreen::update(bool have_to_blit)
{
  const unsigned now = SDL_GetTicks();
  const unsigned diff = now - last_updated_;
  const double velocity = 0.25;
  const int last_x = cur_x_;
  const int last_y = cur_y_;

  if (dec_x_)
    cur_x_ += (int)(velocity * diff * dec_x_);
  if (dec_y_)
    cur_y_ += (int)(velocity * diff * dec_y_);
  last_updated_ = now;

  if (cur_x_ > vscreen_->w - screen_->w)
    cur_x_ = vscreen_->w - screen_->w;
  if (cur_y_ > vscreen_->h - screen_->h)
    cur_y_ = vscreen_->h - screen_->h;
  if (cur_x_ < 0)
    cur_x_ = 0;
  if (cur_y_ < 0)
    cur_y_ = 0;

  // Nothing to do... exit now.
  if (!have_to_blit && cur_x_ == last_x && cur_y_ == last_y)
    return false;

  // Update screen
  int dstx = 0;
  int dsty = 0;
  int vw = vscreen_->w;
  int vh = vscreen_->h;

  if (vw < screen_->w)
    dstx = (screen_->w - vw) / 2;
  else
    vw = screen_->w;
  if (vh < screen_->h)
    dsty = (screen_->h - vh) / 2;
  else
    vh = screen_->h;

  // Clear the main screen.
  SDL_FillRect(screen_, NULL, SDL_MapRGB(screen_->format, 0, 0, 0));

  LOG4("vs: blit from x/y " << cur_x_ << " " << cur_y_ << " w/h " << vw << " " << vh
       << " to : " << dstx << " " << dsty);
  SDL_Rect src = { cur_x_, cur_y_, vw, vh };
  SDL_Rect dst = { dstx, dsty, 0, 0 };
  SDL_BlitSurface(vscreen_, &src, screen_, &dst);
  return true;
}
