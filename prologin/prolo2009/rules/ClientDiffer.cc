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

#include "ClientDiffer.hh"
#include <cassert>

ClientDiffer::ClientDiffer(GameData* game, Client* client)
  : StechecClientDiffer(game, client)
{
}

void ClientDiffer::ApplyDiff(const StechecPkt *pkt)
{
  // When applying other client's orders, we do not care about score nor about money.
  LOG5("Receiving pkt of type %1, args : %2,%3,%4...", pkt->type,
       pkt->arg[0], pkt->arg[1], pkt->arg[2]);

  if (pkt->type == ENCHERE) {
    int team = pkt->arg[0];
    int montant = pkt->arg[1];
    assert(team >= 0 && team < g_->getNbTeam());
    assert(montant >= 0);
    LOG5("Receiving result of auction for team %1 : %2", team, montant);
    g_->encheres_[team] = montant;
    return;
  }

  int x,y;
  if (pkt->type >= FIRST_CLIENT_MSG && pkt->type < LAST_XY_ORDERS) {
    x = pkt->arg[0], y = pkt->arg[1];
    assert(x >= 0 && x < TAILLE_CARTE);
    assert(y >= 0 && y < TAILLE_CARTE);
  }

  switch (pkt->type)
    {
    case INIT_START_TEAM:
      {
	int start_team = pkt->arg[0];
	assert(start_team >= 0 && start_team < g_->getNbTeam());
	g_->start_team_ = start_team;
	LOG4("Received start_team : %1", start_team);
	break;
      }
    case INIT_MONUMENT:
      {
	int id = pkt->arg[0];
	int prestige = pkt->arg[1];
	int reach = pkt->arg[2];
	assert(id >= 0 && id < MAX_MONUMENTS);
	assert(reach >= 0 && reach <= 10); //sanity check
	assert(prestige >= -10 && prestige <= 10); // sanity check
	g_->monuments_[id] = std::make_pair(prestige, reach);
	LOG5("Receiving monument id : %1, reach : %2, prestige : %3", 
	     id, reach, prestige);
	break;
      }
    case NEXT_MONUMENT:
      {
	int monument = pkt->arg[0];
	assert(monument >= 0 && monument < MAX_MONUMENTS);
	g_->monument_en_cours_ = monument;
	LOG5("Receiving next monument : %1", monument);
	break;
      }
    case NEW_MONEY:
      {
	int team = pkt->arg[0];
	assert(team >= 0 && team < g_->getNbTeam());
	int money = pkt->arg[1];
	assert(money >= 0);
	g_->argent_[team] = money;
	LOG4("receiving new money : %1, for team %2", money, team);
	break;
      }
    case NEW_SCORE:
      {
	int team = pkt->arg[0];
	assert(team >= 0 && team < g_->getNbTeam());
	int score = pkt->arg[1];
	assert(score >= 0);
	g_->score_[team] = score;
	LOG4("Receiving new score for team %1 : %2", team, score);
	break;
      }
    case WON_AUCTION:
      {
	int team = pkt->arg[0];
	assert(team >= 0 && team < g_->getNbTeam());
	g_->won_auction_ = team;
	LOG4("Receiving the winner of the auction : %1", team);
	break;
      }
    case BUILD_ROAD:
      {
	g_->constructions_[y][x] = std::make_pair(ROUTE, -1);
	g_->NotifyCellModified(x, y);
	break;
      }
    case BUILD_HOUSE:
      {
	int team = pkt->arg[2];
	assert(team >= 0 && team < g_->getNbTeam());
	g_->constructions_[y][x] = std::make_pair(MAISON, team);
	g_->NotifyCellModified(x, y);
	break;
      }
    case RESERVE_CELL:
      {
	int team = pkt->arg[2];
	assert(team >= 0 && team < g_->getNbTeam());
	g_->constructions_[y][x] = std::make_pair(RESERVATION, team);
	g_->NotifyCellModified(x, y);
	break;
      }
    case DESTROY_HOUSE:
      g_->constructions_[y][x] = std::make_pair(VIDE, -1);
      g_->NotifyCellModified(x, y);
      break;
      
    case SELL_HOUSE:
      g_->constructions_[y][x] = std::make_pair(MAISON, MAIRIE);
      break;
      
    case BUILD_MONUMENT:
      {
	int monument = pkt->arg[2];
	assert(monument >= 0 && monument < MAX_MONUMENTS);
	g_->constructions_[y][x] = std::make_pair(MONUMENT, monument);
	g_->NotifyCellModified(x, y);
	break;
      }
      // Unexpected packet
    default:
      {
        ERR("Differ get unexpected message, id: %1", pkt->type);
	assert(0);
      }
    }
}
