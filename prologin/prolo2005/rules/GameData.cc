/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#include "GameData.hh"


GameData::GameData()
  : terrain_value(NULL),
    players(NULL)
{
}

GameData::~GameData()
{
  if (terrain_value != NULL)
    {
      for (unsigned x = 0; x < map_size_x; x++)
	{
	  free(terrain_value[x]);
	  free(terrain_value_mod[x]);
	  free(terrain_type[x]);
	  free(terrain_building[x]);
	  free(terrain_coleoptere[x]);
	}
      free(terrain_value);
      free(terrain_value_mod);
      free(terrain_type);
      free(terrain_building);
      free(terrain_coleoptere);
    }
  delete[] players;
}

void GameData::InitMap()
{
  terrain_value = (int**) malloc(sizeof (int*) * map_size_x);
  terrain_value_mod = (uint8_t**) malloc(sizeof (uint8_t*) * map_size_x);
  terrain_type = (uint8_t**) malloc(sizeof (uint8_t*) * map_size_x);
  terrain_coleoptere = (Coleoptere***) malloc(sizeof (Coleoptere***) * map_size_x);
  terrain_building = (Building***) malloc(sizeof (Building***) * map_size_x);
  for (unsigned x = 0; x < map_size_x; x++)
    {
      terrain_value[x] = (int*) malloc(sizeof (int) * map_size_y);
      terrain_value_mod[x] = (uint8_t*) malloc(sizeof (uint8_t) * map_size_y);
      terrain_type[x] = (uint8_t*) malloc(sizeof (uint8_t) * map_size_y);
      terrain_coleoptere[x] = (Coleoptere**) calloc(sizeof (Coleoptere*), map_size_y);
      terrain_building[x] = (Building**) calloc(sizeof (Building*), map_size_y);

      memset(terrain_value[x], 0x42, map_size_y);
      memset(terrain_value_mod[x], 100, map_size_y);
    }
}

Coleoptere::Coleoptere()
{
  state = STATE_DEAD;
}

void Coleoptere::Reinit()
{
  stock_ore = MAX_STOCK_COL_ORE;
  stock_plasma = MAX_STOCK_COL_PLASMA;
  set_damage(0);
  state = STATE_NORMAL;
  dock_build = 0;
  build_time = 0;
  dock_col = NULL;
  dock_build = NULL;
  transfert_ore = 0;
  action = false;
}

Factory::Factory()
{
  col_repair = NULL;
  stock_ore = 0;
  stock_plasma = 0;
  buildlist = 0;
}

Player::Player()
{
  factory_count = 0;
  central_count = 0;
  mine_count = 0;
  hotel_count = 0; 
}



/*!
** @brief Module description
*/
extern "C" const struct RuleDescription rules_description = {
  "prolo2005",
  "Prologin 2005 final contest rules",
  "libzavie",
  1,
  0,
};
