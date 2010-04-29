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

void GameData::check(const char * file, int line){
  LOG3("%1:%2", file, line );
  LOG3("current_player:%1", current_player );
  LOG3("unites_allowed : %1", nbr_unites_allowed);
  LOG3("can_play_card : %1", can_play_card);
  LOG3("nbr_toons_spawn: %1 %2", nbr_toons_spawn[0], nbr_toons_spawn[1]);
  for(int i = 0; i < NBR_MAX_UNITES * 2; i ++)
    LOG3("deja_bouge[%1] = %2", i, deja_bougee[i]);
  for(int i = 0; i < unites.size(); i ++)
    LOG3("unite[%1] = {pos: {x:%2, y:%3}, ko:%4, pa:%5, attaques:%6, attaques_gratuites:%7, types:(%8, %9), id:%10}",
	 i, unites[i].pos.x, unites[i].pos.y,
	 unites[i].ko, unites[i].pa, unites[i].attaques, unites[i].attaques_gratuites,
	 unites[i].type_unite_actuel,
	 unites[i].vrai_type_unite, unites[i].id);
  LOG3("players_cartes[0] = {%2, %3, %4, %5}", players_cartes[0].potion, players_cartes[0].deguisement, players_cartes[0].banzai, players_cartes[0].pacifisme);
  LOG3("players_cartes[1] = {%2, %3, %4, %5}", players_cartes[1].potion, players_cartes[1].deguisement, players_cartes[1].banzai, players_cartes[1].pacifisme);
}

#define CHECK check(__FILE__, __LINE__);

GameData::GameData()
{
  current_player = 0; // must be before filling vector unites
  srand(time(0));
  {
    unite u1 = {
      spawn_position(false),
      false,
      PERROQUET,
      PERROQUET,
      0,
      0,
      0,
      0,
      0
    };
    unite u2 = {
      spawn_position(true),
      true,
      PERROQUET,
      PERROQUET,
      0,
      0,
      0,
      0,
      1
    };
    reset_unite(u1, true);
    reset_unite(u2, true);
    unites.push_back(u1);
    unites.push_back(u2);
  };
  {
    cartes c = {1,2,3,4};
    LOG3("cartes = {%2, %3, %4, %5}", c.potion, c.deguisement, c.banzai, c.pacifisme);
  
    players_cartes[0] = c ;
    players_cartes[1] = c ;
  }
  tt.taille = TAILLE_DEPART;
  tt.min_coord = 0;
  tt.max_coord = TAILLE_DEPART - 1;
  CHECK
  reset_moves();
  CHECK
  can_play_card = true;
  nbr_unites_allowed = 3;
  nbr_toons_spawn[0] = 1;
  nbr_toons_spawn[1] = 1;
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
  nbr_unites_allowed = 3;
}

bool GameData::out_map(position p){
  return
    p.x < tt.min_coord || p.x > tt.max_coord ||
    p.y < tt.min_coord || p.y > tt.max_coord
    ;
}

void GameData::retrecissement(){

  tt.taille -= 2;
  tt.min_coord +=1;
  tt.max_coord -= 1;
  LOG3("tt = {size : %1, min : %2, max : %3} ", tt.taille, tt.min_coord, tt.max_coord);
  int end = unites.size() - 1;
  for (int i = 0; i <= end; i++){
    if ( out_map(unites[i].pos)){
      LOG3("switching (%1, %2) and (%4, %4) %5 <=> %6",
	   unites[i].pos.x, unites[i].pos.y,
	   unites[end].pos.x, unites[end].pos.y,
	   i, end);
      unite u = unites[i];
      unites[i] = unites[end];
      unites[end] = u;
      end--;
      LOG3("removing a unit... step 1 : (%1, %2)", u.pos.x, u.pos.y);
      i --;
    }else{
      LOG3("unite (%1, %2) safe,", unites[i].pos.x, unites[i].pos.y);
    }
  }
  for (int s = unites.size() - 1; s >= 0; s --){
    if ( out_map(unites[s].pos)){
       LOG3("removing a unit... step 2 : (%1, %2)", unites[s].pos.x, unites[s].pos.y);
      unites.pop_back();
    }else{
      break;
    }
  }
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
  u.pa = caracteristiques(u.vrai_type_unite).pa_init;
  u.attaques_gratuites = 0;
}

bool GameData::can_active(int i){
  if (deja_bougee[i]){
    return nbr_unites_allowed >= 0;
  } else {
    return nbr_unites_allowed >= 1;
  }
}

void GameData::reset_moves(){
  for (int i = 0; i < NBR_MAX_UNITES * 2; i ++){
    deja_bougee[i] = false;
  }
}

int GameData::nbr_toons(bool e){
  int player = (current_player + e) % 2;
  return nbr_toons_spawn[player];
}

bool GameData::must_remove_ko(){
  for (int i = 0, l = unites.size(); i < l ; i++){
    if (unites[i].ennemi && unites[i].ko > 0 ) return true;
  }
  return false;
}


// accesseurs
cartes GameData::get_cartes(int i){
#ifdef DEBUG
  position p = spawn_pos();
  LOG3("spawn position = %1, %2", p.x, p.y);
#endif
  cartes c = players_cartes[i];
  LOG3("get_cartes(%1) = {%2, %3, %4, %5}", i, c.potion, c.deguisement, c.banzai, c.pacifisme);
  return c;
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
