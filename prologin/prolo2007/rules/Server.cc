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

#include "Server.hh"

#include "GameData.hh"
#include "ServerEntry.hh"

void Server::addLeucocyte(int team_id, int uid, int x, int y)
{
  g_->players[uid].set_id (uid);
  g_->players[uid].set_player (team_id);
  SendToAll(NEW_LEUCO, uid, 3, team_id, x, y);
}

void Server::sendScore()
{
  for (int i = 0; i < g_->getNbPlayer (); ++i)
    SendToAll(SCORE, i, 1, g_->players[i].getScore ());
}
