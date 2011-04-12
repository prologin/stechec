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
    {
      int l = pkt->arg[0];
      LOG6("NB_sources : %1", l);
      g_->sources.reserve(l);
      for (int i = 0; i < l; i++){
	SourceEnergie s;
	g_->sources.push_back(s);
      }
    }
    break;
  case CONTENU_CASE:
    {
      int x = pkt->arg[0];
      int y = pkt->arg[1];
      LOG6("content : (%1, %2)", x, y);
      Case &c = g_->get_case(x, y);
      c.bonus = (type_bonus)pkt->arg[2];
      c.type = (type_case)pkt->arg[3];
      c.source_id = pkt->arg[4];
      LOG6("  - %3", c.type);
    }
    break;
  case SOURCE_CONTENT:
    {
      LOG6("loading source : %1", pkt->arg[0]);
      SourceEnergie &s = g_->sources.at(pkt->arg[0]);
      s.pos.x = pkt->arg[1];
      s.pos.y = pkt->arg[2];
      s.potentiel_max = pkt->arg[3];
      s.potentiel_cur = pkt->arg[3];
    }
    break;
  case MOTO_POS:
    {
	int team = pkt->arg[0];
	position p;
	p.x = pkt->arg[1];
	p.y = pkt->arg[2];
	LOG4("Recv source: (%1, %2)", p.x, p.y);
	g_->creer_trainee_moto(team, p, TAILLE_TRAINEE);
    }
    break;
  default:
    LOG6("ClientDiffer::ApplyDiff - action");
    if (g_->mon_tour()){
      LOG6("ClientDiffer::mon_tour");
      return;
    }
    Action* act = act_from_pkt(pkt->type, pkt);
    g_->stocker_action(act);
    act->appliquer(g_);
    delete act;
  }
}
