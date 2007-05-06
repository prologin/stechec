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

#include "ClientDiffer.hh"


void		ClientDiffer::ApplyDiff(const StechecPkt *com)
{
  // Get the message
  switch (com->type)
    {
      // List of messages
    case INIT_DATA:
      {
	g_->max_date = com->arg[0];
	g_->map_size.row = com->arg[1];
	g_->map_size.col = com->arg[2];
	g_->max_new_seeds = com->arg[3];
	g_->srand (com->arg[4]);
	break;
      }
    case MAP_CONTENT:
      {
	// terrain type : row, col
	g_->terrain_type[com->arg[0]][com->arg[1]] = com->arg[2];
	if (com->arg[2] == CELL)
	  //FIXME: A synchro avec ServerEntry.cc
	  g_->_cells.push_back(new Cellule(com->arg[0], com->arg[1],
					   CELL_MITOSIS_MATERIAL * 2, g_));
	break;
      }
    case NEW_VIRUS:
      {
	LOG1("New virus detected");
	g_->_virus.push_back(new Virus(com->arg[0], com->arg[1], com->arg[2]));
	break;
      }
    case NEW_CELL:
      {
	LOG1("New cell detected");
	g_->_cells.push_back(new Cellule(com->arg[0], com->arg[1], CELL_MITOSIS_MATERIAL * 2,
					 g_));
	break;
      }
    case MOVE_LEUCO:
      {
	Leucocyte& l = g_->players[com->arg[0]];
	int		row = com->arg[1];
	int		col = com->arg[2];
	Position	dep = Position(l.row, l.col);
	Position	arr = Position(row, col);
	if (g_->GetWrapUid () == com->arg[0])
	  c_->UpdateFogOfWar(com->arg[0], dep, arr, true);
	l.row = row;
	l.col = col;
	LOG1("Differ: move Leucocyte %1 from: %2 to : %3", com->arg[0],dep, arr);
	break;
      }
    case NEW_LEUCO:
      {
	Position	dep = Position(0, 0);
	// position is row col...
	Position	arr = Position(com->arg[3], com->arg[2]);


	g_->players[com->arg[1]].set_id (com->arg[1]);
	g_->players[com->arg[1]].set_player (com->arg[0]);
	g_->players[com->arg[1]].col = com->arg[2];
	g_->players[com->arg[1]].row = com->arg[3];
	g_->players[com->arg[1]].setGameData (g_);
	if (com->arg[4] >= 0)
	  g_->players[com->arg[1]].SetRealUid (com->arg[4]);
	LOG1("Received NEW LEUCOCYTE : team : %1, uid : %2, x : %3, y : %4. RealUid : %5, Getuid : %6",
	     com->arg[0], com->arg[1], com->arg[2], com->arg[3], com->arg[4], g_->getUid());
	if (g_->getUid () == com->arg[4])
	  {
	    LOG1("This is me :)))");
	    c_->UpdateFogOfWar(com->arg[1], dep, arr, false);
	  }
	if (com->arg[4] >= 0)
	  g_->uidWrapper[com->arg[4]] = com->arg[1];
	else
	  g_->uidWrapper[com->arg[4]] = com->arg[4];
	break;
      }
    case TRANSMISSION:
      {
	g_->AddMessage(com->arg[0], com->arg[1], com->arg[2], com->arg[3]);
	break;
      }
    case DROP_ANTIBODY:
      {
	g_->players[com->arg[0]].addAntibody ();
	break;
      }
    default :
      {
	LOG1("Unexpected message arrives... id : %1", com->type);
      }
    }
}
