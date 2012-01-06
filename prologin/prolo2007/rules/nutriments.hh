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

#ifndef NUTRIMENTS_HH_
# define NUTRIMENTS_HH_

#include "Constant.hh"
#include <tools.hh>

class GameData;

class Nutriments : public Position
{
public:
  Nutriments (int row, int col, int nb, GameData *g):
    Position (row, col),
    nb_(nb % MAX_NUTRIENT),
    g_(g)
  {
  }
  void	add (int n) {nb_ += n;}
  int	eat (int n);
  void	spread ();
  int	material () {return nb_;}
private:
  int	nb_;
  GameData* g_;

};

#endif // !NUTRIMENTS_HH_
