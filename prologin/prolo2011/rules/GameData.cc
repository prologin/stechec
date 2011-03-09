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

#include "GameData.hh"
#include "Constant.hh"
#include <ctime>
#include <cstdlib>

//todo, better, call Init() from BeforeNewGame, etc..
#define INIT()					\
  assert(initialized_);

Case::Case()
    : type(VIDE),
      bonus(PAS_BONUS),
      nb_trainees_moto(0),
      source_id(-1)
{
}

Joueur::Joueur()
    : score(0),
      bonus()
{
}

void SourceEnergie::set_potentiel(int potentiel)
{
    potentiel_max = potentiel;
    potentiel_cur = potentiel;
}

GameData::GameData()
{
  generer_terrain();
}

void
GameData::generer_terrain()
{
  position spawn1, spawn2;

  terrain_.reserve(TAILLE_TERRAIN * TAILLE_TERRAIN);

  // Mise en place des deux joueurs + trainées
  spawn1.x = 0;
  spawn1.y = 0;
  spawn2.x = TAILLE_TERRAIN - 1;
  spawn2.y = TAILLE_TERRAIN - 1;
  InternalTraineeMoto derouleur_1(this, 0, spawn1, TAILLE_TRAINEE);
  InternalTraineeMoto derouleur_2(this, 1, spawn2, TAILLE_TRAINEE);
  motos.push_back(derouleur_1);
  motos.push_back(derouleur_2);
  get_case(spawn1).nb_trainees_moto = 1;
  get_case(spawn2).nb_trainees_moto = 1;
  joueurs.reserve(2);

  // Mise en place des deux sources
  sources.reserve(2);
  sources[0].set_potentiel(100);
  sources[1].set_potentiel(-100);
  spawn1.x = 0;
  spawn1.y = 4;
  spawn2.x = TAILLE_TERRAIN - 1;
  spawn2.y = TAILLE_TERRAIN - 1 - 4;
  sources[0].pos = spawn1;
  sources[1].pos = spawn2;
  get_case(spawn1).source_id = 0;
  get_case(spawn2).source_id = 1;

  srand(time(0));
  current_player = 1;
  can_play = false;
}
void GameData::Init() {
  // TODO
  initialized_ = true;
}

Case&
GameData::get_case(int x, int y)
{
    return terrain_[x + TAILLE_TERRAIN * y];
}

void GameData::team_switched(){
  LOG4("GameData::team_switched");
  can_play = true;
  current_player = (current_player + 1 ) % 2;
  // TODO reset point d'actions
}

void GameData::check(const char * file, int line){
  LOG3("%1:%2", file, line );
  // TODO
}

int GameData::mon_equipe(){
  LOG4("mon_equipe %1", getTeamId());
  return getTeamId();
}
bool GameData::mon_tour()
{
  unsigned int curr_player = (getCurrentTurn() - 1) % 2;
  LOG4("curr_player = %1, team id = %2, current_player = %3", curr_player, getTeamId(), current_player);
  return curr_player == getTeamId();
}
int GameData::get_current_player(){
  return current_player;
}

void GameData::get_scores(std::vector<int>& scores)
{
    scores.reserve(joueurs.size());
    for (int i = 0; i < joueurs.size(); ++i)
        scores[i] = joueurs[i].score;
}

void
GameData::get_sources(std::vector<source_energie>& srcs)
{
    srcs.reserve(sources.size());
    for (int i = 0; i < sources.size(); ++i)
    {
        srcs[i].id = i;
        srcs[i].pos = sources[i].pos;
        srcs[i].coef = sources[i].potentiel_cur;
    }
}

void
GameData::get_bonus_joueur(int joueur, std::vector<type_bonus>& bonus)
{
    bonus = joueurs[joueur].bonus;
}

int GameData::get_real_turn()
{
  return (getCurrentTurn() - 1) / 2 + 1;
}

bool GameData::isMatchFinished(){
  return getCurrentTurn() == FIN_PARTIE * 2;
}


// actions
void GameData::appliquer_action(Action* a){
  actions.push_back(a);
  a->appliquer(this);
}

extern Api* api;
void GameData::send_actions()
{
  for (std::vector<Action*>::iterator it = actions.begin();
       it != actions.end(); ++it)
  {
    (*it)->envoyer(api);
    delete *it;
  }
  actions.resize(0);
}

bool GameData::annuler(){
  if (actions.empty()){
    LOG3("annuler : empty stack");
    return false;
  }
  else
  {
    Action* act = actions.back();
    actions.pop_back();
    LOG3("annuler : process...");
    act->annuler(this);
    delete act;
    return true;
  }
}
