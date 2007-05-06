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

#include "Server.hh"

#include "GameData.hh"
#include "ServerEntry.hh"

void Server::addLeucocyte (int team, int uid,
			   int x, int y, int real)
{
   SendToAll(NEW_LEUCO, -1, 5, team, uid, x, y, real);
}
