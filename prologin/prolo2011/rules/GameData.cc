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

#include "Utils.hh"
#include "GameData.hh"
#include "Constant.hh"
#include <ctime>
#include <cstdlib>

// TODO: better, call Init() from BeforeNewGame, etc..
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

source_energie  SourceEnergie::to_source_energie(int indice)
{
    source_energie  s;
    s.id = indice;
    s.pos = pos;
    s.coef = potentiel_cur;
    return (s);
}

void SourceEnergie::set_potentiel(int potentiel)
{
    potentiel_max = potentiel;
    potentiel_cur = potentiel;
}

GameData::GameData()
    : current_player(0),
      can_play(true)
{
    int		taille = TAILLE_TERRAIN * TAILLE_TERRAIN;
    Case	default_case;
    Joueur	basic_joueur;

    terrain_.reserve(taille);
    for (int i = 0; i < taille; ++i)
	terrain_.push_back(default_case);

    // FIXME: use a constant (made the assumption that there will always be 2
    // players
    joueurs.reserve(2);
    for (int i = 0; i < 2; ++i)
	joueurs.push_back(basic_joueur);
}

void GameData::Init() {
  LOG2("GameData::Init");
  initialized_ = true;
}

Case&
GameData::get_case(int x, int y)
{
  if (position_invalide(x, y)) abort();
    return terrain_[x + TAILLE_TERRAIN * y];
}

Case&
GameData::get_case(const position &p)
{
  return get_case(p.x, p.y);
}

int GameData::get_free_moto_id()
{
    // Even if it is inefficient, it should work while there’s no more than
    // 2^(sizeof (int) * 8) motos. :-)

    int result = 0;
    while (moto_valide(result))
	++result;
    return result;
}

InternalTraineeMoto&
GameData::creer_trainee_moto(int player, position init, int max_len)
{
    int	id = get_free_moto_id();
    InternalTraineeMoto moto(this, player, id, init, max_len);
    motos.insert(std::pair<int, InternalTraineeMoto>(id, moto));
    return motos[id];
}

bool GameData::moto_valide(int id)
{
    return (motos.find(id) != motos.end());
}

void GameData::supprimer_moto(int id)
{
    motos.erase(id);
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
