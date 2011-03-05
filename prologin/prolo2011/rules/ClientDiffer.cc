/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2011 Prologin
*/

#include "ClientDiffer.hh"

ClientDiffer::ClientDiffer(GameData* game, Client* client)
  : StechecClientDiffer(game, client)
{
}

void ClientDiffer::ApplyDiff(const StechecPkt* pkt)
{
  // TODO
}
