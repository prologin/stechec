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


#ifndef BACTERIAS_HH_
# define BACTERIAS_HH_

#include <tools.hh>

#define BACTERIA_MULT	5


class GameData;

class Bacterias : public Object
{
public:
  Bacterias(int row, int col, int nb, GameData* g);

  virtual void PlayTurn();
  virtual void StopActions() {}

  int	kill(int nb);
  int	getPopulation() { return nb_; }

private:
  void	eat();
  void	spread(); // Only in the vessels

  GameData*  g_;
  int nb_;
};

#endif // !BACTERIAS_HH_
