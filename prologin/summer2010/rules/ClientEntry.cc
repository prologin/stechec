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
#include "ClientEntry.hh"

ClientEntry::ClientEntry(GameData* game, ClientDiffer* diff, Client* client)
  : StechecClientEntry(game, diff, client)
{
}

/*!
** Fonction qui permet de recevoir la map, ou/et de faire
** des initialiations sur les joueurs avant le début de la partie.
** voir StechecPkt et stechec/tools/datatfs/ pour les envois
*/
int        ClientEntry::beforeGame()
{
  StechecPkt com;
  while (fetchCommand(&com))
    differ_->ApplyDiff(&com);
  assert (g_->first_indice0 == g_->next_indice0);
  return 0;
}

/*!
** Fonction située apres l'initialisation du joueur,
** mais avant la boucle de jeu
** Sert a faire d'eventuels envois
*/

int        ClientEntry::initGame()
{
  // TODO
  return 0;
}

/*!
** Permet d'effectuer diverses operations avant
** chaque tour de jeu
*/

int        ClientEntry::beforeNewTurn()
{
  g_->team_switched(false);
  return 0;
}

/*!
** Permet d'effectuer diverses operations apres
** chaque tour de jeu
*/
int        ClientEntry::afterNewTurn()
{
  // TODO
  return 0;
}

/*!
** Permet d'effectuer diverses operations a la fin
** du jeu
*/
int        ClientEntry::afterGame()
{
  // TODO
  return 0;
}
