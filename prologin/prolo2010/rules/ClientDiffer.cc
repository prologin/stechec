/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2010 Prologin
*/

#include "Actions.hh"
#include "ClientDiffer.hh"

ClientDiffer::ClientDiffer(GameData* game, Client* client)
  : StechecClientDiffer(game, client)
{
}

void ClientDiffer::ApplyDiff(const StechecPkt* pkt)
{
  if (pkt->type == RETRECIR){
    g_->retrecissement();
  }else{
    if (g_->mon_tour())
      return;
    Action* act = act_from_pkt(pkt->type, pkt);
    act->appliquer(g_);
    delete act;
  }
}
