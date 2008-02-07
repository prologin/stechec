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

#include <stdlib.h>

#include "Api.hh"


// global used in interface.cc
Api* api;

Api::Api(GameData* gameData, Client* c)
  : StechecApi(gameData, c)
{
  api = this;
}

int Api::numero_tour()
{
  return g_->getCurrentTurn();
}

int Api::nombre_tours()
{
  return g_->_max_turn;
}

bool Api::commence()
{
  return (numero_tour()%2) ^ (g_->getTeamId() != g_->_start_team);
}

int Api::pos_x(int id)
{
  CHECK_ID(id);
  return g_->_robots[id].GetXPos();
}

int Api::pos_y(int id)
{
  CHECK_ID(id);
  return g_->_robots[id].GetYPos();
}

bool Api::porte_balle(int id)
{
  CHECK_ID(id);
  return g_->_robots[id].HasBall();
  // warning : a team can see whether an enemy robot is holding a ball or not
}

int Api::distance(int x1, int y1, int x2, int y2)
{
  return 0;
}

int Api::type_case(int x, int y)
{
  TEST_POS(x,y);
  switch(g_->_map[y][x]) {
  case MAP_WALL : 
    return MUR;
  case MAP_EMPTY :
    return NORMAL;
  case MAP_HOLE:
    return TROU;
  default:
    assert(0); 
    //the map is bogus..
    return BAD_ARGUMENT;
  };
}

bool Api::balle(int x, int y)
{
  TEST_POS(x,y);
  return g_->_balls[x][y] == MAP_BALL;
}

int Api::deplacer(int id, int direction)
{
  CHECK_ID(id);
  CHECK_DIRECTION(direction);
  return 0;
}

int Api::turbo(int id)
{
  CHECK_ID(id);
  return 0;
}

int Api::lacher_balle(int id)
{
  CHECK_ID(id);
  return 0;
}

int Api::prendre_balle(int id)
{
  CHECK_ID(id);
  return 0;
}

int Api::grapin(int id, int direction)
{
  CHECK_ID(id);
  CHECK_DIRECTION(direction);
  return 0;
}

int Api::attendre(int id)
{
  CHECK_ID(id);
  return 0;
}


void Api::teamSwitched()
{
  // GUI stuff. don't care about it
}
