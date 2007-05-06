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

#include "GameData.hh"
#include "ClientEntry.hh"

ClientEntry::ClientEntry(GameData* game, ClientDiffer* diff, Client* client):
  StechecClientEntry(game, diff, client)
{
  c_->InitData();
}

/*!
** fonction qui permet de recevoir la map, ou/et de faire
** des initialiation sur les joueurs avant le debut de la partie
** voir const StechecPkt et tools/module_reseau/ pour les envois
** voir server.h pour la structure game
*/
int        ClientEntry::beforeGame()
{
  StechecPkt com;
  c_->InitData();
  while (fetchCommand(&com))
    differ_->ApplyDiff(&com);
  g_->InitMap();
  g_->init ();
  return 0;
}

/*!
** fonction situee apres l'initialisation du joueur,
** mais avant la boucle de jeu
** Sert a faire des eventuels envois
*/

int        ClientEntry::initGame()
{
  c_->SetCompetence (true);
  return 0;
}

/*!
** permet d'effectuer diverses operations avant
** chaque tour de jeu
*/

int        ClientEntry::beforeNewTurn()
{
  LOG1("Random is now : %1", g_->rand ());
  for (int i = 0; i < g_->getNbPlayer (); ++i)
    {
      if (g_->players[i].getState () == STATE_NORMAL)
	g_->players[i].action = false;
      else
	g_->players[i].action = true;
      g_->players[i].ResetSentMessages ();
    }
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

  g_->PlayTurn ();
  g_->player_turn++;
  return 0;
}

/*!
** permet d'effectuer diverses operations a la fin
** du jeu
*/
int        ClientEntry::afterGame()
{
  // ok quand fog ok c_->FreeData();
  g_->end ();
  g_->FreeData();

  return 0;
}
