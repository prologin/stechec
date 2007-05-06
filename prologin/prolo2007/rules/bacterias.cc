#include "GameData.hh"
#include "bacterias.hh"


void	Bacterias::eat ()
{
  int n;
  n = g_->nutriments[row][col]->eat (nb_ * BACTERY_NUTRIENT);
  if (nb_ > n)
    nb_ = n;
}

#define BACTERIA_MULT	5

void	Bacterias::spread ()
{
  if (nb_ < MAX_BACTERY)
    {
      nb_ *= 2;
      return;
    }
  for (int i = 0; i < 4; ++i)
    {
      int n_col = col;
      int n_row = row;
      n_row += (i == DEC_Y) ? -1 : ((i == INC_Y) ? 1 : 0);
      n_col += (i == DEC_X) ? -1 : ((i == INC_X) ? 1 : 0);
      if ((g_->rand () % 2) && g_->terrain_type[n_row][n_col] == VESSEL &&
	  g_->bacterias[n_row][n_col] == 0)
	{
	  g_->bacterias[n_row][n_col] = new Bacterias(n_row, n_col, nb_ * 0.5, g_);
	  LOG1("New bateria seed [%1, %2]", n_row, n_col);
	  nb_ *= 0.5;
	  return;
	}
    }
}

void Bacterias::PlayTurn ()
{
  eat ();
  spread ();
}
