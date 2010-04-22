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
#include "ServerEntry.hh"
#include "misc/Conf.hh"

ServerEntry::ServerEntry(GameData* game, Server* server,
                         const ConfSection* cfg)
  : StechecServerEntry(game, server, cfg)
{
}

ServerEntry::~ServerEntry(void)
{
}


int		ServerEntry::beforeGame(void)
{

  g_->Init();
  
  return 0;
}

int         ServerEntry::initGame(void)
{
  return 0;
}

int         ServerEntry::beforeNewTurn(void)
{
  return 0;
}

int         ServerEntry::afterNewTurn(void)
{
  g_->team_switched();
  return 0;
}


int         ServerEntry::afterGame(void)
{
  return 0;
}

bool        ServerEntry::isMatchFinished(void)
{
  return false;
}

int ServerEntry::getScore(int uid)
{
  return 0;
}
