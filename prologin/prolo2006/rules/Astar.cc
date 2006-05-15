/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#include "Astar.hh"
#include "Server.hh"

Astar::Astar()
{
  parent = 0;
  cur.row = -42;
  cur.col = -42;
  g = 0;
  h = 0;
}

Astar::Astar(int row, int col)
{
  parent = 0;
  cur.row = row;
  cur.col = col;
  g = 0;
  h = 0;
}

Astar::Astar(int row, int col, Astar* p, int g_n, int h_n)
{
  cur.row = row;
  cur.col = col;
  parent = p;
  g = g_n;
  h = h_n;
}

Astar::~Astar()
{}

int	Astar::distance(int row, int col)
{
  return abs(this->cur.row - row) + abs(this->cur.col - col);
}

void	Astar::findShort(std::list<Astar>& l, Astar& res)
{
  // Ici this vaut toujours end: destination
  std::list<Astar>::iterator	res_i;
  std::list<Astar>::iterator	i;
  int				dist = 1600;

  for (i = l.begin(); i != l.end(); ++i)
    if ((*i).g + (*i).h < dist)
      {
	dist = (*i).g + (*i).h;
	res_i = i;
      }
  res = *res_i;
  l.erase(res_i);
}

std::pair<char, short>	Astar::astar_res()
{
  std::pair<char, short> res;
  Astar* tmp = this;

  res.first = 5;
  res.second = 0;
  if (tmp->parent == 0)
    {
      //      LOG3("Arrivee == depart");
      res.first = 4;
    }
  while (tmp->parent != 0)
    {
      res.second++;
      if (tmp->parent->parent == 0)
	{
	  res.first = (tmp->cur.row == tmp->parent->cur.row ?
			(tmp->cur.col > tmp->parent->cur.col ? INC_X : DEC_X) :
			(tmp->cur.row > tmp->parent->cur.row ? INC_Y : DEC_Y));
	  //	  LOG3("rowp : " << tmp->parent->cur.row << " colp : " << tmp->parent->cur.col
	  //<< " row : " << tmp->cur.row << " col : " << tmp->cur.col << " --> dir :"
	  //   << res->first << "||");
	}
      tmp = tmp->parent;
    }
  //  LOG3("Resultat : dist: " << res.second << " and dir: " << res.first);
  return res;
}
