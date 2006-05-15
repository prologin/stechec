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

#ifndef INTERFACE_HH_
# define INTERFACE_HH_

# include "client_cx.hh"
# include "Api.hh"
# include "Event.hh"
# include "input.hh"

/*!
** @brief A stupid command line based client.
*/
class CmdLineInterface : public Event
{
public:
  CmdLineInterface(xml::XMLConfig* cfg, Api* api, ClientCx* client_cx);

  void                hello();
  void                init();
  void                run();

  void                printGlobal();
  void                printPlayerList();
  void                printPlayer(int player_id, int team_id);
  void                printField();

private:
  bool                checkInput();

  // All callbacks.
  virtual void          evIllegal();
  virtual void          evNewTurn();
  virtual void          evKickOff();
  virtual void          evMoveTurnMarker();
  virtual void          evTimeExceeded();
  virtual void          evChat(const std::string& msg);
  virtual void          evPlayerMove(int team_id, int player_id, const Point& pos);
  virtual void          evPlayerKnocked(int team_id, int player_id);

  xml::XMLConfig*       cfg_;
  Api*                  api_;
  ClientCx*             ccx_;
  
  bool                  standalone_;
  Input                 input_;
  bool                  our_turn_;
  bool                  paused_;
};

#endif /* !INTERFACE_HH_ */
