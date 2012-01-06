/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#ifndef ASTAR_HH_
# define ASTAR_HH_

# include "Contest.hh"

class Astar
{
public:
  Astar();
  Astar(int row, int col);
  Astar(int row, int col, Astar* p, int g_n, int h_n);
  ~Astar();
  Astar*	parent;
  Position	cur;
  int		g;
  int		h;

  int		distance(int x, int y);
  void	findShort(std::list<Astar>& l, Astar&);
  std::pair<char, short>	astar_res();
};

#endif // !ASTAR_HH_
