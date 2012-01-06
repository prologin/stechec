/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#ifndef OUTAGUI_HH_
# define OUTAGUI_HH_

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

class OutAGui : public OutAnEvent
{
public:
  OutAGui(ConfFile* cfg_file, Api* api, ClientCx* ccx);
  ~OutAGui();

  int run();

private:

  /*!
  ** Events from rules.
  */
  virtual void endOfTurn() {}
  virtual void goodmanMove(int player_id, int unit_id, const Position& pos);
  virtual void goodmanChangeState(int team_id, int unit_id, int new_state);
  virtual void goodmanLostDelorean(int team_id, int unit_id);
  virtual void goodmanGetDelorean(int team_id, int unit_id);
  virtual void goodmanLostAlmanach(int team_id, int unit_id);
  virtual void goodmanGetAlmanach(int team_id, int unit_id);
  virtual void deloreanMove(const Position& pos);
  virtual void almanachMove(const Position& pos);

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
  Sprite        delorean_;
  Sprite        almanach_;
  Sprite        unit_[MAX_TEAM][MAX_GOODMEN];

  TextSurface   txt_date_;
  TextSurface   txt_fuel_;
  TextSurface   txt_score_[MAX_TEAM];
};

#endif /* !OUTAGUI_HH_ */
