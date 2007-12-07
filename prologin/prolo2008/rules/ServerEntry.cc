/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2007 Prologin
*/

#include <time.h>
#include "GameData.hh"
#include "ServerEntry.hh"
#include "misc/Conf.hh"

ServerEntry::ServerEntry(GameData* game, Server* server,
                         const ConfSection* cfg)
  : StechecServerEntry(game, server, cfg)
{
}

ServerEntry::~ServerEntry()
{
}


int ServerEntry::loadMap()
{
  /* load or generate map */
  return 0;
}


int		ServerEntry::beforeGame()
{
  if (loadMap())
    return 1;

  return 0;
}

int         ServerEntry::initGame()
{
  return 0;
}

int         ServerEntry::beforeNewTurn()
{
  return 0;
}

int         ServerEntry::afterNewTurn()
{
  return 0;
}


int         ServerEntry::afterGame()
{
  return 0;
}

bool        ServerEntry::isMatchFinished()
{
  return false;
}

int        ServerEntry::getScore(int uid)
{
  return -42;
}
