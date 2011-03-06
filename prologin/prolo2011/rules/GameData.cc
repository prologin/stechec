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
#include <ctime>
#include <cstdlib>

//todo, better, call Init() from BeforeNewGame, etc..
#define INIT()					\
  assert(initialized_);

GameData::GameData()
{
  srand(time(0));
  current_player = 1;
}

void GameData::Init() {
  // TODO
  initialized_ = true;
}

int GameData::mon_equipe(){
  LOG2("mon_equipe %1", getTeamId());
  return getTeamId();
}
bool GameData::mon_tour()
{
  unsigned int curr_player = (getCurrentTurn() - 1) % 2;
  LOG2("curr_player = %1, team id = %2", curr_player, getTeamId());
  return curr_player == getTeamId();
}
int GameData::get_current_player(){
  return current_player;
}

int GameData::get_real_turn()
{
  return (getCurrentTurn() - 1) / 2 + 1;
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
