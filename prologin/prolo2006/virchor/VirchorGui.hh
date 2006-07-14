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

#ifndef VIRCHORGUI_HH_
# define VIRCHORGUI_HH_

# include "tools.hh"
# include "client_cx.hh"
# include "Api.hh"
# include "Event.hh"

# include "VirObject.hh"
# include "VirUdp.hh"

class VirchorGui : public OutAnEvent
{
public:
  VirchorGui(Api* api, ClientCx* ccx);
  ~VirchorGui();
  
  int run();

private:
  
  /*!
  ** Events from rules.
  */
  virtual void moveGoodman(int team_id, int unit_id, const Position& pos);
  virtual void endOfTurn();
  int jump();
  int translate();

  int init();
  
  Api*          api_;
  ClientCx*     ccx_;
  VirUDP        v;

  int           date_;
  Point         delorean_;
  Point         almanach_;
  Point         unit_[MAX_TEAM][MAX_GOODMEN];
};

#endif /* !VIRCHORGUI_HH_ */
