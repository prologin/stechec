/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2007 Prologin
*/

/*
 * Petit outil pour symetriser les maps
 */

#include <fstream>
#include <string>
#include <string>
#include <iostream>
#include <algorithm>

#include "../rules/Constant.hh"

int makeSymetricalMap(std::string in_filename, std::string out_filename)
{
  std::ifstream	map_stream;
  std::ofstream out;
  std::string line;
  int size_y, size_x;

  map_stream.open(in_filename.c_str());
  out.open(out_filename.c_str());
  if (!map_stream || !out)
  {
    std::cerr<<"Unable to load "<<in_filename<<" or "<<out_filename<<std::endl;
    return 1;
  }

  // If the first lines are comments, discard them
  while (map_stream.peek() == '#')
  {
    getline(map_stream, line); 
    out<<line<<std::endl;
  }

  // The first line is "size_x size_y\n"
  if (!(map_stream >> size_x >> size_y))
  {
    std::cerr<<"Expecting size_x and size_y separated by a space"<<std::endl;
    return 1;
  }
  out<<size_x<<" "<<size_y<<std::endl;

  if (size_x <= 4 || size_y <= 4 || size_x > MAP_MAX_X || size_y > MAP_MAX_Y)
  {
    std::cerr<<"Map sizes are invalid, go fix your map"<<std::endl;
    return 1;
  }

  while (map_stream.peek() != '\n')
    map_stream.ignore();
  map_stream.ignore();

  int nb_robots[2] = {0};

  std::string buf[MAP_MAX_Y/2];

  for (int j = 0; j < (size_y+1) / 2; ++j)
  {
    getline(map_stream, line);  
    out<< line<< std::endl;
    if (!map_stream)
    {
      std::cerr<<"Not enough lines in map file"<<std::endl;
      return 1;
    }

    if ((int) line.size() != size_x)
    {
      std::cerr<<"Line "<<j<<" of map has unexpected size "<< line.size() <<" instead of "<< size_x <<std::endl;
      return 1;
    }

    for (int i = 0; i < size_x; ++i)
    { 
      switch (line[i])
      {
        case MAP_ROBOT_TEAM1 :
          if (nb_robots[0] >= MAX_ROBOTS / 2)
          {
	    std::cerr<<"Too many robots for team 0, whose limit is "<< MAX_ROBOTS / 2 << std::endl;
            return 1;
          }
	  nb_robots[0]++;
	  line[i] = MAP_ROBOT_TEAM2;
	  //	  LOG4("Robot in pos %1,%2 for team 0", i, j);
          break;

        case MAP_ROBOT_TEAM2 :
          if (nb_robots[1] >= MAX_ROBOTS / 2)
          {
	    std::cerr<<"Too many robots for team 1, whose limit is "<< MAX_ROBOTS / 2 <<std::endl;
            return 1;
          }
	  nb_robots[1]++;
	  line[i] = MAP_ROBOT_TEAM1;
	    //	  LOG4("Robot in pos %1,%2 for team 1", i, j);
          break;
      case MAP_WALL :
      case MAP_EMPTY :
      case MAP_HOLE :
      case MAP_BALL :
	break;
      case MAP_HOLE_ALTENATE:
	line[i] = MAP_HOLE;
	break;
      default :
	std::cerr<<"Unexpected char in map at pos "<<i<<" "<<j<<" : "<< line[i] <<std::endl;
	return 1;
      }
    }
    buf[j] = line;    
  }
  
  for (int j=0 ; j < size_y / 2 ; ++j) {
    out << buf[size_y / 2 - j - 1] << std::endl;
  } 
  return 0;
}

int main(int argc, char **argv) {

  if (argc < 3) {
    std::cout << "Usage : ./MakeSymMap in.map out.map" << std::endl;	
    return 0;
  }

  makeSymetricalMap(argv[1], argv[2]);

  return 0;
}
