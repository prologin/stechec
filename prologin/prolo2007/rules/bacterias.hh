#ifndef BACTERIAS_HH_
# define BACTERIAS_HH_

#include <tools.hh>

class GameData;

class Bacterias : public Object
{
public:
  Bacterias(int row, int col, int nb, GameData* g):
    Object(row, col),
    g_(g),
    nb_(nb % 5)
  {}
  virtual void PlayTurn ();
private:
  void	eat ();
  void	spread (); // Only in the vessels
  GameData* g_;
  int nb_;
};

#endif // !BACTERIAS_HH_
