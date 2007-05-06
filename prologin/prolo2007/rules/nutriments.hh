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
    LOG1("Setting %1 for nutriment r %2 c %3",
	 nb_, row, col);
  }
  void	add (int n) {nb_ += n % MAX_NUTRIENT;}
  int	eat (int n);
  void	spread ();
  int	material () {return nb_;}
private:
  int	nb_;
  GameData* g_;

};

#endif // !NUTRIMENTS_HH_
