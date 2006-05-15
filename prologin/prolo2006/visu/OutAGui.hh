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

#ifndef OUTAGUI_HH_
# define OUTAGUI_HH_

# include "client_cx.hh"
# include "Api.hh"
# include "Event.hh"
# include "Gui.hh"

class OutAGui : public Gui, public OutAnEvent
{
public:
  OutAGui(ClientCx* ccx, Api* api);
  virtual ~OutAGui();

private:
  const char* getBackground(int land, int x, int y);
  void loadMap();

  /*!
  ** Events from base GUI.
  */
  virtual void slotPeriodic();

  /*!
  ** Events from rules.
  */
  virtual void endOfTurn() {}
  virtual void moveGoodman(int player_id, int unit_id, const Position& pos);
  virtual void changeDate(int date);

  Api*          api_;

  Object2D*     map_[MAX_MAP_SIZE][MAX_MAP_SIZE];
  Object2D*     unit_[MAX_TEAM][MAX_GOODMEN];
};


#endif /* !OUTAGUI_HH_ */
