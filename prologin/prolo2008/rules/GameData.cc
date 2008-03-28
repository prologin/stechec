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
#include <ctime>
#include <cstdlib>

GameData::GameData(void)
{
  std::fill(***_distances, ***_distances + MAP_MAX_X * MAP_MAX_X * MAP_MAX_Y * MAP_MAX_Y, INFINI);
  std::fill(*_map, *_map + MAP_MAX_X * MAP_MAX_Y, MAP_EMPTY);
  std::fill(*_balls, *_balls + MAP_MAX_Y * MAP_MAX_X, MAP_EMPTY);
  std::fill(*_balls_old, *_balls_old + MAP_MAX_Y * MAP_MAX_X, MAP_EMPTY);
  std::fill(_actions_last_turn, _actions_last_turn + MAX_ACTIONS, BAD_ARGUMENT);
  _num_actions_last_turn = 0;
  _nb_robots = 0;
  srand(time(0));
  _start_team = rand() % 2;
}

void GameData::ComputeDistances() {

  //TODO : use the Johnson algorithm (much faster in that case)

  assert(INFINI >= _map_size_y * _map_size_x);
  
  LOG4("ComputeDistances");
  int dir[4][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
  
  for (int y0=0 ; y0 < _map_size_y ; y0++)
    for (int x0=0 ; x0 < _map_size_x ; x0++) {
      _distances[y0][x0][y0][x0] = 0;
      for (int d=0 ; d < 4 ; d++) {
	int y1 = y0 + dir[d][0];
	int x1 = x0 + dir[d][1];
	if (y1<0 || x1 < 0 || y1 >= _map_size_y || x1 >= _map_size_x) continue;
	int cur = _map[y0][x0];
	int next = _map[y1][x1];
	if ( cur == next && (cur == MAP_HOLE || cur == MAP_EMPTY) || // case EMPTY->EMPTY or HOLE -> HOLE
	     cur == MAP_EMPTY && next == MAP_HOLE ) // case EMPTY -> HOLE
	  _distances[y0][x0][y1][x1] = 1;
      }
    }
  
    int n = _map_size_x * _map_size_y;
    // perform a "classical" Floyd-Warshall :
    for (int k=0 ; k < n ; k++)
      for (int i=0 ; i < n ; i++)
	for (int j=0 ; j < n ; j++)  {
	  int iy = i / _map_size_x;
	  int ix = i % _map_size_x;
	  int jy = j / _map_size_x;
	  int jx = j % _map_size_x;
	  int ky = k / _map_size_x;
	  int kx = k % _map_size_x;
	  int &cur = _distances[iy][ix][jy][jx];
	  cur = std::min(cur,  _distances[iy][ix][ky][kx] + _distances[ky][kx][jy][jx]);
	}

}


bool	GameData::makeChecks() {
#ifndef NDEBUG  
  // one robot per cell
  for (int id=0 ; id < MAX_ROBOTS ; id++) {
    if (!_robots[id].IsEnabled())
      continue;
    int x1 = _robots[id]._pos_x;
    int y1 = _robots[id]._pos_y;
    assert( x1 >= 0 && y1 >= 0 && x1 < _map_size_x && y1 < _map_size_y);
    for (int iid=id+1 ; iid < MAX_ROBOTS ; iid++) {
      if (!_robots[iid].IsEnabled())
	continue;
      int x2 = _robots[iid]._pos_x;
      int y2 = _robots[iid]._pos_y;
      assert( x1 != x2 || y1 != y2);
    }
  }

  // ball count is ok :
  static int ball_count=-1;
  int bc = 0;
  for (int i=0 ; i < _map_size_y ; i++)
    for (int j=0 ; j < _map_size_x ; j++)
      bc += (_balls[i][j] == MAP_BALL);
  for (int id=0 ; id < MAX_ROBOTS ; id++)
    bc += _robots[id].HasBall();
  if (ball_count < 0)
    ball_count = bc;
  else
    assert(ball_count == bc);  

#endif
}
