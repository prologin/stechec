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
    data_unites.push_back(u1);
    data_unites.push_back(u2);
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
  data_can_play_card = true;
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
  for (int i = 0, l = data_unites.size(); i < l ; i++){
    data_unites[i].ennemi = !data_unites[i].ennemi; // team switch // todo ne pas avoir besoin de faire ca.
    reset_unite(data_unites[i], false); // reset action points
  }
  data_can_play_card = true;
}

// utils functions

int GameData::toon_at(position p){
  for (int i = 0; i < data_unites.size(); i++){
    unite u2 = data_unites[i];
    if (u2.pos.x == p.x && u2.pos.y == p.y){
      return i;
    }
  }
  return -1;
}

int GameData::indice_of(unite u){
  return toon_at(u.pos);
}

void GameData::reset_unite(unite &u, bool reset_ko){
  u.type_unite_actuel = u.vrai_type_unite;
  if (reset_ko) u.ko = -1;
  u.attaques = 1;
  u.pa = pa(u.vrai_type_unite);
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
  for (int i = 0, l = data_unites.size(); i < l ; i++){
    if (data_unites[i].ennemi == e ) nbr ++;
  }
  return nbr;
}


// accesseurs

int GameData::get_current_player(){
  return current_player;
}
cartes GameData::get_cartes(int i){
  return players_cartes[i];
}

taille_terrain GameData::get_tt(){
  return tt;
}

std::vector<unite> GameData::get_unites(){
  return data_unites;
}

unite GameData::get_unite(int i){
  return data_unites[i];
}


// rules functions

inline position GameData::spawn_pos(){
  return spawn_position(current_player == 0);
}

position GameData::spawn_position(bool white){
  position r;
  if (white){
    r.x=7;
    r.y=2;
  }else{
    r.x=7;
    r.y=13;
  }
  return r;
}

int GameData::porte_attaque(const unite u){
  switch (u.type_unite_actuel){
  case PERROQUET:
    return 1;
  case SINGE:
    return 1;
  case CHAT:
    return 3;
  case KANGOUROU:
    return 2;
  }
}

void GameData::set_deja_bougee(int i){
  deja_bougee[i] = true;
}

int GameData::pa(const type_unite u){
  switch ( u ){
  case PERROQUET:
    return 1;
  case SINGE:
    return 3;
  case CHAT:
    return 5;
  case KANGOUROU:
    return 2;
  }
}

// internals side effects
 
void GameData::do_deplacer(int indice, position pos, int d){
  deja_bougee[indice] = true;
  data_unites[indice].pa -= d;
  data_unites[indice].pos = pos;
}

void GameData::do_spawn(unite u)
{
  deja_bougee[data_unites.size()] = true;
  data_unites.push_back(u);
}

// actions

bool GameData::annuler(){
  return false; // todo
}

void GameData::appliquer_action(Actions a){
  position p;
  actions.push_back(a);
  switch (a.type){
  case e_spawn:
    p = spawn_pos();
    {
      unite u = {
	p,
	false,
	a.tu,
	a.tu,
	-1,
	0,
	0
      };
      do_spawn(u);
    }
    break;
  case e_deplacer:
    do_deplacer(a.i1, a.p, a.d);
  case e_attaquer: // todo
    break;
  case e_deguisement: // todo
    break;
  case e_banzai: // todo
    break;
  case e_pacifisme: // todo
    break;
  case e_soin: // todo
    break;
  }
}
