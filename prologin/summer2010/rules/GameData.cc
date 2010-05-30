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

bool operator<(position p1, position p2){
  return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
}

//todo, better, call Init() from BeforeNewGame, etc..
#define INIT()					\
  assert(initialized_);

GameData::GameData()
{
  srand(time(0));
  next_indice0 = 0;
  can_move = false;
  current_player = 1;
  for (int dx = 0; dx < 3; dx ++){
    for (int dy = 0; dy < 3; dy ++){
      position p_white = {dx, dy};
      position p_black = {(TAILLE_TERRAIN - dx - 1), (TAILLE_TERRAIN - dy - 1)};
      push_empty_player(p_white, 0);
      push_empty_player(p_black, 1);
    }
  }
  score_team[0] = 0;
  score_team[1] = 0;
  player_end_match = false;
}

bool GameData::mon_tour()
{
  LOG2("curr_player = %1, team id = %2", current_player, getTeamId());
  return current_player == getTeamId();
}

void GameData::Init() {
  initialized_ = true;
}

void GameData::push_empty_player(const position p, const int team){
  unite u;
  static int id = 0;
  id++;
  u.pos_unite = p;
  u.team = team;
  u.ko = 0;
  u.pa = MAX_PA;
  u.id = id;
  u.objet = RIEN;
  map_u[p] = u;
}
piece GameData::push_random_piece() {
  int next_indice0_old = next_indice0;
  random_piece(next[next_indice0]);
  next_indice0 = (next_indice0 + 1) % NB_PIECES_SUIVANTES_VISIBLES;
  return next[next_indice0_old];
}
void GameData::push_piece(int temps, int valeur, position p) {
  int next_indice0_old = next_indice0;
  next[next_indice0].valeur = valeur;
  next[next_indice0].tour_apparition = temps;
  next[next_indice0].pos_piece = p;
  next_indice0 = (next_indice0 + 1) % NB_PIECES_SUIVANTES_VISIBLES;
}
bool GameData::case_occupee(const position p){
  return map_p.count(p) != 0 || map_u.count(p) != 0;
}
void GameData::random_position(position & p) {
  p.x = rand() % TAILLE_TERRAIN;
  p.y = rand() % TAILLE_TERRAIN;
  if (case_occupee(p)){
    random_position(p);
  }
}
void GameData::random_piece(piece & p) {
  random_position(p.pos_piece);
  p.valeur = 1 + (rand() % 9);
  p.tour_apparition = get_real_turn() + NB_PIECES_SUIVANTES_VISIBLES;
}

int GameData::get_current_player(){
  return current_player;
}

int GameData::get_real_turn()
{
  return (getCurrentTurn() - 1) / 2 + 1;
}
void GameData::team_switched(){
  current_player = (current_player + 1 ) % 2;
  can_move = true;
  if (getCurrentTurn() % 2 == 0) apparition_piece(); // TODO verifier que ca doit-etre ici ??
}

void GameData::apparition_piece(){
  piece p = next[next_indice0];
  map_p[p.pos_piece] = p;
}

bool GameData::match_finished(){
  return player_end_match || ( get_real_turn() >= MAX_TURN);
}

int GameData::score(int team){
  if (team != 0 && team != 1){
    return 0;
  }
  LOG2("score[%1]= %2", team, score_team[team]);
  return score_team[team];
}

unite GameData::get_unite(position cible){
  return map_u[cible];
}

piece GameData::get_piece(position cible){
  return map_p[cible];
}

void GameData::set_unite(position cible, unite u){
  LOG2("case (%1. %2) adding unit %3", cible.x, cible.y, u.id);
  u.pos_unite = cible;
  map_u[cible] = u;
}

void GameData::remove_unite(position cible){
  LOG2("case (%1, %2) removing unit...", cible.x, cible.y);
  map_u.erase(cible);
}

bool GameData::contains_unite(position cible){
  int i = map_u.count(cible);
  LOG2("case (%1, %2) contains %3 unit", cible.x, cible.y, i);
  return i > 0;
}

void GameData::remove_piece(position cible){
  LOG2("case (%1, %2) removing piece at...", cible.x, cible.y);
  map_p.erase(cible);
}

bool GameData::contains_piece(position cible){
  int i = map_p.count(cible);
  LOG2("case (%1. %2) contains %3 piece", cible.x, cible.y, i);
  return i > 0;
}

void GameData::resoudreFinPartie(){
  score_team[current_player] -= PRIX_FIN_PARTIE;
  player_end_match = true;
}

void GameData::resoudreDeplacer(position cible, position pos){
  int d = distance(cible, pos);
  unite u = map_u[cible];
  u.pa -= d;
  if (contains_piece(pos)){
    piece p = map_p[pos];
    score_team[u.team] += p.valeur;
    remove_piece(pos);
  }
  set_unite(pos, u);
  remove_unite(cible);
}
void GameData::resoudre( const e_com_type type, const int * arg){
  position p1, p2;
  switch (type){
  case DEPLACER_MSG:
    if (mon_tour())  return;
    p1.x = arg[1];
    p1.y = arg[2];
    p2.x = arg[3];
    p2.y = arg[4];
    resoudreDeplacer(p1, p2);
    break;
  case FINIR_MSG:
    if (mon_tour())  return;
    resoudreFinPartie();
    break;
  case NEXT_PIECE_MSG:
    LOG2("RECIEVE PIECES MSG !!");
    p1.x = arg[2];
    p1.y = arg[3];
    push_piece(arg[0], arg[1], p1);
    break;
  }
}

