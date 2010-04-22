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

#include "GameData.hh"
#include <ctime>
#include <cstdlib>

//todo, better, call Init() from BeforeNewGame, etc..
#define INIT()					\
  assert(initialized_);

GameData::GameData()
{
  srand(time(0));
  {
    unite u1 = {
      spawn_position(true),
      false,
      PERROQUET,
      PERROQUET,
      0,
      0,
      0
    };
    unite u2 = {
      spawn_position(false),
      true,
      PERROQUET,
      PERROQUET,
      0,
      0,
      0
    };
    reset_unite(u1, true);
    reset_unite(u2, true);
    unites.push_back(u1);
    unites.push_back(u2);
  };
  current_player = 0;
  {
    cartes init_cartes = {1,1,1,1};
    players_cartes[0] = init_cartes ;
    players_cartes[1] = init_cartes ;
  }
  tt.taille = TAILLE_DEPART;
  tt.min_coord = 0;
  tt.max_coord = TAILLE_DEPART - 1;
  reset_moves();
  can_play_card = true;
  nbr_unites_allowed = 3;
}

void GameData::Init() {
  // TODO
  initialized_ = true;
}

bool GameData::mon_tour()
{
  unsigned int curr_player = (getCurrentTurn() - 1) % 2;
  LOG2("curr_player = %1, team id = %2", curr_player, getTeamId());
  return curr_player == getTeamId();
}

void GameData::team_switched(){
  current_player = (current_player + 1 ) % 2;
  reset_moves(); // nobody has played;
  for (int i = 0, l = unites.size(); i < l ; i++){
    unites[i].ennemi = !unites[i].ennemi; // team switch
    reset_unite(unites[i], false); // reset action points
  }
  can_play_card = true;
}

int GameData::get_current_player(){
  return current_player;
}

int GameData::get_real_turn()
{
  return getCurrentTurn() / 2;
}

// utils functions

int GameData::indice_at(position p){
  for (int i = 0; i < unites.size(); i++){
    unite u2 = unites[i];
    if (u2.pos.x == p.x && u2.pos.y == p.y){
      return i;
    }
  }
  return -1;
}

int GameData::indice_of(unite u){
  return indice_at(u.pos);
}

void GameData::reset_unite(unite &u, bool reset_ko){
  u.type_unite_actuel = u.vrai_type_unite;
  if (reset_ko) u.ko = -1;
  u.attaques = 1;
  u.pa = caracteristiques(u.vrai_type_unite).pa;
}

bool GameData::can_active(int i){
  bool s = deja_bougee[i];
  deja_bougee[i] = true;
  bool out = nbr_unites_activees() <= 3;
  deja_bougee[i] = s;
  return out;
}

void GameData::reset_moves(){
  for (int i = 0; i < 18; i ++){
    deja_bougee[i] = false;
  }
}

int GameData::nbr_unites_activees(){
  int out;
  for (int i = 0; i < 18; i ++){
    out = deja_bougee[i] ? out + 1 : out;
  }
  return out;
}

int GameData::nbr_toons(bool e){
  int nbr;
  for (int i = 0, l = unites.size(); i < l ; i++){
    if (unites[i].ennemi == e ) nbr ++;
  }
  return nbr;
}


// accesseurs
cartes GameData::get_cartes(int i){
  return players_cartes[i];
}

taille_terrain GameData::get_tt(){
  return tt;
}

std::vector<unite> GameData::get_unites(){
  return unites;
}

unite GameData::get_unite(int i){
  return unites[i];
}


// rules functions

position GameData::spawn_pos()
{
  return spawn_position(current_player == 0);
}

position GameData::spawn_position(bool white){
  position r;
  if (white){
    r.x = SPAWN_1_X;
    r.y = SPAWN_1_Y;
  } else {
    r.x = SPAWN_2_X;
    r.y = SPAWN_2_Y;
  }
  return r;
}

caracs GameData::caracteristiques(type_unite tu)
{
  static caracs c[] = {
    { 1, 1 },
    { 2, 3 },
    { 5, 1 },
    { 3, 2 }
  };

  return c[tu];
}

void GameData::set_deja_bougee(int i){
  deja_bougee[i] = true;
}

// actions
void GameData::appliquer_action(Action* a){
  actions.push_back(a);
  a->appliquer(this);
}

bool GameData::annuler(){
  if (actions.empty())
    return false;
  else
  {
    Action* act = actions.back();
    actions.pop_back();

    act->annuler(this);
    delete act;
    return true;
  }
}
