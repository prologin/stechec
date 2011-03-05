/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2004, 2005, 2006 Prologin
*/

#include "Api.hh"

// from fourmi-Api.cc
extern Api* api;

extern "C" {

int get_my_team()
{
  return api->get_my_team();
}

int get_nb_ant(int team_id)
{
  return api->get_nb_ant(team_id);
}

int get_landscape_width()
{
  return api->get_landscape_width();
}

int get_landscape_height()
{
  return api->get_landscape_height();
}

int get_anthill_pos_x(int team_id)
{
  return api->get_anthill_pos_x(team_id);
}

int get_anthill_pos_y(int team_id)
{
  return api->get_anthill_pos_y(team_id);
}

int get_ant_pos_x(int ant_id)
{
  return api->get_ant_pos_x(ant_id);
}

int get_ant_pos_y(int ant_id)
{
  return api->get_ant_pos_y(ant_id);
}

int get_land_type(int x, int y)
{
  return api->get_land_type(x, y);
}

int get_land_ant(int x, int y)
{
  return api->get_land_ant(x, y);
}

int get_ant_luggage(int ant_id)
{
  return api->get_ant_luggage(ant_id);
}

int move_ant(int ant_id, int x, int y)
{
  return api->move_ant(ant_id, x, y);
}

int carry_food(int ant_id)
{
  return api->carry_food(ant_id);
}

int let_food(int ant_id)
{
  return api->let_food(ant_id);
}

/* not really useful yet, as ant can't die */
int get_next_ant()
{
  return api->get_next_ant();
}

}
