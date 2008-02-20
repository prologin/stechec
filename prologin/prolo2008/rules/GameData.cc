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
  std::fill(***_distances, ***_distances + MAP_MAX_X * MAP_MAX_X * MAP_MAX_Y * MAP_MAX_Y, 1000000000);
  std::fill(*_map, *_map + MAP_MAX_X * MAP_MAX_Y, MAP_EMPTY);
  std::fill(*_balls, *_balls + MAP_MAX_Y * MAP_MAX_X, MAP_EMPTY);
  _nb_robots = 0;
  srand(time(0));
  _start_team = rand() % 2;
}

void GameData::ComputeDistances() {
  LOG4("ComputeDistances");
  int dir[4][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
  
  for (int y0=0 ; y0 < _map_size_y ; y0++)
    for (int x0=0 ; x0 < _map_size_x ; x0++) 
      for (int d=0 ; d < 4 ; d++) {
	int y1 = y0 + dir[d][0];
	int x1 = x0 + dir[d][1];
	if (y1<0 || x1 < 0 || y1 > _map_size_y || x1 > _map_size_x) continue;
	int cur = _map[y0][x0];
	int next = _map[y1][x1];
	if ( cur == next && (cur == MAP_HOLE || cur == MAP_EMPTY) || // case EMPTY->EMPTY or HOLE -> HOLE
	     cur == MAP_EMPTY && next == MAP_HOLE ) // case EMPTY -> HOLE
	  _distances[y0][x0][y1][x1] = 1;
      }
  
    int n = MAP_MAX_X * MAP_MAX_Y;
    // perform a "classical" Floyd-Warshall :
    for (int k=0 ; k < n ; k++)
      for (int i=0 ; i < n ; i++)
	for (int j=0 ; j < n ; j++)  {
	  int iy = i / MAP_MAX_X;
	  int ix = i % MAP_MAX_X;
	  int jy = j / MAP_MAX_X;
	  int jx = j % MAP_MAX_X;
	  int ky = k / MAP_MAX_X;
	  int kx = k % MAP_MAX_X;
	  int &cur = _distances[iy][ix][jy][jx];
	  cur = std::min(cur,  _distances[iy][ix][ky][kx] + _distances[ky][kx][jy][jx]);
	}
}
