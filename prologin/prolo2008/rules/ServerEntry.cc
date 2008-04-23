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

#include <fstream>
#include <string>
#include <time.h>
#include "GameData.hh"
#include "ServerEntry.hh"
#include "misc/Conf.hh"

ServerEntry::ServerEntry(GameData* game, Server* server,
                         const ConfSection* cfg)
  : StechecServerEntry(game, server, cfg)
{
}

ServerEntry::~ServerEntry(void)
{
}


int ServerEntry::loadMap(void)
{
  std::string	map_file;
  std::ifstream	map_stream;
  std::string line;
  int size_y, size_x;

  map_file = cfg_->getValue<std::string>("map");
  LOG2("Map file is: %1", map_file);

  map_stream.open(map_file.c_str());
  if (!map_stream)
  {
    ERR("Unable to load %1", map_file);
    return 1;
  }

  // If the first lines are comments, discard them
  while (map_stream.peek() == '#')
  {
    getline(map_stream, line); 
  }

  // The first line is "size_x size_y\n"
  if (!(map_stream >> size_x >> size_y))
  {
    ERR("Expecting size_x and size_y separated by a space");
    return 1;
  }

  if (size_x <= 4 || size_y <= 4 || size_x > MAP_MAX_X || size_y > MAP_MAX_Y)
  {
    ERR("Map sizes are invalid, go fix your map");
    return 1;
  }

  g_->_map_size_x = (unsigned int) size_x;
  g_->_map_size_y = (unsigned int) size_y;

  while (map_stream.peek() != '\n')
    map_stream.ignore();
  map_stream.ignore();

  int nb_robots[2] = {0};

  for (int j = 0; j < size_y; ++j)
  {
    getline(map_stream, line);
    if (!map_stream)
    {
      ERR("Not enough lines in map file");
      return 1;
    }

    if ((int) line.size() != size_x)
    {
      ERR("Line %1 of map has unexpected size %2 instead of %3", j, line.size(), size_x);
      return 1;
    }

    for (int i = 0; i < size_x; ++i)
    { 
      switch (line[i])
      {
        case MAP_ROBOT_TEAM1 :
          if (nb_robots[0] >= MAX_ROBOTS / 2)
          {
            ERR("Too many robots for team 0, whose limit is %1", MAX_ROBOTS / 2);
            return 1;
          }
	  g_->_nb_robots++;
          g_->_robots[nb_robots[0]++].Init(i, j, 0);
	  LOG6("Robot in pos %1,%2 for team 0", i, j);
          break;

        case MAP_ROBOT_TEAM2 :
          if (nb_robots[1] >= MAX_ROBOTS / 2)
          {
            ERR("Too many robots for team 1, whose limit is %1", MAX_ROBOTS / 2);
            return 1;
          }
	  g_->_nb_robots++;
          g_->_robots[MAX_ROBOTS / 2 + nb_robots[1]++].Init(i, j, 1);
	  LOG6("Robot in pos %1,%2 for team 1", i, j);
          break;

      case MAP_WALL :
      case MAP_EMPTY :
      case MAP_HOLE :
      case MAP_HOLE_ALTENATE:
	g_->_map[j][i] = line[i];
	if (g_->_map[j][i] == MAP_HOLE_ALTENATE)
	  g_->_map[j][i] = MAP_HOLE;
	break;
      case MAP_BALL :
	g_->_balls[j][i] = MAP_BALL;	
	LOG6("Set ball at %1,%2", i, j);
	break;
      default :
	ERR("Unexpected char in map at pos %1,%2 : '%3'", i, j, line[i]);
	return 1;
      }
    }
  }

  return 0;
}


int		ServerEntry::beforeGame(void)
{
  if (loadMap())
    return 1;

  g_->_max_turn = cfg_->getValue<int>("max_turn");

  // Broadcasts initalization data (map size, etc.)
  StechecPkt com(INIT_DATA, -1);
  com.Push(3, INIT_MAP_SIZE, g_->_map_size_x, g_->_map_size_y);
  SendToAll(com);

  for (int i=0 ; i < MAX_ROBOTS ; i++)
    if (g_->_robots[i].IsEnabled())
      SendToAll(INIT_DATA, -1, 5, INIT_ROBOT, i, g_->_robots[i]._pos_x, g_->_robots[i]._pos_y, g_->_robots[i]._team_id);

  SendToAll(INIT_DATA, -1, 2, INIT_START_TEAM, g_->_start_team);

  // Broadcasts the map's content
  for (int j = 0; j < g_->_map_size_y; j++)
    for (int i = 0; i < g_->_map_size_x; i++)
    {
      StechecPkt com(MAP_CONTENT, -1);
      com.Push(3, i, j, g_->_map[j][i]);
      SendToAll(com);
    }

  //broadcasts the balls map's content
  for (int j = 0; j < g_->_map_size_y; j++)
    for (int i = 0; i < g_->_map_size_x; i++)
      {
	StechecPkt com(BALLS_CONTENT, -1);
	com.Push(3, i, j, g_->_balls[j][i]);
	SendToAll(com);
      }

  return 0;
}

int         ServerEntry::initGame(void)
{
  return 0;
}

int         ServerEntry::beforeNewTurn(void)
{
  for (int i = 0; i < MAX_ROBOTS; ++i)
  {
    g_->_robots[i].ResetHook();
    g_->_robots[i].ResetTurbo();
  }

  std::copy(*g_->_balls, *g_->_balls + MAP_MAX_Y * MAP_MAX_X, *g_->_balls_old);
  std::fill(g_->_actions_last_turn, g_->_actions_last_turn + MAX_ACTIONS, BAD_ARGUMENT);
  g_->_num_actions_last_turn = 0;
  return 0;
}

int         ServerEntry::afterNewTurn(void)
{

  /*
   * First of all : broadcast all robot data
   */

  for (int i = 0; i < MAX_ROBOTS; ++i)
  {
    if (g_->_robots[i].IsEnabled())
    {
      LOG6("Broadcasting robot's %1 data", i);
      SendToAll(ROBOT_POS, -1, 3, i, g_->_robots[i].GetXPos(), g_->_robots[i].GetYPos());
      SendToAll(ROBOT_UNHOOK, -1, 1, i); //inutile : en fait, aucune information de grappin n'est utile pour les clients.
      SendToAll(ROBOT_HAS_BALL, -1, 2, i, g_->_robots[i].HasBall());
      SendToAll(ROBOT_REMAINING_BULLETS, -1, 2, i, g_->_robots[i]._remaining_bullets);
    }
  }

  /*
   * Broadcast balls
   */

  for (int i=0 ; i < g_->_map_size_y ; ++i)
    for (int j=0 ; j < g_->_map_size_x ; ++j)
      if (g_->_balls_old[i][j] != g_->_balls[i][j])
	SendToAll(BALLS_CONTENT, -1, 3, j, i, g_->_balls[i][j]);

  /*
   * Broadcast actions from last turn
   */
  SendToAll(ACTION_LAST_TURN, -1, 2, 0, ACTION_INIT);
  for (int i=0 ; i < g_->_num_actions_last_turn ; i++) 
    SendToAll(ACTION_LAST_TURN, -1, 2, i, g_->_actions_last_turn[i]);
  LOG6("There are %1 actions to broadcast", g_->_num_actions_last_turn);

  /*
   * make checks
   */
  g_->makeChecks(); 

  return 0;
}


int         ServerEntry::afterGame(void)
{
  return 0;
}

bool        ServerEntry::isMatchFinished(void)
{
  return g_->getCurrentTurn() >= g_->_max_turn;
}

int        ServerEntry::getScore(int uid)
{
  int balls_team0 = std::count(g_->_balls[0], g_->_balls[0] + 2 * MAP_MAX_X, MAP_BALL);
  int balls_team1 = std::count(g_->_balls[g_->_map_size_y-2], g_->_balls[g_->_map_size_y-2] + 2 * MAP_MAX_X, MAP_BALL);
  if (balls_team1 == balls_team0)
    return 1;
  else
    if (balls_team0 > balls_team1)	
      return 3 * (1-uid);
    else
      return 3 * uid;
}
