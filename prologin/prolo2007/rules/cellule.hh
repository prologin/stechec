#ifndef CELLULE_HH
#define CELLULE_HH

#include "Contest.hh"
#include "Constant.hh"
#include "Object.hh"

#ifndef YAML
#define CELL_FOOD 20
#endif

#define MITOSE_RANDOM 10

//
// Une cellule est un élément identifiable immobile
//
// La cellule consomme des nutriments, se divise, et produit des virii
// si elle est infectée
//
class Cellule : public Object
{
public:
  Cellule(int row, int col, int matiere, GameData* g);

  // Santé
  int Sante() const;
  bool Saine() const;
  bool Infectee() const;
  void Infect();

  void	setInfection (int i)
  {
    _sante = CELL_STATE_INFECTED;
    keep_ = CELL_STATE_INFECTED;
    LOG3("Infecting cell with deasease %1", i);
    maladie_ = i;
  }

  void	setSante (int s) {_sante = s;}

  // Quantité de matière restante
  int Matiere() const;

  int Maladie () {return maladie_;}

  // Division cellulaire
  void Mitose();

  int	kill (int n)
  {
    if (life_ < n)
      n = life_;
    life_ -= n;
    return n;
  }
  int	getLife () {return life_;}

  // Production de virii
  void Production_virus();

  bool HasACellule(int nrow, int ncol);
  bool HasAVirus(int nrow, int ncol);

  // La méthode à appeler à chaque tour
  virtual void	PlayTurn ();

  virtual void		StopActions ();

  int keep_;
private:
  int _sante;
  int _matiere;
  int _etat_production_virus;
  int _etat_mitose;
  int maladie_;
  int duration_;
  int life_;
  GameData* g_;
};

#endif // CELLULE_HH
