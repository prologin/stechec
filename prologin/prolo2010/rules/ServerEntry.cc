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

#define ARG2(a, i) a[(i)], a[(i) + 1]
#define ARG4(a, i) ARG2(a, i), ARG2(a, i + 2)
#define ARG8(a, i) ARG4(a, i), ARG4(a, i + 4)
int         ServerEntry::afterNewTurn(void)
{
  // forward the actions
  for (std::vector<std::vector<int> >::iterator it = g_->packets.begin();
       it != g_->packets.end(); ++it)
  {
    SendToAll((*it)[0], -1, 8, ARG8((*it), 1));
  }
  g_->team_switched();
  return 0;
}


int         ServerEntry::afterGame(void)
{
  return 0;
}

bool        ServerEntry::isMatchFinished(void)
{
  for (std::vector<unite>::iterator it = g_->unites.begin();
       it != g_->unites.end(); ++it)
  {
    if (it->ko >= 0 && it->vrai_type_unite == PERROQUET)
    {
      return true;
    }
  }

  return false;
}

int ServerEntry::getScore(int uid)
{
  bool moi_fail = false, ennemi_fail = false;
  int mon_uid = g_->get_current_player();

  for (std::vector<unite>::iterator it = g_->unites.begin();
       it != g_->unites.end(); ++it)
  {
    if (it->ko >= 0 && it->vrai_type_unite == PERROQUET)
    {
      if (it->ennemi)
        ennemi_fail = true;
      else
        moi_fail = true;
    }
  }

  if (moi_fail && ennemi_fail)
    return 0;

  if (uid == mon_uid && moi_fail)
    return 0;

  if (uid != mon_uid && ennemi_fail)
    return 0;

  return 1;
}
