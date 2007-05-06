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
	LOG1("New virus detected : %1 %2 %3", com->arg[0], com->arg[1], com->arg[2]);
	g_->_virus.push_back(new Virus(com->arg[0], com->arg[1], com->arg[2], g_));
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
	int player_id = com->client_id;
	int row = com->arg[0];
	int col = com->arg[1];
	Leucocyte& l = g_->players[player_id];

	Position dep = Position(l.row, l.col);
	Position arr = Position(row, col);
	LOG3("Changing fow of : %1", g_->getUid ());
	//	if (g_->getUid () == player_id)
	c_->UpdateFogOfWar(player_id, dep, arr, true);
	l.row = row;
	l.col = col;
	LOG3("Differ: move Leucocyte %1 from: %2 to : %3", player_id, dep, arr);
	break;
      }

    case NEW_LEUCO:
      {
	int player_id = com->client_id;
	int team_id = com->arg[0];

	LOG3("NEW_LEUCO: team: %1, uid: %2, x: %3, y: %4",
	     team_id, player_id, com->arg[1], com->arg[2]);

	g_->players[player_id].set_id(player_id);
	g_->players[player_id].set_player(team_id);
	g_->players[player_id].col = com->arg[1];
	g_->players[player_id].row = com->arg[2];
	//	g_->players[player_id].setGameData (g_);

	// If this is _my_ leuco
// 	if (g_->getUid() == player_id)
// 	  {
	Position dep = Position(0, 0);
	Position arr = Position(com->arg[2], com->arg[1]); // [row, col]
	LOG4("This is me :))) -> update fog of war");
	c_->UpdateFogOfWar(player_id, dep, arr, false);
	    //	  }
	break;
      }

    case TRANSMISSION:
      {
	g_->AddMessage(com->arg[0], com->arg[1], com->arg[2], com->arg[3]);
	break;
      }

    case DROP_ANTIBODY:
      {
	g_->players[com->client_id].addAntibody();
	LOG3("Dropping antibody.. %1", com->client_id);
	break;
      }
    case COMPETENCE:
      {
	g_->players[com->client_id].competences[PHAGO_SPEED] = com->arg[0];
	g_->players[com->client_id].competences[ANTIBODY_NB] = com->arg[1];
	g_->players[com->client_id].competences[MESSAGES_NB] = com->arg[2];
	g_->players[com->client_id].competences[VISION] = com->arg[3];
	Position dep = Position(0, 0);
	c_->UpdateFogOfWar(com->client_id, dep, g_->players[com->client_id], false);
	break;
      }
    case PHAGOCYTE:
      {
	LOG1("Phagocytose requested by %1 to [%2, %3]", com->client_id, com->arg[0], com->arg[1]);
	g_->Phagocytose (com->client_id, com->arg[0], com->arg[1]);
	break;
      }
    default :
      {
	//	LOG1("Unexpected message arrives... id : %1", com->type);
      }
    }
}
