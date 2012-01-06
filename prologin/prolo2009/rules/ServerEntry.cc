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

#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include "GameData.hh"
#include "ServerEntry.hh"
#include "misc/Conf.hh"

ServerEntry::ServerEntry(GameData* game, Server* server,
                         const ConfSection* cfg)
  : StechecServerEntry(game, server, cfg)
{
}

ServerEntry::~ServerEntry(void)
{
}


int ServerEntry::loadMap(void)
{
  // This could be read from a map file, but it's fixed for now.
  // prestige, portée.
  const int monuments[MAX_MONUMENTS][2] = {{5,1},
					   {4,1},
					   {4,1},
					   {3,1},
					   {3,2},
					   {2,2},
					   {2,2},
					   {2,2},
					   {2,1},
					   {1,3},
					   {1,3},
					   {1,2},
					   {-1,2},
					   {-2,1}};

  for (int m = 0 ; m < MAX_MONUMENTS ; ++m)
    g_->monuments_[m] = std::make_pair(monuments[m][0], monuments[m][1]);

  std::string map_file;
  std::string line;

  map_file = cfg_->getValue<std::string>("map");
  LOG2("Map file is: %1", map_file);
  std::ifstream	map_stream;
  
  map_stream.open(map_file.c_str());
  if (!map_stream)
  {
    ERR("Unable to load %1", map_file);
    return 1;
  }
  bool randomize = false;
  // If the first lines are comments, discard them
  while (map_stream.peek() == '#')
  {
    getline(map_stream, line); 
    if (line.find("#randomize") == 0) {      
      randomize = true;
    }
  }
  
  int nb_monuments;
  if (!(map_stream >> nb_monuments) || nb_monuments <= 0 || nb_monuments > MAX_MONUMENTS) {
    ERR("Map file should start with a number of monuments that is between 1 and %1", MAX_MONUMENTS);
    return 1;
  }
  g_->nb_monuments_ = nb_monuments;

  if (randomize) {
    LOG1("Monuments are randomly shuffled");
    for (int i = 0 ; i < MAX_MONUMENTS ; i++) {
      g_->monument_order_[i] = i;
    }
    std::random_shuffle(g_->monument_order_, g_->monument_order_ + MAX_MONUMENTS);
    return 0;
  }

  for (int m = 0 ; m < nb_monuments ; m++) {
    int p;
    if (!(map_stream >> p)) {
      ERR("Error in map file : the %1-th monument is unreadable", m+1);
      return 1;
    }
    if (p < 1 || p > MAX_MONUMENTS) {
      ERR("The %1-th monument : %2, should be between 1 and %3", m+1, p, MAX_MONUMENTS);
      return 1;
    }
    g_->monument_order_[m] = p - 1;
  }
  
  return 0;
}


int		ServerEntry::beforeGame(void)
{

  g_->Init();

  if (loadMap())
    return 1;

  srand(time(0));
  g_->start_team_ = rand() % g_->getNbTeam();
  SendToAll(INIT_START_TEAM, -1, 1, g_->start_team_);
  LOG5("Sending start_team_ : %1", g_->start_team_);
  LOG2("Team %1 will start the game", g_->start_team_);

  for (int m = 0 ; m < MAX_MONUMENTS ; ++m) {
    SendToAll(INIT_MONUMENT, -1, 3, m, g_->monuments_[m].first, g_->monuments_[m].second);
    LOG5("Sending monument %1 : (%2, %3)", m, g_->monuments_[m].first, g_->monuments_[m].second);
  }
  
  return 0;
}

int         ServerEntry::initGame(void)
{
  return 0;
}

int         ServerEntry::beforeNewTurn(void)
{
  // warning : msg seems not to be transmitted in beforeNewTurn !
  return 0;
}

int         ServerEntry::afterNewTurn(void)
{
  // transmit the money
  // transmit the scores
  // retransmit all msgs in pass_threw_orders_

  if (g_->GetCurrentPhase() == 2) { // enchères
    LOG5("Transmitting winner of auction : %1", g_->won_auction_);
    SendToAll(WON_AUCTION, -1, 1, g_->won_auction_);   
  }
  
  for (int t = 0 ; t < g_->getNbTeam() ; t++) {
    LOG5("Transmitting new money/score for team %1 : %2/%3", t, g_->argent_[t], g_->score_[t]);
    SendToAll(NEW_MONEY, -1, 2, t, g_->argent_[t]);
    SendToAll(NEW_SCORE, -1, 2, t, g_->score_[t]);
  }
  
  for (int i = 0 ; i < g_->nb_pass_threw_orders_ ; ++i) {    
    int* order = g_->pass_threw_orders_[i];
    LOG5("Re-tranmitting orders from clients ; type : %1, args : %2,%3,%4,%5..", order[0], order[1], order[2], order[3], order[4]);
    // We suppose here that there will never be more than 6 arguments to the orders.
    // Note that for orders with less than 6 arguments, we transmit garbage as well.
    assert(MAX_PARAM_PER_ORDER >= 6);
    SendToAll(order[0], -1, 6, order[1], order[2],
	      order[3], order[4], order[5], order[6]);
  }

  if (g_->IsPhase1Ending() && g_->GetRealTurn() <= g_->nb_monuments_) {
    int m = g_->GetRealTurn() - 1;    
    LOG5("Transmitting next monument : %1", g_->monument_order_[m]);
    SendToAll(NEXT_MONUMENT, -1, 1, g_->monument_order_[m]);
    g_->monument_en_cours_ = g_->monument_order_[m];
  }
  return 0;
}


int         ServerEntry::afterGame(void)
{
  return 0;
}

bool        ServerEntry::isMatchFinished(void)
{
  return g_->IsPhase1Ending() && g_->GetRealTurn() == g_->nb_monuments_ + 1;
}

int ServerEntry::getScore(int uid)
{
  int scores[MAX_TEAMS] = {0};
  for (int i = 0 ; i < MAX_TEAMS ; i++) {
    scores[i] = g_->score_[i] + g_->argent_[i];
  }
  // Counts the number of team that has a strictly better or an equal score to the team "uid".
  int cnt_better_or_equal = 0;
  for (int i = 0 ; i < MAX_TEAMS ; i++) {
    if (i == uid) {
      continue;
    }
    if (scores[i] >= scores[uid]) {
      ++cnt_better_or_equal;
    }
  }
  switch(cnt_better_or_equal) {
  case 0:
    return 3; // score absolute winner
    break;
  case 1:
    return 1; // either second place (and strictly better than third), or first place drawn with second.
    break;
  case 2:
    return 0; // either 3 drawns, or third drawn with second.
    break;
  default:
    assert(0);
    break;
  }
}
