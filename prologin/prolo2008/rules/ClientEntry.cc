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

ClientEntry::ClientEntry(GameData* game, ClientDiffer* diff, Client* client)
  : StechecClientEntry(game, diff, client)
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
