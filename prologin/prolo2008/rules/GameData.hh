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

#ifndef GAMEDATA_HH_
# define GAMEDATA_HH_

# include <list>
# include <map>
# include <algorithm>
# include "Contest.hh"
# include "Constant.hh"
# include "Robot.hh"

/*!
** This class is meant to contain all data, accessible from
** everywhere.
*/
class GameData: public StechecGameData
{
public:
  GameData(void);

  unsigned char	_map[MAP_MAX_Y][MAP_MAX_X];
  unsigned int	_map_size_y;
  unsigned int	_map_size_x;

  unsigned int	_start_player;			/// Starting player [0-1]

  int _max_turn;
  int _nb_robots;
  Robot		_robots[MAX_ROBOTS]; // Robots from team 0 : [0, _nb_robots/2) , robots from team 1 : [_nb_robots/2, _nb_robots)
};

#endif // !GAMEDATA_HH_
