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

#include <vector>
#include "ServerResolver.hh"

void	ServerResolver::ApplyResolver(CommandListRef cmdList[])
{
  CommandListRef::iterator it;

  char	msg[MAX_TEAM * MAX_WHITE_CELL] = { 0 };
  int	nb = 0;

  // Qui a envoye un message ?
  for (int i = 0; i < LAST_MSG; ++i)
    // Pour la phagocytose, autre type de resolution de conflits
    if (i != PHAGOCYTE && i != TRANSMISSION && i != COMPETENCE)
      for (it = cmdList[i].begin(); it != cmdList[i].end(); ++it)
	{
	  LOG1("*********** Message nb %1 *********", i);
	  if (msg[(*it)->arg[0]] == 0)
	    {
	      LOG1("*********** id %1 *********", (*it)->arg[0]);
	      nb++;
	      msg[(*it)->arg[0]] = 1;
	    }
	}
  // Renversement du tableau :
  //  id 0 1 2 3    ==>   nb 1 2
  // msg 0 1 0 1          id 1 3

  std::vector<char>	leuco;
  for (int i = 0; i < MAX_TEAM * MAX_WHITE_CELL; ++i)
    if (msg[i] == 1)
      leuco.push_back(i);

  // Choix aleatoire de qui va faire ses actions
  for (int i = nb; i > 0; --i)
    {
      int it = rand() % i;
      makeActions(leuco[it], cmdList);
      int tmp;
      std::vector<char>::iterator j;
      for (j = leuco.begin(), tmp = 0;
	   tmp < it && j != leuco.end(); j++, tmp++)
	;
      leuco.erase(j);
    }

  // Gestion de la phagocytose
  Phagocytose(cmdList[PHAGOCYTE]);
  // Gestion de la transimission
  Messages(cmdList[TRANSMISSION]);
  // Gestion de la competence
  Competences(cmdList[COMPETENCE]);
  // Gestion des anticorps
  Antibody (cmdList[DROP_ANTIBODY]);
}

void	ServerResolver::makeActions(int id, CommandListRef cmdList[])
{
  CommandListRef::iterator it;

  for (int i = 0; i < LAST_MSG; ++i)
    for (it = cmdList[i].begin(); it != cmdList[i].end(); ++it)
      if ((*it)->arg[0] == id)
	{
	  switch (i)
	    {
	    case MOVE_LEUCO:
	      {
		moveLeucocyte(*it);
		break;
	      }
	    }
	}
}


void	ServerResolver::moveLeucocyte(const StechecPkt* elt)
{
  // void	ServerResolver::moveLeucocyte(CommandListRef& cmdList)
  // {
  //   **************** OLD ******************
  //   const	StechecPkt* elt;
  //   int	id, y, x;
  //   char	resolv[MAX_MAP_SIZE][MAX_MAP_SIZE][MAX_TEAM * MAX_WHITE_CELL] = { 0 };
  //   char	leuco_mov[MAX_TEAM * MAX_WHITE_CELL] = { 0 };

  //   for (CommandListRef::iterator it = cmdList.begin();
  //        it != cmdList.end(); ++it)
  //     {
  //       elt = *it;
  //       id = elt->arg[0];
  //       y = elt->arg[1];
  //       x = elt->arg[2];
  //       // Verification virus : si il y a un virus, bouge pas
  //       if (g_->terrain_type[y][x] != VIRUS)
  // 	resolv[y][x][id]++;
  //       else
  // 	resolv[g_->players[id].row][g_->players[id].col][id]++;
  //       leuco_mov[id]++;
  //     }
  //   // Ajout de ceux qui ont pas demande a bouger
  //   for (int i = 0; i < MAX_TEAM * MAX_WHITE_CELL; ++i)
  //     if (leuco_mov[i] == 0)
  //       resolv[g_->players[i].row][g_->players[i].col][i]++;

  int id = elt->arg[0];
  int y = elt->arg[1];
  int x = elt->arg[2];


  if (g_->TestVirus(y, x)// FIXME TEST LEUCOCYTE
      )
    {
      g_->players[id].row = y;
      g_->players[id].col = x;
      //      g_->terrain_type[y][x] = WHITE_CELL;
      LOG1("Accept MOVE: Get arg x: %1, y:%2, id: %3", x, y, id);
      // Si mouvement valide, on envoit ca a tout le monde
      SendToAll(*elt);
    }
}


void	ServerResolver::Antibody (CommandListRef& cmdList)
{
  CommandListRef::iterator it;
  StechecPkt* elt;
  for (it = cmdList.begin(); it != cmdList.end(); ++it)
    {
      elt = *it;
      Leucocyte cur = g_->players[elt->arg[0]];
      cur.addAntibody ();
      SendToAll(*elt);
    }
}

void	ServerResolver::Competences(CommandListRef& cmdList)
{
  CommandListRef::iterator it;
  StechecPkt* elt;
  for (it = cmdList.begin(); it != cmdList.end(); ++it)
    {
      elt = *it;
      g_->players[elt->arg[0]].competences[PHAGO_SPEED] =
	elt->arg[1];
      g_->players[elt->arg[0]].competences[ANTIBODY_NB] =
	elt->arg[2];
      g_->players[elt->arg[0]].competences[MESSAGES_NB] =
	elt->arg[3];
      g_->players[elt->arg[0]].competences[VISION] =
	elt->arg[4];
    }
}

void	ServerResolver::Phagocytose(CommandListRef& cmdList)
{

}

void	ServerResolver::Messages(CommandListRef& cmdList)
{
  CommandListRef::iterator it;

  for (it = cmdList.begin(); it != cmdList.end(); ++it)
    {
      LOG1("Send Message ...");
      SendToAll(**it);
    }
}
