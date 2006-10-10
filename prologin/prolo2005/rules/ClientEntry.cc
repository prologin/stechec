/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#include "GameData.hh"
#include "ClientEntry.hh"

ClientEntry::ClientEntry(GameData* game, ClientDiffer* diff, Client* client):
  StechecClientEntry(game, diff, client)
{
}

/*!
** fonction qui permet de recevoir la map, ou/et de faire
** des initialiation sur les joueurs avant le debut de la partie
** voir const StechecPkt et tools/module_reseau/ pour les envois
** voir server.h pour la structure game
*/
int        ClientEntry::beforeGame()
{
  g_->players = new Player[MAX_PLAYER];

  StechecPkt com;
  if (!fetchCommand(&com))
    {
      ERR("Command list is empty");
      return 1;
    }
  g_->maxTurn = com.arg[0];
  g_->map_size_x = com.arg[1];
  g_->map_size_y = com.arg[2];
  LOG3("ClientEntry::beforeGame: maxTurn = %1, map_x %2, map_y %3",
       g_->maxTurn, g_->map_size_x, g_->map_size_y);

  // alloue la map
  c_->InitData();
  g_->InitMap();

  // Initialize g_->p, a shorcut used in the API/differ to get the
  // current player.
  g_->p = g_->getUid() >= 0 ? &g_->players[g_->getUid()] : NULL;

  while (fetchCommand(&com))
    differ_->ApplyDiff(&com);

  return 0;
}

/*!
** fonction situee apres l'initialisation du joueur,
** mais avant la boucle de jeu
** Sert a faire des eventuels envois
*/
int        ClientEntry::initGame()
{
  return 0;
}

/*!
** permet d'effectuer diverses operations avant
** chaque tour de jeu
*/
int        ClientEntry::beforeNewTurn()
{
  for (int i = 0; i < g_->getNbPlayer(); i++)
    for (int j = 0; j < MAX_COLEOPTERE; j++)
      g_->players[i].coleopteres[j].action = false;

  return 0;
}

/*!
** permet d'effectuer diverses operations apres
** chaque tour de jeu
*/
int        ClientEntry::afterNewTurn()
{
  StechecPkt com;
  while (fetchCommand(&com))
    differ_->ApplyDiff(&com);
  return 0;
}

/*!
** permet d'effectuer diverses operations a la fin
** du jeu
*/
int        ClientEntry::afterGame()
{
  c_->FreeData();
  g_->FreeData();

  return 0;
}
