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

int max(int a, int b){
  return a>b?a:b;
}

int min(int a, int b){
  return a>b?a:b;
}

int distance(position p1, position p2){
  return max(abs(p1.x - p2.x), abs(p1.y - p2.y));
}

//todo, better, call Init() from BeforeNewGame, etc..
#define INIT()					\
  assert(initialized_);

GameData::GameData()
{
  srand(time(0));
  next_indice0 = 0;
  first_indice0 = 0;
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
piece GameData::push_random_piece(int tour_apparition) {
  piece p;
  random_piece(p, tour_apparition);
  push_piece(tour_apparition, p.valeur, p.pos_piece);
  return p;
}
void GameData::push_piece(int temps, int valeur, position p) {
  LOG3("push piece temps : %1 valeur %2 indice = %3", temps, valeur, next_indice0);
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
void GameData::random_piece(piece & p, int tour_apparition) {
  random_position(p.pos_piece);
  p.valeur = 1 + (rand() % 9);
  p.tour_apparition = tour_apparition;
}

int GameData::get_current_player(){
  return current_player;
}

int GameData::get_real_turn()
{
  return (getCurrentTurn() - 1) / 2 + 1;
}
void GameData::team_switched(bool serv){
  current_player = (current_player + 1 ) % 2;
  can_move = true;
  // reset des KO et des PA
  std::vector<unite> out;
  std::map<position, unite>::iterator itr;
  for(itr = map_u.begin(); itr != map_u.end(); ++itr){
    unite &u = itr->second;
    if (u.team == current_player){
      if (u.ko) u.ko --;
      u.pa = MAX_PA;
    }
  }
}

void GameData::apparition_piece(){
  piece p = next[first_indice0];
  if (contains_unite(p.pos_piece)){
    score_team[map_u[p.pos_piece].team] += p.valeur;
  }else{
    map_p[p.pos_piece] = p;
  }
  first_indice0 = (first_indice0 + 1) % NB_PIECES_SUIVANTES_VISIBLES;
}

bool GameData::match_finished(){
  return player_end_match || ( getCurrentTurn() >= MAX_TURN * 2);
}

int GameData::score(int team){
  if (team != 0 && team != 1){
    return 0;
  }
  LOG4("score[%1]= %2", team, score_team[team]);
  return score_team[team];
}

unite GameData::get_unite(position cible){
  return map_u[cible];
}

piece GameData::get_piece(position cible){
  return map_p[cible];
}

void GameData::set_unite(position cible, unite u){
  LOG4("case (%1. %2) adding unit %3", cible.x, cible.y, u.id);
  u.pos_unite = cible;
  map_u[cible] = u;
}

void GameData::remove_unite(position cible){
  LOG4("case (%1, %2) removing unit...", cible.x, cible.y);
  map_u.erase(cible);
}

bool GameData::contains_unite(position cible){
  int i = map_u.count(cible);
  LOG4("case (%1, %2) contains %3 unit", cible.x, cible.y, i);
  return i > 0;
}

void GameData::remove_piece(position cible){
  LOG4("case (%1, %2) removing piece at...", cible.x, cible.y);
  map_p.erase(cible);
}

bool GameData::contains_piece(position cible){
  int i = map_p.count(cible);
  LOG4("case (%1. %2) contains %3 piece", cible.x, cible.y, i);
  return i > 0;
}

caracteristiques_objet GameData::proprietes_objet(type_objet to){
  static caracteristiques_objet caracs[] = {
    {100, 10},
    {10, 4},
    {0, -1}
  };
  return caracs[to];
}

#define POSITION_VALIDE(p) if (p.x < 0 || p.y < 0 || p.x >= TAILLE_TERRAIN || p.y >= TAILLE_TERRAIN) throw POSITION_INVALIDE;
#define UNITE_IN(p) if (!contains_unite(p)) throw POSITION_INVALIDE;
#define PAS_UNITE_IN(p) if (contains_unite(p)) throw POSITION_INVALIDE;

void GameData::resoudreFinPartie(){
  LOG3("argent[%1] = %2, prix fin = %3", current_player, score_team[current_player], PRIX_FIN_PARTIE);
  if (score_team[current_player] < PRIX_FIN_PARTIE) throw PLUS_D_ARGENT;
  score_team[current_player] -= PRIX_FIN_PARTIE;
  player_end_match = true;
}

void GameData::resoudreDeplacer(position cible, position pos){
  UNITE_IN(cible);
  LOG3("position de depart libre");
  PAS_UNITE_IN(pos);
  LOG3("position de destination libre");
  POSITION_VALIDE(pos);
  LOG3("position de destination dans le terrain");
  int d = distance(cible, pos);
  unite u = map_u[cible];
  if (u.team != current_player) throw PAS_A_TOI;
  if (u.pa < d) throw PLUS_DE_PA;
  if (u.ko > 0) throw UNITE_KO;
  u.pa -= d;
  if (contains_piece(pos)){
    piece p = map_p[pos];
    score_team[u.team] += p.valeur;
    remove_piece(pos);
  }
  set_unite(pos, u);
  remove_unite(cible);
}

void GameData::resoudreAcheterObjet(position cible, type_objet objet){
  caracteristiques_objet o = proprietes_objet(objet);
  UNITE_IN(cible);
  unite &u = map_u[cible];
  if (u.team != current_player) throw PAS_A_TOI;
  if (u.ko > 0) throw UNITE_KO;
  if ( score_team[u.team] < o.cout) throw PLUS_D_ARGENT;
  score_team[u.team] -= o.cout;
  u.objet = objet;
}

bool GameData::filet(int x, int y){
  position p;
  p.x = x;
  p.y = y;
  if (contains_piece(p)){
    piece pi = map_p[p];
    score_team[current_player] += pi.valeur;
  }
  if (contains_piece(p)){
    unite &u = map_u[p];
    u.ko = FILET_KO;
    return true;
  }else{
    return false;
  }
}

void GameData::resoudreUtiliserObjet(position cible, position pos){
  UNITE_IN(cible);
  PAS_UNITE_IN(pos);
  POSITION_VALIDE(pos);
  unite &u = map_u[cible];
  int d = distance(cible, pos);
  int p = proprietes_objet(u.objet).porte;
  if (u.team != current_player) throw PAS_A_TOI;
  if (p < d) throw PAS_A_PORTE;
  if (u.ko > 0) throw UNITE_KO;
  switch (u.objet){
  case FILET:
    if (cible.x == pos.x){
      int y = cible.y;
      for (int x = min(cible.x, pos.x), ex = max(cible.x, pos.x); x <= ex; x ++){
	if (filet(x, y)) break;
      }
    }else if (cible.y == pos.y){
      int x = cible.x;
      for (int y = min(cible.y, pos.y), ey = max(cible.y, pos.y); y <= ey; y ++ ){
	if (filet(x, y)) break;
      }
    }else{
      throw UTILISATION_IMPOSSIBLE;
    }
    break;
  case MARTEAU:
    {
      unite &adversaire = map_u[pos];
      adversaire.ko = MARTEAU_KO;
    };
    break;
  case RIEN:
    break;
  }
  u.objet = RIEN;
}

void GameData::resoudre( const e_com_type type, const int * arg){
  position p1, p2;
  LOG3("current player = %1", current_player);
  switch (type){
  case DEPLACER_MSG:
    LOG3("resoudre deplacer msg");
    if (mon_tour())  return;
    if (!can_move) abort();
    p1.x = arg[1];
    p1.y = arg[2];
    p2.x = arg[3];
    p2.y = arg[4];
    resoudreDeplacer(p1, p2);
    break;
  case FINIR_MSG:
    LOG3("resoudre finir msg");
    if (!can_move) abort();
    if (mon_tour())  return;
    resoudreFinPartie();
    break;
  case NEXT_PIECE_MSG: // client only
    LOG3("resoudre next piece msg");
    p1.x = arg[2];
    p1.y = arg[3];
    if (can_move) apparition_piece();
    push_piece(arg[0], arg[1], p1);
    LOG2("first_indice = %1 next_indice = %2", first_indice0, next_indice0);
    break;
  case ACHETER_OBJET_MSG:
    LOG3("resoudre acheter objet msg");
    if (!can_move) abort();
    if (mon_tour())  return;
    p1.x = arg[1];
    p1.y = arg[2];
    resoudreAcheterObjet(p1, (type_objet)arg[3]);
    break;
  case UTILISER_OBJET_MSG:
    LOG3("resoudre utiliser objet msg");
    if (!can_move) abort();
    if (mon_tour())  return;
    p1.x = arg[1];
    p1.y = arg[2];
    p2.x = arg[3];
    p2.y = arg[4];
    resoudreUtiliserObjet(p1, p2);
    break;
  default:
    abort();
  }
}

