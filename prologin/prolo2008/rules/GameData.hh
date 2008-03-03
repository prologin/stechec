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
  //storing only the environnement that doesn't move : floor, wall and hole (MAP_WALL|MAP_EMPTY|MAP_HOLE)
  //remainder : team 0 starts from coordinates 0,1.. and team 1 from coordinates Y_MAX-1,Y_MAX-2...
  unsigned char _balls[MAP_MAX_Y][MAP_MAX_X]; //storing balls that are on the floor (MAP_BALL|MAP_EMPTY)
  unsigned char _balls_old[MAP_MAX_Y][MAP_MAX_X]; // to keep track of the differences between client and server
  unsigned int	_map_size_y;
  unsigned int	_map_size_x;

  unsigned int	_start_team;			/// Starting team [0-1]

  int _max_turn;
  int _nb_robots; //ne pas oublier de le transmettre client->server
  Robot	_robots[MAX_ROBOTS]; // Robots from team 0 : [0, MAX_ROBOTS/2) , robots from team 1 : [MAX_ROBOTS/2, MAX_ROBOTS)


  // Some data to keep track of the order of the orders issued by the robots
  // This data is not intended to be synchronized with the server
  int _count_orders_per_robot[MAX_ROBOTS]; // For each robots, how many orders has been issued ?
  int _robot_turn[MAX_ROBOTS]; // For each robot, his turn among the others robots in the same team. Equal to -1 if not initialized.
  int _next_turn; // The robot's turn to be given.
  

  int _distances[MAP_MAX_Y][MAP_MAX_X][MAP_MAX_Y][MAP_MAX_X];
  void ComputeDistances();

  bool makeChecks(); //performs some checks to see if all is going well
};

#endif // !GAMEDATA_HH_
