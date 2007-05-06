/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
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
#define MAX_CELL 1000
#define MAX_VIRUS 1000
#define MAX_BACTERIA 1000

class GlobulusGui : public GlobulusEvent
{
public:
  GlobulusGui(xml::XMLConfig* xml, Api* api, ClientCx* ccx);
  ~GlobulusGui();

  int run();

private:

  /*!
  ** Events from rules.
  */
  virtual void endOfTurn() {}
  virtual void moveLeucocyte(int team_id, int unit_id, const Position& to);
  virtual void newLeucocyte(int team_id, int unit_id, const Position& at);
  virtual void moveVirus(int virus_id, const Position& to);
  virtual void newVirus(int virus_id, const Position& at, int type);
  virtual void dieVirus(int virus_id);
  virtual void newBacteria(int bacteria_id, const Position& at);
  virtual void dieBacteria(int bacteria_id);
  virtual void newCell(int cell_id, const Position& at);
  virtual void caseUpdate(int x, int y, int food, int antibody);
  virtual void cellUpdate(int cell_id, int type);

  Api*          api_;
  ClientCx*     ccx_;

  SDLWindow     win_;

  int           map_x_;
  int           map_y_;
  int           case_size_;

  void          init();
  void          initMapSquare(int x, int y);
  void          setZoom(int zoom);
  void          refreshInfoBox();

  VirtualScrollableSurface*     vscreen_;

  Square        map_[MAX_MAP_SIZE][MAX_MAP_SIZE];
  Sprite        food_[MAX_MAP_SIZE][MAX_MAP_SIZE];
  Sprite        units_[MAX_TEAM][MAX_WHITE_CELL];
  Sprite        cells_[MAX_CELL];
  Sprite        virus_[MAX_VIRUS];
  Sprite        bacterias_[MAX_BACTERIA];

  TextSurface   txt_beat_;
  TextSurface   txt_score_[MAX_TEAM];
};

#endif /* !GLOBULUSGUI_HH_ */
