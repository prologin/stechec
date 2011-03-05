/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#include <stdlib.h>

#include "Api.hh"

// global used in interface.cc
Api* api;

static const int directions4[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
static const int directions8[8][2] = {{0,1},{0,-1},{1,0},{-1,0},
                                      {1,1},{1,-1},{-1,1},{-1,-1}};

static int ScoreConstructionDepuisValeurCase(int valeur_case) {
  return std::max(valeur_case + 1, 0);
}


Api::Api(GameData* gameData, Client* c)
  : StechecApi(gameData, c)
{
  api = this;
}

int Api::TeamFromServer(int team)
{
  assert(team >= 0);
  const int m = g_->getNbTeam();
  if (team >= m) {
    return team;
  } else {
    return (team + m - g_->getTeamId()) % m;
  }
}

int Api::TeamFromChampion(int team)
{
  const int m = g_->getNbTeam();
  assert(team >= 0 && team < g_->getNbTeam());
  return (team + m + g_->getTeamId()) % m;
}

int Api::type_case(int x, int y)
{
  CHECK_POS(x, y);
  return g_->constructions_[y][x].first;
}

int Api::valeur_case(int x, int y)
{
  CHECK_POS(x, y);
  return g_->valeurs_cases_[y][x];
}

int Api::appartenance(int x, int y)
{
  CHECK_POS(x, y);
  const int t = type_case(x, y);
  if (t != MAISON && t != RESERVATION)
    return MAIRIE;
  return TeamFromServer(g_->constructions_[y][x].second);
}

int Api::type_monument(int x, int y)
{
  CHECK_POS(x, y);
  if (type_case(x, y) != MONUMENT)
    return PAS_DE_MONUMENT;
  return g_->constructions_[y][x].second;
}

int Api::portee_monument(int num_monument)
{
  CHECK_MONUMENT(num_monument);
  return g_->monuments_[num_monument].second;
}

int Api::prestige_monument(int num_monument)
{
  CHECK_MONUMENT(num_monument);
  return g_->monuments_[num_monument].first;
}

int Api::numero_tour()
{
  return g_->GetRealTurn();
}

int Api::commence()
{
  return TeamFromServer(g_->GetFirstPlayerPhase1());
}

int Api::montant_encheres(int num_joueur)
{
  CHECK_TEAM(num_joueur);
  return g_->encheres_[TeamFromChampion(num_joueur)];
}

int Api::vainqueur_encheres() {
  if (g_->won_auction_ < 0) {
    assert((g_->GetCurrentPhase() == 1 || g_->GetCurrentPhase() == 2)
	   && g_->GetRealTurn() == 1);
    return 0;
  }
  return TeamFromServer(g_->won_auction_);
}

int Api::score(int num_joueur)
{
  CHECK_TEAM(num_joueur);  
  return g_->score_[TeamFromChampion(num_joueur)];
}

int Api::finances(int num_joueur)
{
  CHECK_TEAM(num_joueur);
  return g_->argent_[TeamFromChampion(num_joueur)];
}

int Api::monument_en_cours()
{
  const int ret = g_->monument_en_cours_;
  if (ret >= 0)
    return ret;
  else
    return PAS_DE_MONUMENT;
}

int Api::distance(int x1, int y1, int x2, int y2)
{
  CHECK_POS(x1, y1);
  CHECK_POS(x2, y2);
  const int dx = std::abs(x1 - x2);
  const int dy = std::abs(y1 - y2);
  if (dx + dy > MAX_DISTANCE) {
    return TROP_LOIN;
  }
  if (y2 < y1) {
    std::swap(y1, y2);
    std::swap(x1, x2);
  }
  // Now y2 >= y1, and (x1,y1) is taken to be the origin.
  const int l = y2 - y1;
  const int c = MAX_DISTANCE + x2 - x1;
  assert(l >= 0 && l <= MAX_DISTANCE);
  assert(c >= 0 && c <= 2 * MAX_DISTANCE);
  int d = g_->distances_[y1][x1][l][c];
  if (d > MAX_DISTANCE) {
    return TROP_LOIN;
  } else {
    return d;
  }
}

bool Api::check_pos(int x, int y) {
  return !(x < 0 || y < 0 || x >= TAILLE_CARTE || y >= TAILLE_CARTE);
}

bool Api::CanBuild(int x, int y) {
  ASSERT_POS(x, y);
  const std::pair<int, int> &c = g_->constructions_[y][x];
  if (c.first != VIDE && c.first != RESERVATION) {
    return false;
  }
  if (c.first == RESERVATION && c.second != g_->getTeamId()) {
    return false;
  }
  return true;
}

int Api::route_possible(int x, int y)
{
  CHECK_POS(x, y);
  if (!CanBuild(x, y))
    return CASE_OCCUPEE;
  for (int d = 0 ; d < 4 ; d++) {
    const int xx = x + directions4[d][0];
    const int yy = y + directions4[d][1];
    if (!check_pos(xx, yy))
      continue;
    if (g_->constructions_[yy][xx].first == ROUTE)
      return SUCCES;
  }
  return NON_CONNEXE;
}

int Api::construction_possible(int x, int y)
{
  CHECK_POS(x, y);
  if (!CanBuild(x, y))
    return CASE_OCCUPEE;

  if (!g_->cases_non_blocantes_[y][x])
    return BLOCAGE;

  for (int d = 0 ; d < 8 ; d++) {
    const int xx = x + directions8[d][0];
    const int yy = y + directions8[d][1];
    if (!check_pos(xx, yy))
      continue;
    if (g_->constructions_[yy][xx].first == ROUTE)
      return SUCCES;
  }

  return NON_CONNEXE;
}

int Api::cout_achat_maison()
{
  const int c = g_->CoutConstructions(g_->routes_construites_,
				      g_->maisons_construires_ + 1);
  if (c >= INFINI) return INFINI;
  return c - g_->CoutConstructions(g_->routes_construites_,
				   g_->maisons_construires_);
}

int Api::cout_achat_route()
{
  const int c = g_->CoutConstructions(g_->routes_construites_ + 1,
				      g_->maisons_construires_);
  if (c >= INFINI) return INFINI;
  return c - g_->CoutConstructions(g_->routes_construites_,
				   g_->maisons_construires_);
}

bool Api::mon_tour()
{
  return g_->mon_tour();
}

int Api::construire_route(int x, int y)
{
  if (!mon_tour())
    return ACTION_INTERDITE;
  const int p = g_->GetCurrentPhase();
  if (p == 2)
    return ACTION_INTERDITE;
  const int r = route_possible(x, y);
  if (r != SUCCES) {
    return r;
  }
  const int c = cout_achat_route();    
  int &a = g_->argent_[g_->getTeamId()];
  if (c > a) {
    return FINANCES_DEPASSEES;
  }

  // Construction en elle-même :
  LOG2("Building road at (%1,%2); cost=%3", x, y, c);
  a -= c;
  g_->constructions_[y][x] = std::make_pair(ROUTE, -1);
  g_->ComputeNonBlockingCells();
  g_->routes_construites_++;
  g_->NotifyCellModified(x, y);
  StechecPkt com(BUILD_ROAD, -1);
  com.Push(4, x, y, c, g_->ordre_++);
  SendToServer(com);
  return SUCCES;
}

int Api::construire_maison(int x, int y)
{
  if (!mon_tour())
    return ACTION_INTERDITE;
  if (g_->GetCurrentPhase() != 1)
    return ACTION_INTERDITE;
  const int m = construction_possible(x, y);
  if (m != SUCCES)
    return m;
  const int c = cout_achat_maison();
  if (c == INFINI)
    return ACTION_INTERDITE;
  int &a = g_->argent_[g_->getTeamId()];
  if (c > a)
    return FINANCES_DEPASSEES;
    
  // construction :
  LOG2("Building house at (%1,%2); cost=%3", x, y, c);
  a -= c;
  g_->constructions_[y][x] = std::make_pair(MAISON, g_->getTeamId());
  g_->maisons_construires_++;
  const int score_inc = ScoreConstructionDepuisValeurCase(g_->valeurs_cases_[y][x]);
  g_->score_[g_->getTeamId()] += score_inc;
  g_->NotifyCellModified(x, y);
  g_->ComputeNonBlockingCells();
  StechecPkt com(BUILD_HOUSE, -1);
  com.Push(6, x, y, g_->getTeamId(), c, score_inc, g_->ordre_++);
  SendToServer(com);
  return SUCCES;
}

int Api::reserver_case(int x, int y)
{
  if (!mon_tour())
    return ACTION_INTERDITE;
  if (g_->GetCurrentPhase() != 1)
    return ACTION_INTERDITE;
  int m = construction_possible(x, y);
  if (m != SUCCES)
    return m;
  // Cas non traité par construction_possible.
  if (g_->constructions_[y][x].first == RESERVATION)
    return CASE_OCCUPEE;
  const int c = std::max(1, g_->valeurs_cases_[y][x]);
  int &a = g_->argent_[g_->getTeamId()];
  if (c > a)
    return FINANCES_DEPASSEES;
    
  // construction :
  LOG2("Booking cell at (%1,%2); cost=%3", x, y, c);
  a -= c;
  g_->constructions_[y][x] = std::make_pair(RESERVATION, g_->getTeamId());
  g_->NotifyCellModified(x, y);
  g_->ComputeNonBlockingCells();
  StechecPkt com(RESERVE_CELL, -1);
  com.Push(5, x, y, g_->getTeamId(), c, g_->ordre_++);
  SendToServer(com);
  return SUCCES;
}

int Api::detruire_maison(int x, int y)
{
  CHECK_POS(x, y);
  if (!mon_tour())
    return ACTION_INTERDITE;
  const int phase = g_->GetCurrentPhase();
  if (phase == 2)
    return ACTION_INTERDITE;
  if (g_->constructions_[y][x].first != MAISON)
    return PAS_DE_MAISON;
  const int c = std::max(1, g_->valeurs_cases_[y][x]);
  int &a = g_->argent_[g_->getTeamId()];
  if (c > a)
    return FINANCES_DEPASSEES;
    
  // action:
  LOG2("Destroying house at (%1,%2); cost=%3", x, y, c);
  a -= c;
  const int target = g_->constructions_[y][x].second;
  g_->constructions_[y][x] = std::make_pair(VIDE, -1);
  g_->NotifyCellModified(x, y);
  g_->ComputeNonBlockingCells();
  int score_inc = 0;
  if (target != MAIRIE) {
    score_inc = - ScoreConstructionDepuisValeurCase(g_->valeurs_cases_[y][x]);
    g_->score_[target] += score_inc;
  }
  StechecPkt com(DESTROY_HOUSE, -1);
  com.Push(6, x, y, c, target, score_inc, g_->ordre_++);
  SendToServer(com);
  return SUCCES;
}

int Api::vendre_maison(int x, int y)
{
  CHECK_POS(x, y);
  if (!mon_tour())
    return ACTION_INTERDITE;
  if (g_->constructions_[y][x].first != MAISON)
    return PAS_DE_MAISON;
  if (g_->constructions_[y][x].second != g_->getTeamId())
    return ACTION_INTERDITE; // change that ? todo

  const int earning = ScoreConstructionDepuisValeurCase(g_->valeurs_cases_[y][x]);

  // action:
  LOG2("Selling house to town hall at (%1,%2); earning=%3", x, y, earning);
  g_->argent_[g_->getTeamId()] += earning;
  g_->constructions_[y][x].second = MAIRIE;
  int score_inc = - earning;
  g_->score_[g_->getTeamId()] += score_inc;
  StechecPkt com(SELL_HOUSE, -1);
  com.Push(5, x, y, earning, score_inc, g_->ordre_++);
  SendToServer(com);
  return SUCCES;
}

int Api::encherir(int montant)
{
  if (!mon_tour())
    return ACTION_INTERDITE;
  if (g_->GetCurrentPhase() != 2)
    return ACTION_INTERDITE;
  if (montant < 0)
    return ACTION_INTERDITE; // todo : improve that ?
  if (montant > g_->argent_[g_->getTeamId()])
    return FINANCES_DEPASSEES;
  if (g_->did_auction_)
    return ACTION_INTERDITE;

  LOG2("Making a bid of %1 for monument number %2", montant, g_->monument_en_cours_);
  g_->did_auction_ = true;
  StechecPkt com(ENCHERE, -1);
  com.Push(2, g_->getTeamId(), montant);
  SendToServer(com);

  return SUCCES;
}

int Api::construire_monument(int x, int y)
{
  CHECK_POS(x, y);
  if (!mon_tour())
    return ACTION_INTERDITE;
  if (g_->GetCurrentPhase() != 3)
    return ACTION_INTERDITE;

  const int m = construction_possible(x, y);
  if (m != SUCCES)
    return m;
    
  // construction :
  LOG2("Building monument number %1 at (%1,%2)", g_->monument_en_cours_, x, y);
  g_->constructions_[y][x] = std::make_pair(MONUMENT, g_->monument_en_cours_);
  g_->NotifyCellModified(x, y);
  g_->ComputeNonBlockingCells();
  {
    StechecPkt com(BUILD_MONUMENT, -1);
    com.Push(4, x, y, g_->monument_en_cours_, g_->ordre_++);
    SendToServer(com);
  }

  int score_inc[MAX_TEAMS] = {0};
  const int portee = portee_monument(g_->monument_en_cours_);
  const int prestige = prestige_monument(g_->monument_en_cours_);
  for (int i = y - portee ; i <= y + portee ; ++i) {
    for (int j = x - portee ; j <= x + portee ; ++j) {
      if (i < 0 || j < 0 || i >= TAILLE_CARTE || j >= TAILLE_CARTE)
	continue;
      int &v = g_->valeurs_cases_[i][j];
      const int old_house_val = ScoreConstructionDepuisValeurCase(v);
      v += prestige;
      if (g_->constructions_[i][j].first != MAISON)
	continue;
      if (g_->constructions_[i][j].second == MAIRIE)
	continue;
      const int new_house_val = ScoreConstructionDepuisValeurCase(v);
      assert(g_->constructions_[i][j].second >= 0 && g_->constructions_[i][j].second < g_->getNbTeam());
      score_inc[g_->constructions_[i][j].second] += new_house_val - old_house_val;
    }
  }
  
  static int score_update_cnt = 0;

  for (int t = 0 ; t < g_->getNbTeam() ; ++t) {
    g_->score_[t] += score_inc[t];
    StechecPkt com(SCORE_UPDATE, -1);
    com.Push(3, t, score_inc[t], score_update_cnt++);
    SendToServer(com);
  }

  return SUCCES;    
}


void Api::teamSwitched()
{
}
