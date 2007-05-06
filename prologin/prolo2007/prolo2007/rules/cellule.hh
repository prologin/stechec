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

  // Quantité de matière restante
  int Matiere() const;

  // Division cellulaire
  void Mitose();

  // Production de virii
  void Production_virus();

  // La méthode à appeler à chaque tour
  virtual void	PlayTurn ();

private:
  int _sante;
  int _matiere;
  int _etat_production_virus;
  int _etat_mitose;
  GameData* g_;
};

#endif // CELLULE_HH
