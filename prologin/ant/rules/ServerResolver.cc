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

#include "ServerResolver.hh"


void         ServerResolver::ApplyResolver(CommandListRef cmdList[])
{
  CommandListRef::iterator it;

  for (int it_type = 0; it_type < 10; ++it_type)
    for (it = cmdList[it_type].begin(); it != cmdList[it_type].end(); ++it)
      {
        switch ((*it)->type)
          {
          case ANT_GET_FOOD:
            resolv_take_food(*it);
            break;
          
          case ANT_LET_FOOD:
            resolv_poser_food(*it);
            break;
          
          case ANT_MOVE:
            resolv_move(*it);
            break;
          }
      }
}

/**
** prend la bouffe
*/
void        ServerResolver::resolv_take_food(const StechecPkt* elt)
{
  player_t* p = &g_->player[elt->client_id];
  
  int id = elt->arg[0];
  unit_t *unit = &p->all_unit[id];
  
  if (unit->food < MAX_FOOD_UNIT)
    {
      unit->food++;
      g_->map[unit->x][unit->y] = NOTHING;
      
      // ok, peut recuperer la nourriture
      SendToAll(*elt);
      
      // il y a eu une moodification de la carte
      StechecPkt com(MAP_CONTENT, -1);
      com.Push(3, unit->x, unit->y, NOTHING);
      SendToAll(com);
    }
}

/**
** pose la nourriture dans la fourmiliere
*/
void        ServerResolver::resolv_poser_food(const StechecPkt* elt)
{
  player_t* p = &g_->player[elt->client_id];
    
  int id = elt->arg[0];
  unit_t *unit = &p->all_unit[id];
  
  if (unit->food > 0)
    {
      unit->food--;
      p->amount_of_food++;
      
      /* accepte de poser une unite de nourriture */
      SendToAll(*elt);
      
      if (p->amount_of_food >= 10)
        {
          /* nouvelle unitee */
          StechecPkt new_unit(ANT_NEW_UNIT, elt->client_id);
          SendToAll(new_unit);
          
          p->amount_of_food = 0;
          p->nb_unit++;
        }
    }
}

/**
** Deplace la fourmis
*/
void        ServerResolver::resolv_move(const StechecPkt* elt)
{
  player_t* p = &g_->player[elt->client_id];
  
  int id = elt->arg[0];
  int x = elt->arg[1];
  int y = elt->arg[2];
  unit_t* unit = &p->all_unit[id];

  if (abs(unit->x - x) > 1  || abs(unit->y - y) > 1)
    return;

  // pas de deplacement en diagonale
  if (x - unit->x && y - unit->y)
    return;
  
  if (g_->map[x][y] == NOTHING
      || g_->map[x][y] == FOOD
      || (g_->map[x][y] == ANTHILL
          && x == p->anthill_x
          && y == p->anthill_y))
    {
      if (g_->map_ant[x][y] == NO_ANT)
        {
          /* met a jour la carte et le brouillard de guerre */
          g_->MoveAnt(elt->client_id, unit->x, unit->y, x, y);
          unit->x = x;
          unit->y = y;
          
          /* signale qu'elle peut se deplacer */
          SendToAll(*elt);
        }
    }
}
