/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2007 Prologin
*/

#ifndef GLOBULUSGUI_HH_
# define GLOBULUSGUI_HH_

# include <SDL.h>
# include <SDL_image.h>

# include "tools.hh"
# include "ClientCx.hh"
# include "Api.hh"
# include "Event.hh"

# include "SDLWindow.hh"
# include "Square.hh"
# include "Sprite.hh"
# include "VirtualScrollableSurface.hh"
# include "TextSurface.hh"

// FIXME: remove this
#define MAX_CELL (MAX_MAP_SIZE * MAX_MAP_SIZE)
#define MAX_VIRUS (MAX_MAP_SIZE * MAX_MAP_SIZE)

class GlobulusGui : public GlobulusEvent
{
public:
  GlobulusGui(ConfFile* cfg_file, Api* api, ClientCx* ccx);
  ~GlobulusGui();

  int run();

private:

  /*!
  ** Events from rules.
  */
  virtual void endOfTurn() {}

  Api*          api_;
  ClientCx*     ccx_;

  SDLWindow     win_;

  int           map_x_;
  int           map_y_;
  int           case_size_;
  int		current_team_;

  void          init();
  void          initMapSquare(int x, int y);
  void          setZoom(int zoom);
  void          refreshInfoBox();

  VirtualScrollableSurface*     vscreen_;

  Square        map_[MAX_MAP_SIZE][MAX_MAP_SIZE];
  Sprite        food_[MAX_MAP_SIZE][MAX_MAP_SIZE];
  Sprite        antibody_[MAX_MAP_SIZE][MAX_MAP_SIZE];
  Sprite        units_[MAX_TEAM][MAX_PLAYER];
  Sprite        cells_[MAX_CELL];
  Sprite        virus_[MAX_VIRUS];
  Sprite        bacterias_[MAX_MAP_SIZE][MAX_MAP_SIZE];

  Surface       back_txt_;
  TextSurface   txt_beat_;
  TextSurface   txt_score_[MAX_TEAM];
};

#endif /* !GLOBULUSGUI_HH_ */
