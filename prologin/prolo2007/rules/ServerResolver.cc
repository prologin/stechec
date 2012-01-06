/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2007 Prologin
*/

#include <vector>
#include "ServerResolver.hh"

void	ServerResolver::ApplyResolver(CommandListRef cmdList[])
{
  // Gere les messages speciaux a part.
  Competences   (cmdList[COMPETENCE]);
  moveLeucocyte (cmdList[MOVE_LEUCO]);
  Phagocytose   (cmdList[PHAGOCYTE]);
  Messages      (cmdList[TRANSMISSION]);
  Antibody      (cmdList[DROP_ANTIBODY]);
}

bool	ServerResolver::GonnaBeBlocked(int id, int y, int x, CommandListRef& cmdList)
{
  CommandListRef::iterator it;
  int found = -1;
  if (g_->TestLeucocyte (y, x))
    return false;
  for (it = cmdList.begin(); it != cmdList.end() && found == -1; ++it)
    {
      if (g_->players[(*it)->client_id].row == y &&
	  g_->players[(*it)->client_id].col == x &&
	  g_->players[id].row == (*it)->arg[0] &&
	  g_->players[id].col == (*it)->arg[1] &&
	  // 	  && !GonnaBeBlocked((*it)->arg[0], (*it)->arg[1], cmdList)
	  g_->TestCell((*it)->arg[0], (*it)->arg[1]) &&
	  g_->TestVirus((*it)->arg[0], (*it)->arg[1]))
	return false;
    }
  return true;
}

void	ServerResolver::moveLeucocyte(CommandListRef& cmdList)
{
  CommandListRef::iterator it;
  StechecPkt* elt;
  bool  *validated = new bool[cmdList.size ()];

  for (int i = 0; i < cmdList.size (); ++i)
    validated[i] = true;

  int i = 0;
  for (it = cmdList.begin(); it != cmdList.end(); ++it, ++i)
    {
      elt = *it;
      int id = elt->client_id;
      int y = elt->arg[0];
      int x = elt->arg[1];

      if (g_->TestVirus(y, x)// FIXME TEST LEUCOCYTE
	  )
	{
	  validate (validated, cmdList, i);
	  if (!validated[i])
	    continue;
	  if (GonnaBeBlocked (id, y, x, cmdList))
	    continue;
	  g_->players[id].row = y;
	  g_->players[id].col = x;
	  //      g_->terrain_type[y][x] = WHITE_CELL;
	  LOG4("Accept MOVE: Get arg x: %1, y:%2, id: %3", x, y, id);
	  // Si mouvement valide, on envoit ca a tout le monde
	  SendToAll(*elt);
	}
    }
}


void	ServerResolver::Antibody (CommandListRef& cmdList)
{
  CommandListRef::iterator it;
  StechecPkt* elt;
  for (it = cmdList.begin(); it != cmdList.end(); ++it)
    {
      elt = *it;
      Leucocyte* cur = &g_->players[elt->client_id];
      LOG4("Leucocyte %1 drops antibodies", elt->client_id);
      cur->addAntibody();
      SendToAll(*elt);
    }
}

void	ServerResolver::Competences(CommandListRef& cmdList)
{
  CommandListRef::iterator it;
  for (it = cmdList.begin(); it != cmdList.end(); ++it)
    {
      StechecPkt* elt = *it;

      g_->players[elt->client_id].competences[PHAGO_SPEED] = elt->arg[0];
      g_->players[elt->client_id].competences[ANTIBODY_NB] = elt->arg[1];
      g_->players[elt->client_id].competences[MESSAGES_NB] = elt->arg[2];
      g_->players[elt->client_id].competences[VISION] = elt->arg[3];
      SendToAll(*elt);
    }
}

void	ServerResolver::validatePh (bool*  validated,
				  CommandListRef& cmdList, int index)
{
  int x = g_->players[cmdList[index]->client_id].col;
  int y = g_->players[cmdList[index]->client_id].row;
  int tox = cmdList[index]->arg[1];
  int toy = cmdList[index]->arg[0];
  for (int i = 0; i < cmdList.size (); ++i)
    {
      if (i != index)
	{
	  if (cmdList[i]->arg[0] == y &&
	      cmdList[i]->arg[1] == x &&
	      tox == g_->players[cmdList[i]->client_id].col &&
	      toy == g_->players[cmdList[i]->client_id].row)
	    {
	      validated[i] = false;
	      validated[index] = false;
	      LOG4("Action %1 %2 cancelled...", i, index);
    }
	}
    }
}

void	ServerResolver::validate (bool*  validated,
				  CommandListRef& cmdList, int index)
{
  int x = cmdList[index]->arg[1];
  int y = cmdList[index]->arg[0];

  for (int i = 0; i < cmdList.size (); ++i)
    {
      LOG4("Validating %1 with %2 : [%3,%4]<>[%5,%6]", i, index,
	   y, x, cmdList[i]->arg[0],cmdList[i]->arg[1]);
      if (i != index)
	{
	  if (cmdList[i]->arg[0] == y &&
	      cmdList[i]->arg[1] == x)
	    {
	      validated[i] = false;
	      validated[index] = false;
	      LOG4("Action %1 %2 cancelled...", i, index);
	    }
	}
    }
}

void	ServerResolver::Phagocytose(CommandListRef& cmdList)
{
  CommandListRef::iterator it;
  bool  *validated = new bool[cmdList.size ()];

  for (int i = 0; i < cmdList.size (); ++i)
    validated[i] = true;

  int i = 0;
  for (it = cmdList.begin(); it != cmdList.end(); ++it, ++i)
    {
      StechecPkt* elt = *it;
      LOG3("Phagocytose requested by %1 to [%2,%3]",
	  elt->client_id, elt->arg[0], elt->arg[1]);
      if (validated[i])
	{
	  validatePh (validated, cmdList, i);
	  if (!validated[i])
	    continue;
	  LOG3("Phagocytose accepted by %1 to [%2,%3]",
	       elt->client_id, elt->arg[0], elt->arg[1]);
	  g_->Phagocytose (elt->client_id, elt->arg[0], elt->arg[1]);
	  SendToAll(**it);
	}
    }
  delete[] validated;
}

void	ServerResolver::Messages(CommandListRef& cmdList)
{
  CommandListRef::iterator it;

  for (it = cmdList.begin(); it != cmdList.end(); ++it)
    {
      LOG5("Send Message ...");
      SendToAll(**it);
    }
}
