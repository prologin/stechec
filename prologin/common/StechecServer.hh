/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#ifndef STECHECSERVER_HH_
# define STECHECSERVER_HH_

# include "StechecGameData.hh"

/*!
** Stechec server.
*/
class StechecServer
{
public:
  StechecServer(GameData* game);
  StechecServer(GameData* game, Server* server);
  virtual ~StechecServer();

protected:

  GameData*     g_;
  Server*       s_;

  /**
  ** Send a data to all clients connected to server.
  */
  void          SendToAll(const StechecPkt& data);
  void          SendToAll(int type, int player_id, int nbarg, ...);

private:

  // the remaining is private
  friend class SRules;

  CommandList&  getCommitList();
  void          cleanCommitList();
  CommandList   commit_;
};


#endif /* !STECHECSERVER_HH_ */
