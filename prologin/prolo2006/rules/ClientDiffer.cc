/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#include "ClientDiffer.hh"


void		ClientDiffer::ApplyDiff(const StechecPkt *com)
{
  Player*	p = NULL;
  if (com->client_id >= 0)
    p = &g_->players[com->client_id];

  switch (com->type)
    {
    case MAP_CONTENT:
      {
	int x = com->arg[0];
	int y = com->arg[1];
        g_->terrain_graphic[x][y] = com->arg[2];
        if (com->arg[2] == GRASSY_AREA
            || com->arg[2] == STREET)
          g_->terrain_type[x][y] = STANDARD;
        else if (com->arg[2] == BUILDING)
          g_->terrain_type[x][y] = WALL;
        else
          g_->terrain_type[x][y] = com->arg[2];
	break;
      }

    case MOVE_GOODMAN:
      {
	Player*		p = &g_->players[com->client_id];
	int		id = com->arg[0];
	int		row = com->arg[1];
	int		col = com->arg[2];
	GoodMan&	gdm = p->team[id];
	Position	dep = Position(gdm.row, gdm.col);
	Position	arr = Position(row, col);

	LOG3("Differ: move GoodMan from: %1 to : %2", dep, arr);
        c_->UpdateFogOfWar(com->client_id, dep, arr, true);
	gdm.setRow(row);
	gdm.setCol(col);
	break;
      }
    case GOODMAN_CHANGE_STATE:
      {
	int	id_player = com->client_id;
	int	id = com->arg[0];
	int	state = com->arg[1];

	GoodMan& gdm = g_->players[id_player].team[id];
	gdm.setState(state);
	LOG3("Differ : GoodMan %1 of player %2, new state is %3",
	     com->arg[0], com->arg[2], state);
	break;
      }
    case GOODMAN_DELOREAN:
      {
	if (com->arg[1] == 0)
	  {
	    g_->players[com->arg[2]].team[com->arg[0]].lostDelorean();
	    LOG3("Differ : GoodMan %1 of player %2, lost delorean",
		 com->arg[0], com->arg[2]);
	  }
	else
	  {
	    g_->players[com->arg[2]].team[com->arg[0]].getDelorean();
	    LOG3("Differ : GoodMan %1 of player %2 get delorean",
		 com->arg[0], com->arg[2]);
	  }
	break;
      }
    case GOODMAN_ALMANACH:
      {
	if (com->arg[1] == 0)
	  {
	    g_->players[com->arg[2]].team[com->arg[0]].lostAlmanach();
	    LOG3("Differ : GoodMan %1 of player %2 lost almanach",
		 com->arg[0], com->arg[2]);
	  }
	else
	  {
	    g_->players[com->arg[2]].team[com->arg[0]].getAlmanach();
	    LOG3("Differ : GoodMan %1 of player %2 get almanach",
		 com->arg[0], com->arg[2]);
	  }
	break;
      }
    case GOODMAN_MONEY_CHANGE:
      {
	g_->players[com->client_id].team[com->arg[0]].setMoney(com->arg[1]);
	LOG3("Differ : GoodMan %1 of player %2, money is now %3",
	     com->arg[0], com->client_id, com->arg[1]);
	break;
      }
    case MEMORIZE:
      {
	assert(p != NULL);
	int	id = com->arg[0];

	p->memorize(id);
	LOG3("Differ : Memorizing game %1", id);
	break;
      }
    case DELOREAN_MOVE:
      {
	g_->delorean.row = com->arg[0];
	g_->delorean.col = com->arg[1];
	LOG3("Differ : Delorean Moving to : %1 - %2", com->arg[0], com->arg[1]);
	break;
      }
    case DELOREAN_RELOADING:
      {
	g_->delorean.setReloading(com->arg[0]);
	LOG3("Differ : Delorean Reloading time : %1", com->arg[0]);
	break;
      }
    case DELOREAN_AUTO:
      {
	g_->delorean.setAutonomie(com->arg[0]);
	LOG3("Differ : Delorean Autonomie : %1", com->arg[0]);
	break;
      }
    case ALMANACH_MOVE:
      {
	g_->almanach.row = com->arg[0];
	g_->almanach.col = com->arg[1];
	LOG3("Differ: move almanach row: %1, column: %2", com->arg[0], com->arg[1]);
	break;
      }
    case NEW_GOODMAN:
      {
	Position	dep = Position(0, 0);
	Position	arr = Position(com->arg[2], com->arg[3]);

	g_->players[com->arg[0]].team[com->arg[1]].row = com->arg[2];
	g_->players[com->arg[0]].team[com->arg[1]].col = com->arg[3];
        c_->UpdateFogOfWar(com->arg[0], dep, arr, false);
	break;
      }
    case CHANGE_DATE:
      {
	g_->player_turn = com->arg[0];
	LOG3("Differ: date is now : %1", com->arg[0]);
	break;
      }
    case BET_TIME:
      {
	g_->almanach.getAlmanach()[com->arg[0]]->setTimeLeft(com->arg[1]);
	break;
      }
    case UPDATE_ROM:
      {
	g_->players[com->client_id].recordGame(com->arg[0],
					       com->arg[1] == 0 ? false : true,
					       com->arg[2]);
	break;
      }
    case NEW_CASINO:
      {
	g_->addCasino(com->arg[0], com->arg[1]);
	LOG3("Differ: new casino : row %1, column: %2", com->arg[0], com->arg[1]);
	break;
      }
    case NEW_BET:
      {
	g_->almanach.addBet(com->arg[0], com->arg[1], com->arg[2], com->arg[3]);
	LOG6("Differ: new bet (%1) in casino: %2, kitty: %3, result: %4, begin: %5",
	     g_->almanach.getAlmanach().size() - 1, com->arg[3], com->arg[1],
	     com->arg[2], com->arg[0]);
	break;
      }
    case INIT_DATA:
      {
	g_->max_date = com->arg[0];
	g_->map_size.row = com->arg[1];
	g_->map_size.col = com->arg[2];
      }
    }
}
