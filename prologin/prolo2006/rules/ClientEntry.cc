/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#include "GameData.hh"
#include "Astar.hh"
#include "ClientEntry.hh"

ClientEntry::ClientEntry(GameData* game, ClientDiffer* diff, Client* client):
  StechecClientEntry(game, diff, client)
{
  c_->InitData();
}

int	ClientEntry::visible_test(Position ref, Position pos)
{
   float	sqr;

   float	row;
   float	col;

   float	row1;
   float	col1;

   sqr = sqrt(((pos.row - ref.row) * (pos.row - ref.row))
	      + ((pos.col - ref.col) * (pos.col - ref.col))) * 1.3;
   if (sqr == 0)
      return 0;
   row = (float)(pos.row - ref.row) / sqr;
   col = (float)(pos.col - ref.col) / sqr;
   row1 = ref.row + 0.5;
   col1 = ref.col + 0.5;
   while ((row < 0 ? (int)row1 >= pos.row : (int)row1 <= pos.row)
	  && (col < 0 ? (int)col1 >= pos.col : (int)col1 <= pos.col))
   {
     g_->visibility[ref.row][ref.col][(int)row1][(int)col1] = 1;
      if (IS_OPAQUE((int)row1, (int)col1))
	 break;
      row1 += row;
      col1 += col;
   }
   return 0;
}

int	ClientEntry::visible_from(int from_x, int from_y)
{
  Position	pos;
  Position	from(from_y, from_x);

  for (pos.row = 0, pos.col = 0; pos.col < g_->map_size.col; pos.col++)
    if (visible_test(from, pos))
      return 1;
  for (pos.row = g_->map_size.row - 1, pos.col = 0;
       pos.col < g_->map_size.col; pos.col++)
	  if (visible_test(from, pos))
	    return 1;
  for (pos.row = 0, pos.col = 0; pos.row < g_->map_size.row; pos.row++)
    if (visible_test(from, pos))
      return 1;
  for (pos.row = 0, pos.col = g_->map_size.col - 1;
	pos.row < g_->map_size.row; pos.row++)
    if (visible_test(from, pos))
      return 1;
  return 0;
}

/*!
** fonction qui permet de recevoir la map, ou/et de faire
** des initialiation sur les joueurs avant le debut de la partie
** voir const StechecPkt et tools/module_reseau/ pour les envois
** voir server.h pour la structure game
*/
int        ClientEntry::beforeGame()
{
  //  g_->players = new Player[MAX_TEAM];

   StechecPkt com;

//   if (!fetchCommand(&com))
//     {
//       ERR("Command list is empty");
//       return 1;
//     }
//   g_->maxTurn = com.arg[0];
//   g_->map_size.row = com.arg[1];
//   g_->map_size.col = com.arg[2];
//   LOG3("ClientEntry::beforeGame: maxTurn = " << g_->maxTurn
//        << ", map_x " << g_->map_size.row
//        << ", map_y " << g_->map_size.col);

  // alloue la map


  if (g_->getUid() >= 0)
    g_->p = &g_->players[g_->getUid()];
  else
    g_->p = NULL;

  c_->InitData();
  while (fetchCommand(&com))
    differ_->ApplyDiff(&com);
  g_->InitMap();

  return 0;
}

/*!
** fonction situee apres l'initialisation du joueur,
** mais avant la boucle de jeu
** Sert a faire des eventuels envois
*/

int        ClientEntry::initGame()
{
  memset(g_->visibility, 0, sizeof (char) * MAX_MAP_SIZE * MAX_MAP_SIZE* MAX_MAP_SIZE * MAX_MAP_SIZE);
   for (int row = 0; row < g_->map_size.row; ++row)
     for (int col = 0; col < g_->map_size.col; ++col)
       {
	 visible_from(col, row);
       }
  return 0;
}

/*!
** permet d'effectuer diverses operations avant
** chaque tour de jeu
*/

int        ClientEntry::beforeNewTurn()
{
  for (int i = 0; i < g_->getNbPlayer(); i++)
    for (int j = 0; j < MAX_GOODMEN; j++)
      g_->players[i].team[j].setAction(false);

  return 0;
}

/*!
** permet d'effectuer diverses operations apres
** chaque tour de jeu
*/
int        ClientEntry::afterNewTurn()
{
  std::vector<Bet*>::iterator	i = g_->almanach.getAlmanach().begin();
  Bet*				bet;
  StechecPkt com;

  while (fetchCommand(&com))
    differ_->ApplyDiff(&com);

   for (; i != g_->almanach.getAlmanach().end(); ++i)
     {
       bet = *i;
       if (g_->player_turn >= bet->getDate()
	   && g_->player_turn <= (bet->getDate() + BET_LENGTH))
	 {
	   bet->setOpen(true);
	   bet->setTimeLeft(BET_LENGTH + (bet->getDate() - g_->player_turn));
	   if (g_->player_turn == bet->getDate())
	     {
	       LOG3("Opening bet: %1 from casino: %2", bet->getId(), bet->getCasinoId());
	     }
	   LOG3("Bet time left for bet: %1 is now %2",
		bet->getId(), BET_LENGTH + (bet->getDate() - g_->player_turn));
	 }
     }
  return 0;
}

/*!
** permet d'effectuer diverses operations a la fin
** du jeu
*/
int        ClientEntry::afterGame()
{
  // ok quand fog ok c_->FreeData();
  g_->FreeData();

  return 0;
}
