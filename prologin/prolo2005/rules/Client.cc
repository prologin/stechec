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
#include "Client.hh"

void Client::InitData()
{
  player_fog = (uint16_t***) malloc(sizeof (uint16_t**) * g_->getNbPlayer());
  player_know_map = (bool***) malloc(sizeof (bool**) * g_->getNbPlayer());
  for (int i = 0; i < g_->getNbPlayer(); i++ )
    {
      player_fog[i] = (uint16_t**) malloc(sizeof (uint16_t*) * g_->map_size_x);
      player_know_map[i] = (bool**) malloc(sizeof (bool*) * g_->map_size_x);
      for (unsigned x = 0; x < g_->map_size_x; x++)
        {
          player_fog[i][x] = (uint16_t*) calloc(sizeof (uint16_t), g_->map_size_y);
          player_know_map[i][x] = (bool*) calloc(sizeof (bool), g_->map_size_y);
        }
    }
}

void Client::FreeData()
{
  for (int i = 0; i < g_->getNbPlayer(); i++ )
    {
      for (unsigned x = 0; x < g_->map_size_x; x++)
        {
          free(player_know_map[i][x]);
          free(player_fog[i][x]);
        }
      free(player_fog[i]);
      free(player_know_map[i]);
    }
  free(player_fog);
  free(player_know_map);
}

// mettre from_x a INT_MAX quand nouvelle unite
// methode tres bourrin, si qq un a mieux a proposer...
void Client::UpdateFogOfWar(unsigned player_id, unsigned from_x, unsigned from_y,
                            unsigned to_x, unsigned to_y)
{
  unsigned x, y;

  // brouille le champs de vision de la position qu'on quitte
  if (from_x < INT_MAX)
    for (x = from_x - 5; x <= from_x + 5; x++)
      for (y = from_y - 5; y <= from_y + 5; y++)
        if (x < g_->map_size_x && y < g_->map_size_y &&
            abs(x - from_x) + abs(y - from_y) <= 5)
          {
            assert(player_fog[player_id][x][y] != 0);
            if (player_fog[player_id][x][y] != 0xFF)
              player_fog[player_id][x][y]--;
          }

  // ajoute au champs de vision la position a laquelle on arrive
  for (x = to_x - 5; x <= to_x + 5; x++)
    for (y = to_y - 5; y <= to_y + 5; y++)
      if (x < g_->map_size_x && y < g_->map_size_y &&
          abs(x - to_x) + abs(y - to_y) <= 5)
        {
          if (player_fog[player_id][x][y] != 0xFFF)
            player_fog[player_id][x][y]++;

          // flag la map comme etant connue
          player_know_map[player_id][x][y] = true;
        }
}
