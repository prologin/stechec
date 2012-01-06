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

#include "Api.hh"

// global used in interface.cc
Api* api;

Api::Api(GameData* gameData, Client* c) :
  StechecApi(gameData, c)
{
  // set global.
  api = this;
}

Api::~Api()
{
}

void Api::teamSwitched()
{
  // Shortcut to get my team (or the selected team for viewers).
  if (g_->getUid() >= 0)
    g_->p = &g_->player[g_->getUid()];
  else
    g_->p = NULL;
}


int Api::get_my_team()
{
  if (g_->p != NULL)
    return g_->p->id;
  return -1;
}

int Api::get_nb_ant(int team_id)
{
  if (team_id < 0 || team_id >= g_->getNbPlayer())
    return BAD_ARGUMENT;

  return g_->player[team_id].nb_unit;
}

int Api::get_landscape_width()
{
  return g_->map_size_x;
}

int Api::get_landscape_height()
{
  return g_->map_size_y;
}

int Api::get_anthill_pos_x(int team_id)
{
  if (team_id < 0 || team_id >= g_->getNbPlayer())
    return BAD_ARGUMENT;

  return g_->player[team_id].anthill_x;
}

int Api::get_anthill_pos_y(int team_id)
{
  if (team_id < 0 || team_id >= g_->getNbPlayer())
    return BAD_ARGUMENT;

  return g_->player[team_id].anthill_y;
}

int Api::get_ant_pos_x(int ant_id)
{
  if (g_->p == NULL || ant_id < 0 || ant_id >= g_->p->nb_unit)
    return BAD_ARGUMENT;

  return g_->p->all_unit[ant_id].x;
}

int Api::get_ant_pos_y(int ant_id)
{
  if (g_->p == NULL || ant_id < 0 || ant_id >= g_->p->nb_unit)
    return BAD_ARGUMENT;

  return g_->p->all_unit[ant_id].y;
}

int Api::get_land_type(int x, int y)
{
  if (x < 0 || x >= g_->map_size_x || y < 0 || y >= g_->map_size_y)
    return BAD_ARGUMENT;

  if (g_->getUid() < 0)
    return g_->map[x][y];
  if (g_->player_fog[g_->getUid()][x][y] > 0 || g_->player_know_map[g_->getUid()][x][y])
    return g_->map[x][y];
  return UNKNOWN;
}

int Api::get_land_ant(int x, int y)
{
  if (x < 0 || x >= g_->map_size_x || y < 0 || y >= g_->map_size_y)
    return BAD_ARGUMENT;

  if (g_->getUid() < 0)
    return g_->map_ant[x][y];
  if (g_->player_fog[g_->getUid()][x][y] > 0)
    return g_->map_ant[x][y];
  return NO_ANT;
}

int Api::get_ant_luggage(int ant_id)
{
  if (g_->p == NULL || ant_id < 0 || ant_id >= g_->p->nb_unit)
    return BAD_ARGUMENT;

  return g_->p->all_unit[ant_id].food;
}

//
// these three functions ask something from server
//
  
int Api::move_ant(int ant_id, int x, int y)
{
  if (x < 0 || x >= g_->map_size_x || y < 0 || y >= g_->map_size_y)
    return BAD_ARGUMENT;
  if (ant_id < 0 || ant_id >= g_->p->nb_unit)
    return BAD_ARGUMENT;

  if (g_->map[x][y] == OBSTACLE)
    return INVALID_COMMAND;

  StechecPkt com(ANT_MOVE, -1);
  com.Push(3, ant_id, x, y);
  SendToServer(com);

  return SUCCESS;
}

int Api::carry_food(int ant_id)
{
  if (ant_id < 0 || ant_id >= g_->p->nb_unit)
    return BAD_ARGUMENT;

  if (g_->map[g_->p->all_unit[ant_id].x][g_->p->all_unit[ant_id].y] != FOOD)
    return INVALID_COMMAND;

  StechecPkt com(ANT_GET_FOOD, -1);
  com.Push(1, ant_id);
  SendToServer(com);

  return SUCCESS;
}

int Api::let_food(int ant_id)
{
  if (ant_id < 0 || ant_id >= g_->p->nb_unit)
    return BAD_ARGUMENT;

  if ((int)g_->p->all_unit[ant_id].x == g_->p->anthill_x
      && (int)g_->p->all_unit[ant_id].y == g_->p->anthill_y)
    {
      StechecPkt com(ANT_LET_FOOD, -1);
      com.Push(1, ant_id);
      SendToServer(com);
      return SUCCESS;
    }
  return INVALID_COMMAND;
}

/* not really useful yet, as ant can't die */
int Api::get_next_ant()
{
  static int current_id = 0;

  if (++current_id == g_->p->nb_unit)
    current_id = 0;
  return current_id - 1;
}
