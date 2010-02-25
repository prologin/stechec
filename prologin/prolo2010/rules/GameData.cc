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

  // TODO
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
