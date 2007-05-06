#include "nutriments.hh"
#include "GameData.hh"
#define SPREADING_RATIO 0.3

int Nutriments::eat (int n)
{
  if (n > nb_)
    n = nb_;
  nb_ -= n;
  return n;
}

void	Nutriments::spread ()
{
  bool	frees[4];
  int	nb_frees = 4;
  float	ratio;

  for (int i = 0; i < 4; ++i)
    frees[i] = true;
  if (nb_ <= 0)
    return;
  if (row == 0)
    {
      frees[DEC_Y] = false;
      nb_frees -= 1;
    }
  if (col == 0)
    {
      frees[DEC_X] = false;
      nb_frees -= 1;
    }
  if (row == g_->map_size.row - 1)
    {
      frees[INC_Y] = false;
      nb_frees -= 1;
    }
  if (col == g_->map_size.col - 1)
    {
      frees[INC_X] = false;
      nb_frees -= 1;
    }

  ratio = (1 - SPREADING_RATIO) / (float)nb_frees;
  for (int i = 0; i < 4; ++i)
    {
      int n_col = col;
      int n_row = row;
      if (frees[i])
	{
	  n_row += (i == DEC_Y) ? -1 : ((i == INC_Y) ? 1 : 0);
	  n_col += (i == DEC_X) ? -1 : ((i == INC_X) ? 1 : 0);
	  //	  LOG1("Spreading <: box [%1, %2] => [%3, %4] : %5", row, col, n_row, n_col, (int)(nb_ * ratio));
	  if (i == INC_Y) // circulation du sang vers le "bas"
	    g_->nutriments[n_row][n_col]->add((int)(nb_ * ratio * 4));
	  else
	    g_->nutriments[n_row][n_col]->add((int)(nb_ * ratio));
	}
    }
  nb_ *= SPREADING_RATIO;
}
