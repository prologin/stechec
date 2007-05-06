
#include <iostream>
#include "virus.hh"

static unsigned int id_virus = 0;

Virus::Virus(int row, int col, int maladie) :
  Position(row, col), _maladie(maladie), _hote(0), _etat_infection(0),
  _proteines(42), // FIXME: les prot��nes d�pendent de la maladie
  _direction(rand() % 4),
  _id(id_virus++)
{}


// Type de virus
int Virus::Maladie() const
{
  return _maladie;
}


// Prot�ines de fixation disponibles
int Virus::Proteines() const
{
  return _proteines;
}


// Neutralisation de prot�ines de fixation par des anticorps
int Virus::FixeProteines(int anticorps)
{
  return std::min(anticorps, _proteines);
}


// D�placement
bool Virus::DeplacementPossible(int x, int y)
{
//   return (Mobile::DeplacementPossible(x, y) &&
// 	  (true || true)); // FIXME: absence de cellule en (x, y) OU (_x, _y)
}

// La m�thode � appeler � chaque tour
void Virus::ActionTour()
{

  // Si le virus n'a plus de prot�ine de fixation libre, il est neutralis�
  if (0 == _proteines)
  {
    return;
  }

  // FIXME: comportement du virus
  //
  //  * Le virus infecte une cellule quand il en trouve une non
  //    infect�e. Il ne doit pas pouvoir se ballader de cellule en
  //    cellule : s'il est sur une cellule et qu'il se d�place, il
  //    doit revenir sur une case vide, ou alors il ne bouge pas.
  //
  //  * Est-ce qu'il voit des cellules de loin (cinq cases) ou
  //    est-ce qu'il les rencontre par hasard ?

  // Le virus est-il libre
//   if (0 == _hote)
//   {
//     // Si le virus rencontre une cellule, il l'infecte
//     Cellule * cellule = 0; // FIXME: cellule en (x, y) ou rien
//     if (cellule != 0 && CELL_STATE_HEALTHY == cellule->Sante())
//     {
//       _hote = cellule;
//       _hote->Infecte();
//     }
//     else
//     {
//       // Sinon il vadrouille dans une direction arbitraire, diff�rente
//       // de la direction pr�c�dente
//       int direction = rand() % 3;
//       if (direction >= _direction)
//       {
// 	++direction;
//       }
//       _direction = direction % 4;
//       Deplace(X() + 2 * (_direction % 2) - 1, Y() + 2 * (_direction / 2) - 1);
//     }
//   }
//   else
//   {
//     // L'infection est-elle finie ?
//     if (CELL_STATE_INFECTED == _hote->Sante())
//     {
//       if (_etat_infection >= INFECTION_DURATION)
//       {
// 	// Oui : on transforme la cellule en usine � virii
// 	_hote->InfectionFinie();
//       }
//       else
//       {
// 	// Non : l'infection continue
// 	++_etat_infection;
//       }
//     }
//     // Si l'infection est finie, le virus reste bloqu� dans cet �tat
//     // (dans la r�alit� il a fusionn� avec la cellule h�te).
//   }
}
