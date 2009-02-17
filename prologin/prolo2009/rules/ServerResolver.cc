/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2008 Prologin
*/

#include <vector>
#include "ServerResolver.hh"

ServerResolver::ServerResolver(GameData* game, Server* server)
  : StechecServerResolver(game, server)
{

}
// Get the order (first, second, third, etc...) by which the action was done by the client.
// (ensure that different orders modifying the same cell will be properly handled).
static int GetOrder(StechecPkt *pkt) {
  int order = -1;
  switch(pkt->type) {
  case BUILD_ROAD:
  case BUILD_MONUMENT:
    order = pkt->arg[3];
    break;
  case BUILD_HOUSE:
  case DESTROY_HOUSE:
    order = pkt->arg[5];
    break;
  case RESERVE_CELL:
  case SELL_HOUSE:
    order = pkt->arg[4];
    break;
  default:
    assert(0);
  }
  assert(order >= 0);
  return order;
}

void ServerResolver::CopyPassThrewOrder(CommandListRef cmdList[])
{
  static bool already_modified[TAILLE_CARTE][TAILLE_CARTE];  
  std::fill(*already_modified, *already_modified + TAILLE_CARTE * TAILLE_CARTE, false);

  static bool delete_it[MAX_ORDERS];
  std::fill(delete_it, delete_it + MAX_ORDERS, false);

  std::fill(*g_->pass_threw_orders_, *g_->pass_threw_orders_ + MAX_ORDERS * 10, -1);
  g_->nb_pass_threw_orders_ = 0;
  
  for (int i = FIRST_CLIENT_MSG ; i < LAST_XY_ORDERS ; ++i) {
    for (CommandListRef::iterator cmd = cmdList[i].begin() ; cmd != cmdList[i].end() ; ++cmd) {
      StechecPkt *pkt= *cmd;
      const int order = GetOrder(pkt);
      assert(order >= 0 && order < MAX_ORDERS);
      g_->pass_threw_orders_[order][0] = pkt->type;
      std::copy(pkt->arg, pkt->arg + 6, &g_->pass_threw_orders_[order][1]);
      g_->nb_pass_threw_orders_ = std::max(g_->nb_pass_threw_orders_, order + 1);
      assert(g_->nb_pass_threw_orders_ < MAX_ORDERS);
    }
  }
 
  for (int i = g_->nb_pass_threw_orders_ - 1 ; i >= 0 ; --i) {
    if (g_->pass_threw_orders_[i][0] == -1) {
      // this order is garbage.
      delete_it[i] = true;
      continue;
    }
    const int x = g_->pass_threw_orders_[i][1];
    const int y = g_->pass_threw_orders_[i][2];
    assert(x >= 0 && x < TAILLE_CARTE);
    assert(y >= 0 && y < TAILLE_CARTE);
    bool &m = already_modified[y][x];
    if (m) {
      delete_it[i] = true;
    } else {
      m = true;
    }
  }
  int j = 0;
  for (int i = 0 ; i < g_->nb_pass_threw_orders_ ; ++i) {
    if (delete_it[i]) {
      continue;
    }    
    if (i != j) {
      std::copy(g_->pass_threw_orders_[i], g_->pass_threw_orders_[i] + 10, g_->pass_threw_orders_[j]);
    }
    ++j;
  }
  g_->nb_pass_threw_orders_ = j;

  // Now, every cell is altered (at most) once among all the orders (by the latest order).

  // send only in second phase, otherwise 0 auctions will be incorrectly sent.
  if (g_->GetCurrentPhase() == 2) {
    // We just copy the auction orders, *and* append 0 auctions for those who haven't made any:
    int &i = g_->nb_pass_threw_orders_;
    bool did_auction[MAX_TEAMS] = {0};
    for (CommandListRef::iterator cmd = cmdList[ENCHERE].begin() ; cmd != cmdList[ENCHERE].end(); ++cmd) {  
      StechecPkt *pkt = *cmd;
      const int team = pkt->arg[0];
      assert(team >= 0 && team < g_->getNbTeam());
      did_auction[team] = true;
      g_->pass_threw_orders_[i][0] = ENCHERE;
      std::copy(pkt->arg, pkt->arg + 2, &g_->pass_threw_orders_[i][1]);
      i++;
    }
    for (int t = 0 ; t < g_->getNbTeam() ; ++t) {
      if (!did_auction[t]) {
	int *order = g_->pass_threw_orders_[i];
	order[0] = ENCHERE;
	order[1] = t;
	order[2] = 0;
	i++;
      }
    }
  }
  
  assert(g_->nb_pass_threw_orders_ <= MAX_ORDERS);
}

void ServerResolver::ResolveMoneyAndScores(CommandListRef cmdList[])
{
  for (int i = FIRST_CLIENT_MSG ; i < LAST_MSG ; ++i) {
    for (CommandListRef::iterator cmd = cmdList[i].begin() ; cmd != cmdList[i].end() ; ++cmd) {
      StechecPkt *pkt= *cmd;
      int team = pkt->client_id;
      assert(team >= 0 && team < g_->getNbTeam());
      switch (i) {
      case BUILD_ROAD:	
	g_->argent_[team] -= pkt->arg[2];
	break;
      case BUILD_HOUSE:
	g_->argent_[team] -= pkt->arg[3];
	g_->score_[team] += pkt->arg[4];
	break;
      case RESERVE_CELL:
	g_->argent_[team] -= pkt->arg[3];
	break;
      case DESTROY_HOUSE:
	{
	  g_->argent_[team] -= pkt->arg[2];
	  const int target = pkt->arg[3];
	  assert(target >= 0 && target < g_->getNbTeam() || target == MAIRIE);
	  if (target != MAIRIE)
	    g_->score_[target] += pkt->arg[4];
	  else
	    assert(pkt->arg[4] == 0);
	   
	  break;
	}
      case SELL_HOUSE:
	g_->argent_[team] += pkt->arg[2];
	g_->score_[team] += pkt->arg[3];
	break;
      case SCORE_UPDATE:
	{
	  const int target = pkt->arg[0];
	  assert(target >= 0 && target < g_->getNbTeam());
	  g_->score_[target] += pkt->arg[1];
	}
	break;
      case BUILD_MONUMENT:
      case ENCHERE:
        break;
      default:
	assert(0);
      }
    }
  } 
}

void ServerResolver::ApplyResolver(CommandListRef cmdList[])
{
  // Indeed, the server needn't to know anything about the map !
  // The only things the resolver takes care about are :
  // - the money of each player
  // - the one who won the auction
  // - the score of each player.

  CopyPassThrewOrder(cmdList);

  ResolveMoneyAndScores(cmdList);

  // auction
  if (g_->GetCurrentPhase() == 2) {
    for (int m = 0 ; m < LAST_MSG ; ++m) 
      // Only sell actions are allowed during auction turn.
      if (m != ENCHERE && m != SELL_HOUSE)
	assert(cmdList[m].empty());

    int best_auction = 0;
    int encheres[MAX_TEAMS] = {0};
    bool did_auction[MAX_TEAMS] = {0};
    for (CommandListRef::iterator cmd = cmdList[ENCHERE].begin() ; cmd != cmdList[ENCHERE].end(); ++cmd) {
      const int team = (*cmd)->arg[0];
      const int montant = (*cmd)->arg[1];
      assert(team >= 0 && team < g_->getNbTeam());
      assert(!did_auction[team]);
      did_auction[team] = true;
      encheres[team] = montant;
      best_auction = std::max(montant, best_auction);
    }
    LOG4("nbteam %1", g_->getNbTeam());
    g_->won_auction_ = -1;
    // Determine the winner of the auction.
    for (int p = 0 ; p < g_->getNbTeam() ; p++) {
      // Examine teams backwards.
      const int nbteam = g_->getNbTeam();
      const int team =  (g_->GetFirstPlayerPhase1() + nbteam - 1 - p) % nbteam;
      assert(team >= 0 && team < g_->getNbTeam());
      if (encheres[team] == best_auction) {
	LOG4("Team %1 wins auction while spending %2", team, best_auction);
	g_->won_auction_ = team;
	g_->argent_[team] -= best_auction;
	break;
      }
    }
    assert(g_->won_auction_ != -1);
  } else {
    assert(cmdList[ENCHERE].empty());
  }
  
  g_->MakeChecks(true);
}
