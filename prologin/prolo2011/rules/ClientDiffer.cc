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

#include "Actions.hh"
#include "ClientDiffer.hh"

ClientDiffer::ClientDiffer(GameData* game, Client* client)
  : StechecClientDiffer(game, client)
{
}

void ClientDiffer::ApplyDiff(const StechecPkt* pkt)
{
  switch (pkt->type){
  case NB_SOURCES:
    g_->sources.reserve(pkt->arg[0]);
    break;
  case CONTENU_CASE:
    {
      int x = pkt->arg[0];
      int y = pkt->arg[1];
      Case &c = g_->get_case(x, y);
      c.bonus = (type_bonus)pkt->arg[2];
      c.type = (type_case)pkt->arg[3];
      c.source_id = pkt->arg[4];
    }
    break;
  case SOURCE_CONTENT:
    {
      SourceEnergie &s = g_->sources.at(pkt->arg[0]);
      s.pos.x = pkt->arg[1];
      s.pos.y = pkt->arg[2];
      s.potentiel_max = pkt->arg[3];
      s.potentiel_cur = pkt->arg[3];
    }
    break;
  default:
    LOG2("ClientDiffer::ApplyDiff");
    if (g_->mon_tour()){
      LOG2("ClientDiffer::mon_tour");
      return;
    }
    Action* act = act_from_pkt(pkt->type, pkt);
    LOG2("type = %1", pkt->type);
    act->appliquer(g_);
    delete act;
  }
}
