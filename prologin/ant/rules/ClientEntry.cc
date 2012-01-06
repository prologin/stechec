/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2004, 2005, 2006 Prologin
*/


#include "Client.hh"
#include "ClientDiffer.hh"
#include "ClientEntry.hh"


ClientEntry::ClientEntry(GameData* game, ClientDiffer* diff, Client* client):
  StechecClientEntry(game, diff, client)
{
}

ClientEntry::~ClientEntry()
{
  delete[] g_->player;

  for (int i = 0; i < g_->getNbPlayer(); i++)
    {
      for (int j = 0; j < g_->map_size_x; j++)
        free(g_->player_know_map[i][j]);
      free(g_->player_know_map[i]);
    }
  free(g_->player_know_map);
} 

/*!
** fonction qui permet de recevoir la map, ou/et de faire
** des initialiation sur les joueurs avant le debut de la partie
** voir struct s_com et tools/module_reseau/ pour les envois
** voir server.h pour la structure game
*/
int        ClientEntry::beforeGame()
{
  StechecPkt data;

  fetchCommand(&data);
  g_->map_size_x = data.arg[0];
  g_->map_size_y = data.arg[1];
  
  // fabecc: interface with visio
  //g_->player = (player_t*) calloc(sizeof (player_t), g_->getNbPlayer());
  g_->player = new player_t[g_->getNbPlayer()];
  for (int i = 0; i < g_->getNbPlayer(); i++)
    g_->player[i].id = i;
  g_->InitMap();

  // recupere les fourmilieres
  for (int i = 0; i < g_->getNbPlayer(); i++)
    {
      fetchCommand(&data);
      differ_->ApplyDiff(&data);
    }

  g_->InitMapMisc();

  g_->player_know_map = (bool ***) malloc(sizeof (bool**) * g_->getNbPlayer());
  for (int i = 0; i < g_->getNbPlayer(); i++)
    {
      g_->player_know_map[i] = (bool **) malloc(sizeof (bool*) * g_->map_size_x);
      for (int j = 0; j < g_->map_size_x; j++)
        g_->player_know_map[i][j] = (bool *) calloc(sizeof (bool), g_->map_size_y);
    }

  // Initialize g_->p, a shorcut used in the API/differ to get the
  // current player.
  g_->p = g_->getUid() >= 0 ? &g_->player[g_->getUid()] : NULL;
  
  // Recupere les donnees initiale de la map, et des joueurs
  while (fetchCommand(&data))
    differ_->ApplyDiff(&data);
  
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
  return 0;
}

/*!
** permet d'effectuer diverses operations apres
** chaque tour de jeu
*/
int        ClientEntry::afterNewTurn()
{
  return 0;
}

/*!
** permet d'effectuer diverses operations a la fin
** du jeu
*/
int        ClientEntry::afterGame()
{
  return 0;
}
