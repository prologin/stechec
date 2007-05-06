#ifndef VIRUS_HH
#define VIRUS_HH

#include "Contest.hh"
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
class Virus : public Position
{
public:
  Virus(int row, int col, int maladie);

  // Type de virus
  int Maladie() const;

  // Prot�ines de fixation disponibles
  int Proteines() const;
  int FixeProteines(int anticorps);

  // D�placement
  virtual bool DeplacementPossible(int x, int y);

  // La m�thode � appeler � chaque tour
  void ActionTour();

  int get_id() { return _id; }

private:
  const int _maladie;
  Cellule * _hote;
  int _etat_infection;
  int _proteines;
  int _direction;
  int _id;
};

#endif // VIRUS_HH
