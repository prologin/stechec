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

#ifndef VIRTUALSCREEN_HH_
# define VIRTUALSCREEN_HH_

# include "tools.hh"
# include <SDL.h>

class VirtualScreen
{
public:
  VirtualScreen(SDL_Surface* screen, int virtual_width, int virtual_height);
  ~VirtualScreen() {}

  void mouseMotion(int x, int y);
  bool update(bool have_to_blit);

  int getZoom() const { return zoom_; }
  void setZoom(int zoom);

  SDL_Surface* getScreen() { return vscreen_; }
  
private:
  int width_;
  int height_;
  int zoom_;

  int dec_x_;
  int dec_y_;
  int cur_x_;
  int cur_y_;
  unsigned last_updated_; ///< Ticks (ms).
  SDL_Surface* screen_;
  SDL_Surface* vscreen_;
};

#endif /* !VIRTUALSCREEN_HH_ */
