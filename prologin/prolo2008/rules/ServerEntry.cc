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
  LOG3("Map file is: %1", map_file);

  map_stream.open(map_file.c_str());
  if (!map_stream) {
    ERR("Unable to load %1", map_file);
    return 1;
  }

  if (map_stream.peek() == '#') // comment
    {
      getline(map_stream, line);
    }
  if (!(map_stream >> size_y >> size_x)) {
    ERR("Expecting size_y and size_x separated by a space");
    return 1;
  }

  if (size_x <= 4 || size_y <= 4 || size_x >= MAP_MAX_X || size_y >= MAP_MAX_Y) {
    ERR("Map size are invalid, go fix your map");
    return 1;
  }
  g_->_map_size_x = (unsigned int) size_x;
  g_->_map_size_y = (unsigned int) size_y;

  while (map_stream.peek() != '\n') map_stream.ignore();
  map_stream.ignore();

  int nb_robots[2] = {0};

  for (int i = 0 ; i < size_y ; ++i) {
    getline(map_stream, line);
    if (!map_stream) {
      ERR("Not enough line in map file");
      return 1;
    }
    if ((int)line.size() != size_x) {
      ERR("Line %1 of map has unexpected size %2 instead of %3", i, line.size(), size_x);
      return 1;
    }
    for (int j=0 ; j < size_x ; ++j) {
      switch(line[j]) {
      case MAP_ROBOT_TEAM1 :
	if (nb_robots[0] >= MAX_ROBOTS / 2) {
	  ERR("Too many robots for team 1, whose limit is %1", MAX_ROBOTS/2);
	  return 1;
	}
	g_->_robots[nb_robots[0]++].Init(j,i,0);
	break;
      case MAP_ROBOT_TEAM2 :
	if (nb_robots[1] >= MAX_ROBOTS/2) {
	  ERR("Too many robots for team 2, whose limit is %1", MAX_ROBOTS/2);
	  return 1;
	}
	g_->_robots[MAX_ROBOTS/2 + nb_robots[1]++].Init(j,i,1);
	break;
      case MAP_WALL :
      case MAP_EMPTY :
      case MAP_HOLE :
      case MAP_BALL :
	g_->_map[i][j] = line[j];
	break;
      default :
	ERR("Unexpected char in map at pos %1,%2", i, j);
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

  //max_turn :
  g_->_max_turn =  cfg_->getValue<int>("max_turn");
  //broadcast map size :
  StechecPkt com(INIT_DATA, -1);
  com.Push(3, INIT_MAP_SIZE, g_->_map_size_y, g_->_map_size_x);
  SendToAll(com);

  //broadcast the map :
  for (int i=0 ; i < g_->_map_size_y ; i++)
    for (int j=0 ; j < g_->_map_size_x ; j++) {
      StechecPkt com(MAP_CONTENT, -1);
      com.Push(3, i, j, g_->_map[i][j]);
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
  return 0;
}

int         ServerEntry::afterNewTurn(void)
{
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
  return -42;
}
