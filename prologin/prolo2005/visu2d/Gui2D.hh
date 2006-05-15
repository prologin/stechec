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

#ifndef GUI2D_HH_
# define GUI2D_HH_

# include "client_cx.hh"
# include "Api.hh"
# include "Event.hh"
# include "Gui.hh"

class Gui2D : public Gui, public ZavieEvent
{
public:
  Gui2D(ClientCx* ccx, Api* api);
  virtual ~Gui2D();

private:

  void applyDiff(StechecPkt* pkt);
  void loadMap();

  virtual void slotPeriodic();

  /*!
  ** Events
  */
  virtual void newUnit(int player_id, int unit_id, const Position& pos);
  virtual void moveColeoptera(int player_id, int unit_id, const Position& pos);
  virtual void buildingFinished(int player_id, const Position& pos);
  
  Api*          api_;

  Object2D*     map_bg_[50][50];
  Object2D*     map_fg_[50][50];
  Object2D*     map_building_[50][50];
  Object2D*     unit_[MAX_PLAYER][MAX_COLEOPTERE];
};


#endif /* !GUI2D_HH_ */
