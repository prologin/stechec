#ifndef VIRUS_HH
#define VIRUS_HH

#include "Contest.hh"
#include "Object.hh"
#include "Constant.hh"
#include "cellule.hh"

#ifndef YAML
#define VIRUS_PROTEIN 20
#define INFECTION_DURATION 10
#endif

//
// Un virus est un mobile capable d'infect�e une (seule) cellule
//
// Lorsqu'un virus infecte une cellule, il ne peut plus rien faire
// d'autre et reste coinc� avec son h�te.
//
class Virus : public Object
{
public:
  Virus(int row, int col, int maladie, GameData* g);
  virtual ~Virus();

  // Type de virus
  int Maladie() const;

  int getLife () {return life_;}

  int	kill (int n)
  {
    if (life_ < n)
      n = life_;
    life_ -= n;
    return n;
  }
  // D�placement
  virtual bool DeplacementPossible(int x, int y);

  // La m�thode � appeler � chaque tour
  virtual void PlayTurn();

  virtual void		StopActions ();

private:
      const int _maladie;
      Cellule	*_hote;
      int _etat_infection;
      int life_;
      int _direction;
      GameData* _g;
};

#endif // VIRUS_HH
