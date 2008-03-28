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

#include "ClientDiffer.hh"

ClientDiffer::ClientDiffer(GameData* game, Client* client)
  : StechecClientDiffer(game, client)
{
}

void ClientDiffer::ApplyDiff(const StechecPkt *pkt)
{
  
  switch (pkt->type)
    {
    case INIT_DATA :
      {
	switch(pkt->arg[0]) {
	case INIT_MAP_SIZE :
	  {
	    // Client received the map size
	    int size_x = pkt->arg[1];
	    int size_y = pkt->arg[2];
	    LOG4("%1,%2", size_x, size_y);
	    assert(size_x >= 4 && size_y >= 4 && size_x <= MAP_MAX_X && size_y <= MAP_MAX_Y);
	    g_->_map_size_x = size_x;
	    g_->_map_size_y = size_y;
	    LOG4("Receiving map size %1 %2", size_x, size_y);
	    break;
	  }
	case INIT_ROBOT :
	  {
	    int id = pkt->arg[1];
	    int x = pkt->arg[2];
	    int y = pkt->arg[3];
	    int t = pkt->arg[4];
	    assert(id >= 0 && id < MAX_ROBOTS);
	    assert(x >= 0 && y >= 0 && x < MAP_MAX_X && y < MAP_MAX_Y);
	    assert(t==0 || t==1);
	    if (!g_->_robots[id].IsEnabled())
	      g_->_nb_robots++;
	    g_->_robots[id].Init(x,y,t);
	    break;
	  }
	case INIT_START_TEAM :
	  {
	    int start_team = pkt->arg[1];
	    assert(start_team == 0 || start_team == 1);
	    g_->_start_team = start_team;
	    break;
	  }
	default :
	  assert(0);
	}
	break;
      }

      // Client received map data
    case MAP_CONTENT :
      {
	int x = pkt->arg[0];
	int y = pkt->arg[1];
	int c = pkt->arg[2];
      
	assert(g_->_map_size_y != -1);
	assert(x >= 0 && y >= 0 && x < g_->_map_size_x && y < g_->_map_size_y);
	//	LOG4("Received map content in %1 %2 : %3", x ,y, c);
	g_->_map[y][x] = (unsigned char) c;
	break;
      }
      //client received data for the "balls" map
    case BALLS_CONTENT :
      {
	int x = pkt->arg[0];
	int y = pkt->arg[1];
	int c = pkt->arg[2];
	assert(g_->_map_size_y != -1);
	assert( x>= 0 && y >= 0 && x < g_->_map_size_x && y < g_->_map_size_y);
	assert(c == MAP_EMPTY || c == MAP_BALL);
	//	LOG4("Received balls map content in %1 %2 : %3", x, y, c);
	g_->_balls[y][x] = (unsigned char) c;
	break;
      }
      // Client received position of robots
    case ROBOT_POS:
      {
        int robot_id = pkt->arg[0];
        int x        = pkt->arg[1];
        int y        = pkt->arg[2];
	LOG4("Received position of robot %1 : %2,%3", robot_id, x, y);
        if (robot_id < 0 || robot_id >= MAX_ROBOTS)
	  {
	    ERR("Robot Id %1: invalid range in ROBOT_POS", robot_id);
	    break;
	  }
        else if (!g_->_robots[robot_id].IsEnabled())
	  {
	    ERR("Robot Id %1: try to use a disabled robot in ROBOT_POS", robot_id);
	    break;
	  }
        g_->_robots[robot_id].SetPos(x, y);
        break;
      }

      // Client received which robot got the ball
    case ROBOT_HAS_BALL:
      {
        int robot_id = pkt->arg[0];
	LOG4("Received robot_has_ball for robot %1 : %2", robot_id, pkt->arg[1]);
        if (Robot::CheckRange(robot_id))
	  {
	    ERR("Robot Id %1: invalid range in ROBOT_HAS_BALL", robot_id);
	    break;
	  }
        else if (!g_->_robots[robot_id].IsEnabled())
	  {
	    ERR("Robot Id %1: try to use a disabled robot in ROBOT_HAS_BALL", robot_id);
	  }
        if (pkt->arg[1])
          g_->_robots[robot_id].PickUpBall();
        else
          g_->_robots[robot_id].ReleaseBall();
        break;
      }

      // Client received which robot hooks who
    case ROBOT_HOOK:
      {
        int robot_id        = pkt->arg[0];
        int hooked_robot_id = pkt->arg[1];

        if (Robot::CheckRange(robot_id) || Robot::CheckRange(hooked_robot_id))
	  {
	    ERR("Robot Id %1, HookedRobot id %2: invalid range in ROBOT_HOOK", robot_id, hooked_robot_id);
	    break;
	  }
        else if (!g_->_robots[robot_id].IsEnabled())
	  {
	    ERR("Robot Id %1: try to used a disabled robot in ROBOT_HOOK", robot_id);
	    break;
	  }
        else if (!g_->_robots[hooked_robot_id].IsEnabled())
	  {
	    ERR("HookedRobot Id %1: try to use a disabled robot in ROBOT_HOOK", hooked_robot_id);
	    break;
	  }
        else if (robot_id == hooked_robot_id)
	  {
	    LOG1("Robot, HookedRobot: have the same Id %1 in ROBOT_HOOK", robot_id);
	    break;
	  }
        g_->_robots[robot_id].SetHook(hooked_robot_id);
        break;
      }

      // Client received which robot reset its hook
    case ROBOT_UNHOOK:
      {
        int robot_id = pkt->arg[0];

        if (Robot::CheckRange(robot_id))
	  {
	    ERR("Robot Id %1: invalid range in ROBOT_UNHOOK", robot_id);
	    break;
	  }
        else if (!g_->_robots[robot_id].IsEnabled())
	  {
	    ERR("Robot Id %1: try to use a disabled robot in ROBOT_UNHOOK", robot_id);
	    break;
	  }
        g_->_robots[robot_id].ResetHook();
        break;
      }

    case ACTION_LAST_TURN :
      {
	int n = pkt->arg[0];
	int action = pkt->arg[1];
	assert(n >= 0 && n < MAX_ACTIONS);
	if (action == ACTION_INIT) {
	  std::fill(g_->_actions_last_turn, g_->_actions_last_turn + g_->_num_actions_last_turn, BAD_ARGUMENT);
	  g_->_num_actions_last_turn = 0;
	} else {
	  g_->_actions_last_turn[n] = action;
	  g_->_num_actions_last_turn = std::max(g_->_num_actions_last_turn, n+1);
	  LOG4("Received action of last turn : %1", action);
	}
	break;
      }

      // Unexpected packet
    default:
      {
        LOG2("Differ get unexpected message, id: %1", pkt->type);
	assert(0);
      }
    }
}
