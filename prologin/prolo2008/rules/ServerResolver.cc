/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2008 Prologin
*/

#include <vector>
#include "ServerResolver.hh"

ServerResolver::ServerResolver(GameData* game, Server* server)
  : StechecServerResolver(game, server)
{
  // fill directions :
  std::fill(*_directions, *_directions + 5*2, 0);

  _directions[HAUT][1] = -1;
  _directions[BAS][1] = 1;
  _directions[GAUCHE][0] = -1;
  _directions[DROITE][0] = -1;

}

void ServerResolver::ApplyResolver(CommandListRef cmdList[])
{

  // cmdList is an array representing each types of packets.
  // cmdList[PCK_TYPE] evaluates to a list, driven by an iterator.
 

  // First, put the orders in robot_orders, to know wich the order of each robot's orders.

  std::pair<StechecPkt*,int> robot_orders[MAX_ROBOTS][MAX_ORDERS]; // pair of packet and type of packet
  int robots_turn[MAX_ROBOTS];	
  int turn_to_robot[2][MAX_ROBOTS/2];
  int orders_constants[] = {DROP_BALL, MOVE, PICK_UP_BALL, HOOK_ROBOT, RELEASE_HOOK, BOOST_TURBO, WAIT, LAUNCH_BULLET};
  int size_orders_constants =  (sizeof orders_constants) / sizeof(int);
  std::fill(*robot_orders, *robot_orders + MAX_ROBOTS * MAX_ORDERS, std::make_pair((StechecPkt*)0, -1));
  std::fill(robots_turn, robots_turn + MAX_ORDERS, -1);
  std::fill(*turn_to_robot, *turn_to_robot + 2*(MAX_ROBOTS/2), -1);
  for (int i=0 ; i < size_orders_constants ; i++) {
    int order_cst = orders_constants[i];
    for (CommandListRef::iterator cmd = cmdList[order_cst].begin() ; cmd != cmdList[order_cst].end() ; ++cmd) {
      StechecPkt *pkt= *cmd;
      int id = pkt->arg[0];
      int robot_turn = pkt->arg[1];
      int order_number = pkt->arg[2];
      LOG5("Add order from id %1, robot_turn %2, order_number %3", id, robot_turn, order_number);
      robot_orders[ id ][ order_number ] = std::make_pair(pkt, order_cst);
      robots_turn[ id ] = robot_turn;
      if (id < MAX_ROBOTS / 2) 
	turn_to_robot[0][robot_turn] = id;
      else
	turn_to_robot[1][robot_turn] = id;
    }
  }

  // Resolve robots' orders in the intended order :

  for (int i=0 ; i < MAX_ORDERS ; i++) {
    for (int turn=0 ; turn < MAX_ROBOTS / 2 ; turn++) {
      int current_start_team = g_->_start_team;
      if (g_->getCurrentTurn()%2 == 0)
	current_start_team = !current_start_team;
      int id = turn_to_robot[current_start_team][turn];
      if (id != -1) {
	LOG4("Trying to resolve order from robot %1 of team %2, order numbered %3, turn %4",
	     id, current_start_team, i, turn);
	ResolveOrder( robot_orders[id][i].first, robot_orders[id][i].second );
      }
      id = turn_to_robot[!current_start_team][turn];
      if (id != -1) {
	LOG4("Trying to resolve order from robot %1 of team %2, order numbered %3, turn %4",
	     id, !current_start_team, i, turn);
	ResolveOrder( robot_orders[id][i].first, robot_orders[id][i].second );
      }
    }
  }

}

void ServerResolver::ResolveOrder(StechecPkt *pkt, int type)
{
  if (!pkt || type < 0) 
    return;

  LOG4("Resolving message of type %1, args : %1 %2 %3", type, pkt->arg[0], pkt->arg[1], pkt->arg[2]);

  switch(type) {
  case MOVE : case DROP_BALL : case HOOK_ROBOT :
    ResolveOrderWithDirection(pkt, type);
    break;
  case PICK_UP_BALL : case BOOST_TURBO :
    ResolveSimpleOrder(pkt, type);
  default :
    LOG2("Bogus order coming to the server from robot id %1", pkt->arg[0]);
    assert(0);
  }

}

bool ServerResolver::CheckPosition(int x, int y) {
  return x>= 0 && y >= 0 && x < g_->_map_size_x && y < g_->_map_size_y;
}

int ServerResolver::GetRobotIdInPos(int x, int y) {
  for (int i=0 ; i < MAX_ROBOTS ; ++i)
    if (g_->_robots[i].IsEnabled() && g_->_robots[i]._pos_x == x && g_->_robots[i]._pos_y == y)
      return i;
  return -1;
}

bool ServerResolver::CanDoSimpleMove(int x0, int y0, int x1, int y1, bool pushed=false) {
  if (!CheckPosition(x0,y0) || !CheckPosition(x1,y1))
    return false;
  int cur_cell = g_->_map[y0][x0];
  int next_cell = g_->_map[y1][x1];
  return 
    cur_cell == next_cell && (cur_cell == MAP_HOLE || cur_cell == MAP_EMPTY) ||
    cur_cell == MAP_EMPTY && next_cell == MAP_HOLE ||
    pushed && cur_cell == MAP_HOLE && next_cell == MAP_EMPTY ;
}

void ServerResolver::UpdateRobotPos(int id, int new_x, int new_y) {
  LOG4("Hamster %1 was in %2,%3 and moves to %4,%5", id, g_->_robots[id]._pos_x, g_->_robots[id]._pos_y, new_x, new_y);
  g_->_robots[id]._pos_x = new_x;
  g_->_robots[id]._pos_y = new_y;
  g_->_robots[id].ResetHook(); // Très important, sinon un bug subtile peut aparaitre:
			       // si id est poussé, et que ses ordres sont résolus après, il pourrait entrainer avec lui un robot..
}

bool ServerResolver::ApplyChainMove(int dir, int id, int x, int y, int next_x, int next_y, bool first=true) {
  if (!CheckPosition(x,y) || !CheckPosition(next_x, next_y)) return false;
  int target_id;
  if (CanDoSimpleMove(x,y,next_x, next_y, !first) ) {
    if ( (target_id=GetRobotIdInPos(next_x, next_y)) < 0 ||  //we can move to (next_x, next_y), because there is nothing there
	 ApplyChainMove(dir, target_id, next_x, next_y, 
			next_x + _directions[dir][0], 
			next_y + _directions[dir][1], 
			false)) { // we can move, because the robot there can !
      UpdateRobotPos(id, next_x, next_y);
      int hook = g_->_robots[id].GetHook();
      if (first && hook != -1) {
	//take robot hook with us
	LOG4("Hamster %1 moves to %2,%3, because it was hooked by hamster %4", hook, x, y, id);
	UpdateRobotPos(hook, x, y);
      }
      return true;
    }
  }
  LOG4("Hamster %1 can't move to %2,%3", id, next_x, next_y);
  return false;
}

bool ServerResolver::ApplyDrop(int id, int x, int y, int target_x, int target_y) {
  if (!CheckPosition(x,y) || !CheckPosition(target_x, target_y))
    return false;

  if ( ! g_->_robots[id].HasBall() ) {
    LOG4("Hamster %1 cannot drop the apple, because he doesn't have any !", id);
    return false;
  }
   
  int target_id = GetRobotIdInPos(target_x, target_y);
  if (target_id < 0 && g_->_balls[target_y][target_x] == MAP_EMPTY) {
    LOG4("Hamster %1 dropped apple to cell %2,%3", id, target_x, target_y);
    g_->_robots[id].SetBall(false);
    g_->_balls[target_y][target_x] = MAP_BALL;
    return true;
  }
  if (target_id < 0) {
    LOG4("Hamster %1 cannot drop apple to cell %2,%3, because there is already one in that cell", id, target_x, target_y);
    return false;
  }

  if (target_id >= 0 && g_->_robots[target_id].HasBall()==false) {
    g_->_robots[id].SetBall(false);
    g_->_robots[target_id].SetBall(true);
    LOG4("Apple given to hamster %1 from hamster %2", target_id, id);
    return true;
  }

  LOG4("Cannot give apple to hamster %1 because he has already one", target_id);

  return true;
}

void ServerResolver::ResolveOrderWithDirection(StechecPkt *pkt, int type) {
  
  int id = pkt->arg[0];
  int dir = pkt->arg[3];
  assert(id >= 0 && id < MAX_ROBOTS);
  int x = g_->_robots[id]._pos_x;
  int y = g_->_robots[id]._pos_y;
  //  int expected_id = GetRobotIdInPos(x,y);
  //  LOG4("GetRobotIdInPos(%1,%2) = %3 ; id = ", x, y, expected_id, id);
  if (g_->_robots[id].IsEnabled() == false) {
    LOG4("Warning : hamster %1 is disabled, order ignored", id);
    return;
  }
  assert(GetRobotIdInPos(x,y) == id);

  int next_x = x + _directions[dir][0];
  int next_y = y + _directions[dir][1];

  switch(type) {
  case MOVE :
    ApplyChainMove(dir, id, x, y, next_x, next_y);
    break;
  case DROP_BALL :
    ApplyDrop(id, x, y, next_x, next_y);
    break;
  case HOOK_ROBOT :
    ApplyHook(id, x, y, next_x, next_y);
    break;
  default :
    assert(0);
  }
}

bool ServerResolver::ApplyHook(int id, int x, int y, int target_x, int target_y) {
  if (!CheckPosition(x,y) || !CheckPosition(target_x, target_y)) return false;
  int target_id = GetRobotIdInPos(target_x, target_y);
  if (target_id < 0) {
    LOG4("Hamster %1 cannot throw hook to %2,%3, because there is nobody there", id, target_x, target_y);
    return false;
  }
  g_->_robots[id].SetHook(target_id);
  return true;
}

bool ServerResolver::ApplyTurbo(int id) {
  g_->_robots[id].BoostTurbo();
}

bool ServerResolver::ApplyPickUpBall(int id, int x, int y) {
  if (!CheckPosition(x,y)) return false;
  if (g_->_balls[y][x] != MAP_BALL) {
    LOG4("Hamster %1 cannot take apple in %2,%3, because there is no ball in that cell", id, x, y);
    return false;
  }
  if (g_->_robots[id].HasBall()) {
    LOG4("Hamster %1 cannot take apple beacause he has already one !", id);
    return false;
  }
  g_->_balls[y][x] = MAP_EMPTY;
  g_->_robots[id].SetBall(true);
  return true;
}

void ServerResolver::ResolveSimpleOrder(StechecPkt *pkt, int type) {
  int id = pkt->arg[0];
  assert(id >= 0 && id < MAX_ROBOTS);
  int x = g_->_robots[id]._pos_x;
  int y = g_->_robots[id]._pos_y;
  if (g_->_robots[id].IsEnabled() == false) {
    LOG4("Warning : hamster %1 is disabled, order ignored", id);
    return;
  }
  assert(GetRobotIdInPos(x,y) == id);

  switch(type) {
  case PICK_UP_BALL :
    ApplyPickUpBall(id, x, y);
    break;
  case BOOST_TURBO :
    ApplyTurbo(id);
    break;
  default :
    assert(0);
  }
  
}
