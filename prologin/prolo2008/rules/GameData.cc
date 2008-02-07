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

#include "GameData.hh"

GameData::GameData(void)
{
  std::fill(*_map, *_map + MAP_MAX_X * MAP_MAX_Y, MAP_EMPTY);
  std::fill(*_balls, *_balls + MAP_MAX_Y * MAP_MAX_X, MAP_EMPTY);
  _nb_robots = 0;
}
