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

int Api::taille_carte_x()
{
  return g_->_map_size_x;
}

int Api::taille_carte_y()
{
  return g_->_map_size_y;
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
  ChampionIdToRealId(&id);
  return g_->_robots[id].GetXPos();
}

int Api::pos_y(int id)
{
  CHECK_ID(id);
  ChampionIdToRealId(&id);
  return SwitchSubjectiveGlobalCoords(g_->_robots[id].GetYPos());
}

int Api::porte_pomme(int id)
{
  CHECK_ID(id);
  ChampionIdToRealId(&id);
  return g_->_robots[id].HasBall();
  // warning : a team can see whether an enemy robot is holding a ball or not
}

int Api::distance(int x1, int y1, int x2, int y2)
{
  TEST_POS(x1,y1);
  TEST_POS(x2,y2);

  return g_->_distances[SwitchSubjectiveGlobalCoords(y1)][x1][SwitchSubjectiveGlobalCoords(y2)][x2];
  //returns INFINI if there is no path
}

int Api::type_case(int x, int y)
{
  TEST_POS(x,y);
  y = SwitchSubjectiveGlobalCoords(y);

  switch(g_->_map[y][x]) {
  case MAP_WALL : 
    return OBSTACLE;
  case MAP_EMPTY :
    return NORMAL;
  case MAP_HOLE:
    return TRANCHEE;
  default:
    LOG2("bogus map at pos %1,%2 : '%3'", x,y,g_->_map[y][x]);
    assert(0); 
    //the map is bogus..
    return BAD_ARGUMENT;
  };
}

int Api::pomme(int x, int y)
{
  TEST_POS(x,y);
  y = SwitchSubjectiveGlobalCoords(y);
  return g_->_balls[y][x] == MAP_BALL;
}

int Api::deplacer(int id, int direction)
{
  CHECK_OWN_ID(id);
  CHECK_DIRECTION(direction, 0);
  ChampionIdToRealId(&id);
  SwitchSubjectiveGlobalDir(&direction);
  int turn = Order(id);
  if (turn < 0) return turn;

  SendOrderWithDirection(id, turn, MOVE, direction);
  LOG4("Robot id %1 asks for moving to direction %2, robot's turn = %3, index of order = %4",
       id, direction, turn, g_->_count_orders_per_robot[id]);
  g_->_count_orders_per_robot[id]++;

  return 0;
}

int Api::turbo(int id)
{
  CHECK_OWN_ID(id);
  ChampionIdToRealId(&id);

  int turn = Order(id);
  if (turn < 0) return turn;

  SendBasicOrder(id, turn, BOOST_TURBO);
  LOG4("Robot id %1 asks for boosting turbo, robot's turn = %2, index of order = %3",
       id, turn, g_->_count_orders_per_robot[id]);
  g_->_count_orders_per_robot[id]++;

  return 0;
}

int Api::lacher_pomme(int id, int direction)
{
  CHECK_OWN_ID(id);
  CHECK_DIRECTION(direction, 1);
  ChampionIdToRealId(&id);
  SwitchSubjectiveGlobalDir(&direction);
  int turn = Order(id);
  if (turn < 0) return turn;
  SendOrderWithDirection(id, turn, DROP_BALL, direction);

  LOG4("Robot id %1 wants to drop the ball, robot's turn = %2, index of order = %3",
       id, turn, g_->_count_orders_per_robot[id]);
  g_->_count_orders_per_robot[id]++;
  return 0;
}

int Api::ramasser_pomme(int id)
{
  CHECK_OWN_ID(id);
  ChampionIdToRealId(&id);
  
  int turn = Order(id);
  if (turn < 0) return turn;
  SendBasicOrder(id, turn, PICK_UP_BALL);
  LOG4("Robot id %1 wants to take the ball, robot's turn = %2, order number %3", 
       id, turn, g_->_count_orders_per_robot[id]);

  g_->_count_orders_per_robot[id]++;

  return 0;
}

int Api::grapin(int id, int direction)
{
  CHECK_OWN_ID(id);
  CHECK_DIRECTION(direction, 0);
  ChampionIdToRealId(&id);
  SwitchSubjectiveGlobalDir(&direction);
  int turn = Order(id);
  if (turn < 0) return turn;

  SendOrderWithDirection(id, turn, HOOK_ROBOT, direction);
  LOG4("Robot id %1 wants to throw hook to direction %2, robot's turn = %3, index of order = %4",
       id, direction, turn, g_->_count_orders_per_robot[id]);
  g_->_count_orders_per_robot[id]++;
  return 0;
}

int Api::projectile(int id, int direction)
{
  CHECK_OWN_ID(id);
  CHECK_DIRECTION(direction, 0);
  ChampionIdToRealId(&id);
  SwitchSubjectiveGlobalDir(&direction);

  int turn = Order(id);
  if (turn < 0) return turn;
  
  SendOrderWithDirection(id, turn, LAUNCH_BULLET, direction);
  LOG4("Robot id %1 wants to throw a bullet to direction %2, robot's turn = %3, index of order = %4",
       id, direction, turn, g_->_count_orders_per_robot[id]);
  g_->_count_orders_per_robot[id]++;
  return 0;
}

int Api::attendre(int id)
{
  CHECK_OWN_ID(id);
  ChampionIdToRealId(&id);
  int turn = Order(id);
  if (turn < 0) return turn;

  SendBasicOrder(id, turn, WAIT);
  LOG4("Robot id %1 wants to wait, robot's turn = %2, order number %3", 
       id, turn, g_->_count_orders_per_robot[id]);
  g_->_count_orders_per_robot[id]++;
  return 0;
}


void Api::teamSwitched()
{
  // GUI stuff. don't care about it
}


void Api::ChampionIdToRealId(int *id)
{
  *id += MAX_ROBOTS / 2 * g_->getTeamId();
  *id %= MAX_ROBOTS;
}

int Api::Order(int real_id) 
{
  if (g_->_count_orders_per_robot[real_id] >= MAX_ORDERS)
    return TOO_MANY_ORDERS;
  
  //compute robot's turn :
  int &turn = g_->_robot_turn[real_id];
  if (turn == -1) 
    turn = g_->_next_turn++;

  return turn;
}

void Api::SendBasicOrder(int real_id, int robot_turn, int order_cst) 
{
  StechecPkt com(order_cst, -1);
  com.Push(3, real_id, robot_turn, g_->_count_orders_per_robot[real_id]);
  SendToServer(com);
}

void Api::SendOrderWithDirection(int real_id, int robot_turn, int order_cst, int direction) {
  StechecPkt com(order_cst, -1);
  com.Push(4, real_id, robot_turn, g_->_count_orders_per_robot[real_id], direction);
  SendToServer(com);
}


int Api::SwitchSubjectiveGlobalCoords(int y) {
  if (g_->getTeamId() == 1) {
    return taille_carte_y() - y - 1;
  }
  return y;
}

void Api::SwitchSubjectiveGlobalDir(int *dir) {
  if (g_->getTeamId() == 1) {
    if (*dir == BAS) *dir = HAUT;
    else if (*dir == HAUT) *dir = BAS;
  }
}
