/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) !!year!! Prologin
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
