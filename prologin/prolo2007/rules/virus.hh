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
// Un virus est un mobile capable d'infectée une (seule) cellule
//
// Lorsqu'un virus infecte une cellule, il ne peut plus rien faire
// d'autre et reste coincé avec son hôte.
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
  // Déplacement
  virtual bool DeplacementPossible(int x, int y);

  // La méthode à appeler à chaque tour
  virtual void PlayTurn();

  virtual void		StopActions ();

  void celluleDied(Cellule *cell);
  
private:
      const int _maladie;
      Cellule	*_hote;
      int _etat_infection;
      int life_;
      int _direction;
      GameData* _g;
};

#endif // VIRUS_HH
