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

#include "ClientDiffer.hh"

/*
** Dispatch le s_com
*/
void ClientDiffer::ApplyDiff(const StechecPkt* com)
{
  assert(com);

  player_t* p = NULL;
  if (com->client_id != -1)
    p = &g_->player[com->client_id];

  switch (com->type)
    {
    case POSITION_ANTHILL:
      {
        int player_id = com->arg[0];
        int x = com->arg[1];
        int y = com->arg[2];
        g_->player[player_id].anthill_x = x;
        g_->player[player_id].anthill_y = y;
        g_->map[x][y] = ANTHILL;
      }
      break;

    case MAP_CONTENT:
      {
        int x = com->arg[0];
        int y = com->arg[1];
        g_->map[x][y] = com->arg[2];
      }
      break;
      
    case ANT_NEW_UNIT:
      {
        p->all_unit[p->nb_unit].x = p->anthill_x;
        p->all_unit[p->nb_unit].y = p->anthill_y;
        p->nb_unit++;
        p->amount_of_food -= 10;
        if (p->amount_of_food < 0)
          p->amount_of_food = 0;
      }
      break;
      
    case ANT_MOVE:
      {
        int id = com->arg[0];
        int x = com->arg[1];
        int y = com->arg[2];
        unit_t *unit = &p->all_unit[id];
        g_->MoveAnt(com->client_id, unit->x, unit->y, x, y);

        // flag la map comme etant connue
        for (int ix = x - 1; ix <= x + 1; ix++)
          for (int iy = y - 1; iy <= y + 1; iy++)
            if (ix >= 0 && ix < g_->map_size_x && iy >= 0 && iy < g_->map_size_y)
              g_->player_know_map[com->client_id][ix][iy] = true;

        unit->x = x;
        unit->y = y;
      }
      break;
      
    case ANT_GET_FOOD:
      {
        int id = com->arg[0];
        unit_t *unit = &p->all_unit[id];
        
        unit->food++;
      }
      break;
      
    case ANT_LET_FOOD:
      {
        int id = com->arg[0];
        unit_t *unit = &p->all_unit[id];
        
        unit->food--;
        p->amount_of_food++;
      }
      break;
    }
}
