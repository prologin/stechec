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

#include "GameData.hh"


GameData::GameData() :
  map(NULL),
  player(NULL),
  map_size_x(-1),
  map_size_y(-1),
  player_fog(NULL),
  map_ant(NULL),
  player_know_map(NULL),
  p(0)
{
}

GameData::~GameData()
{
  // we aren't sure that game was initialized - maybe there was
  // an error before.
  if (map == NULL)
    return;

  for (int x = 0; x < map_size_x; x++)
    {
      free(map[x]);
      free(map_ant[x]);
    }
  for (int i = 0; i < getNbPlayer(); i++)
    {
      for (int j = 0; j < map_size_x; j++)
	free(player_fog[i][j]);
      free(player_fog[i]);
    }
  free(map_ant);
  free(map);
  free(player_fog);
}

void GameData::InitMap()
{
  map_ant = (char **) malloc(map_size_x * sizeof (char *));
  map = (char **)malloc (sizeof (char **) * map_size_x);
  for (int x = 0; x < map_size_x; x++)
    {
      map[x] = (char *) malloc(sizeof (char *) * map_size_y);
      map_ant[x] = (char *) malloc(sizeof (char *) * map_size_y);
      memset(map_ant[x], NO_ANT, map_size_y);
    }
}

void GameData::InitMapMisc()
{
  player_fog = (uint8_t ***) malloc(getNbPlayer() * sizeof (char **));
  for (int i = 0; i < getNbPlayer(); i++)
    {
      player_fog[i] = (uint8_t **) malloc(map_size_x * sizeof (char *));
      for (int j = 0; j < map_size_x; j++)
        player_fog[i][j] = (uint8_t *) calloc(map_size_y, sizeof (char *));

      for (int x = player[i].anthill_x - 2; x <= player[i].anthill_x + 2; x++)
        for (int y = player[i].anthill_y - 2; y <= player[i].anthill_y + 2; y++)
          if (x >= 0 && x < map_size_x && y >= 0 && y < map_size_y)
            player_fog[i][x][y] = 0xFF;
    }
}

void GameData::MoveAnt(int player_id, int from_x, int from_y, int to_x, int to_y)
{
  int        x, y;

  /* deplace la fourmis */
  map_ant[from_x][from_y] = NO_ANT;
  if (map[to_x][to_y] != ANTHILL)
    map_ant[to_x][to_y] = player_id;
  
  /* brouille le champs de vision de la position qu'on quitte */
  for (x = from_x - 1; x <= from_x + 1; x++)
    for (y = from_y - 1; y <= from_y + 1; y++)
      if (x >= 0 && x < map_size_x && y >= 0 && y < map_size_y)
        {
          assert(player_fog[player_id][x][y] != 0);
          if (player_fog[player_id][x][y] != 0xFF)
            player_fog[player_id][x][y]--;
        }

  /* ajoute au champs de vision la position a laquelle on arrive */
  for (x = to_x - 1; x <= to_x + 1; x++)
    for (y = to_y - 1; y <= to_y + 1; y++)
      if (x >= 0 && x < map_size_x && y >= 0 && y < map_size_y)
        {
          assert(player_fog[player_id][x][y] != 0xFE);
          if (player_fog[player_id][x][y] != 0xFF)
            player_fog[player_id][x][y]++;
        }
}

const char* GameData::GetCommandString(int cst)
{
  if (cst < 0 || cst >= LAST_MSG)
    return NULL;

  static const char* names[] =
    {
      "INIT_MAP_SIZE",
      "ANT_NEW_UNIT",
      "ANT_MOVE",
      "ANT_GET_FOOD",
      "ANT_LET_FOOD",
      "MAP_CONTENT",
      "POSITION_ANTHILL"
    };
  return names[cst];
}

/*!
** @brief Module description
*/
extern "C" const struct RuleDescription rules_description = {
  "ant",
  "Prologin's world: ant",
  1,
  0,
};
