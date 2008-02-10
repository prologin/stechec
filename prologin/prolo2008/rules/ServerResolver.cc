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
}

static void     ResolveMoves(CommandListRef &cmdList)
{
  CommandListRef::iterator iter;

  for (iter = cmdList.begin(); iter != cmdList.end(); ++iter)
  {
    int robot_id = (*iter)->arg[0];
    LOG2("Robot %1 is moving", robot_id);
  }
}

void ServerResolver::ApplyResolver(CommandListRef cmdList[])
{
  // cmdList is an array representing each types of packets.
  // cmdList[PCK_TYPE] evaluates to a list, driven by an iterator.
 

  // First, put the orders in robot_orders, to know wich the order of each robot's orders.

  std::pair<StechecPkt*,int> robot_orders[MAX_ROBOTS][MAX_ORDERS]; // pair of packet and type of packet
  int robots_turn[MAX_ROBOTS];	
  int turn_to_robot[2][MAX_ROBOTS/2];
  int orders_constants[] = {DROP_BALL, MOVE, PICK_UP_BALL, HOOK_ROBOT, RELEASE_HOOK, BOOST_TURBO, WAIT};
  int size_orders_constants = 7;
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
  
  // ResolveMoves(cmdList[MOVE]);
}

void ServerResolver::ResolveOrder(StechecPkt *pkt, int type)
{
  if (!pkt || type < 0) 
    return;

  // todo
  LOG4("Resolving message of type %1, args : %1 %2 %3", type, pkt->arg[0], pkt->arg[1], pkt->arg[2]);

}
