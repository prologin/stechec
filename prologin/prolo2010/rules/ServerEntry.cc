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
  g_->team_switched();
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
  if (
      g_->get_real_turn() % TEMPS_RETRECISSEMENT == 0 &&
      g_->get_real_turn() != 0 &&
      g_->getCurrentTurn() % 2 == 0
      )
    {
      g_->retrecissement();
      StechecPkt com(RETRECIR, -1);
      SendToAll(com);
    }
  return 0;
}


int         ServerEntry::afterGame(void)
{
  return 0;
}

bool        ServerEntry::isMatchFinished(void)
{
  bool titi[2] = {false, false};
  for (std::vector<unite>::iterator it = g_->unites.begin();
       it != g_->unites.end(); ++it)
  {
    if (it->ko == -1 && it->vrai_type_unite == PERROQUET)
    {
      titi[it->ennemi] = true;
    }
  }
  if (titi[0]){
    LOG3("isMatchFinished: je suis vivant");
  }
  if (titi[1]){
    LOG3("isMatchFinished: il est vivant");
  }
  return ! (titi[0] && titi[1]);
}

int ServerEntry::getScore(int uid)
{
  bool moi_fail = false, ennemi_fail = false;
  int mon_uid = g_->get_current_player();

  for (std::vector<unite>::iterator it = g_->unites.begin();
       it != g_->unites.end(); ++it)
    if (it->ko == -1 && it->vrai_type_unite == PERROQUET)
      if (uid != mon_uid)
        if (!it->ennemi)
          moi_fail = true;
        else
          ennemi_fail = true;
      else
        if (it->ennemi)
          moi_fail = true;
        else
          ennemi_fail = true;

  if (!moi_fail && !ennemi_fail)
    return 0;

  if (!moi_fail)
    return 0;

  return 1; //otherwise ennemi_fail
}
