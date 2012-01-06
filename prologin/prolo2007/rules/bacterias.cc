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

#include "GameData.hh"
#include "bacterias.hh"

Bacterias::Bacterias(int row, int col, int nb, GameData* g)
  : Object(row, col),
    g_(g),
    nb_(nb % 5)
{
}

void Bacterias::PlayTurn()
{
  eat();
  spread();
}

int Bacterias::kill(int nb)
{
  if (nb_ < nb)
    nb = nb_;
  nb_ -= nb;
  return nb;
}

void Bacterias::eat()
{
  int n;

  n = g_->nutriments[row][col]->eat(g_->nutriments[row][col]->material());
  for (int i = 0; i < 4; ++i)
    {
      int n_col = col;
      int n_row = row;
      n_row += (i == DEC_Y) ? -1 : ((i == INC_Y) ? 1 : 0);
      n_col += (i == DEC_X) ? -1 : ((i == INC_X) ? 1 : 0);
      if (n_row < 0 || n_col < 0 || n_col >= g_->map_size.col || n_row >= g_->map_size.row)
	continue;
      n += g_->nutriments[n_row][n_col]->eat(g_->nutriments[n_row][n_col]->material() / 3);
    }
  if (nb_ < n)
    nb_ += 1 * g_->rand() % 2;
}

void Bacterias::spread()
{
  if (nb_ < MAX_BACTERY)
    {
      nb_ += 5 ;  // was *= 2, ajout kjus
      return;
    }

  for (int i = 0; i < 4; ++i)
    {
      int n_col = col;
      int n_row = row;

      n_row += (i == DEC_Y) ? -1 : ((i == INC_Y) ? 1 : 0);
      n_col += (i == DEC_X) ? -1 : ((i == INC_X) ? 1 : 0);
      if (n_row < 0 || n_col < 0 || n_col >= g_->map_size.col || n_row >= g_->map_size.row)
	continue;
      if ((g_->getCurrentTurn () % 2) &&
	  g_->terrain_type[n_row][n_col] == VESSEL &&
	  g_->bacterias[n_row][n_col] == 0)
	{
	  if (rand() % 5 == 0)
	    {
	      g_->bacterias[n_row][n_col] = new Bacterias(n_row, n_col,
							  (int)(nb_ * 0.5), g_);
	      LOG4("New bateria seed [%1, %2]", n_row, n_col);
	      nb_ = (int)(nb_ * 0.5);
	    }
	  return;
	}
    }
}
