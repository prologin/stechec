#include "Constant.hh"
#include "GameData.hh"
#include "leucocyte.hh"

Leucocyte::Leucocyte () :
  Object (),
  score_(0),
  state_(STATE_NORMAL),
  nb_sent_messages_(0),
  max_messages_sendable_(0),
  last_message_(0)
{
  for (int i = 0; i < LAST_COMPETENCE; ++i)
    competences[i] = 0;
}

# define MULT_ANTIBODY 5

void	Leucocyte::addAntibody ()
{
  int	nb = competences[ANTIBODY_NB] * MULT_ANTIBODY;

  antibodies[row][col] += nb;
  // spread should be donne in PlayTurn
}

void	Leucocyte::PlayTurn ()
{
  // FIXME
  // decrement antibody
  for (int y = 0; g_->map_size.row; ++y)
    for (int x = 0; g_->map_size.col; ++x)
      antibodies[row][col] *= 0.6; // avoid surpopulation of antibodies.
  // spread
}

void	Leucocyte::setGameData (GameData*g)
{
  g_ = g;
  for (int y = 0; y < g_->map_size.row; ++y)
    for (int x = 0; x < g_->map_size.col; ++x)
      antibodies[y][x] = 0;
}
