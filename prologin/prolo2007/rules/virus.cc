#include <iostream>
#include "GameData.hh"
#include "virus.hh"

static unsigned int id_virus = 0;

Virus::Virus(int row, int col, int maladie, GameData *g) :
   Object(row, col),
   _maladie(maladie),
   _hote(NULL),
   _etat_infection(0),
   life_(VIRUS_LIFE),
   _g(g)
{
   this->set_id(id_virus++);
}

Virus::~Virus()
{
}

// Type de virus
int Virus::Maladie() const
{
  return _maladie;
}


// Déplacement
bool Virus::DeplacementPossible(int x, int y)
{
//   return (Mobile::DeplacementPossible(x, y) &&
// 	  (true || true)); // FIXME: absence de cellule en (x, y) OU (_x, _y)
}

void Virus::StopActions()
{
  _etat_infection = -1;
  if (_hote)
    _hote->setSante (_hote->keep_);
  _hote = NULL;
}

// La méthode à appeler à chaque tour
void Virus::PlayTurn()
{
  // if (life_ < VIRUS_LIFE)
  //life_ += _g->nutriments[row][col]->eat (2);
  if (_etat_infection == INFECTION_DURATION)
  {
     _hote->Infect();
     _hote->setInfection (_maladie);
     LOG5("/!\\ Cellule Infected (%1, %2) /!\\", _hote->row,
	  _hote->col);
     _hote = NULL;
     _etat_infection = 0;
     life_ = 0;
     return;
  }
  if (_etat_infection > 0)
  {
     _etat_infection++;
     return;
  }

  int cell[4] = { 0 };
  int nb = 0;
  for (int i = 0; i < 4; ++i)
  {
     int nrow = row + ((i == DEC_Y) ? -1 : ((i == INC_Y) ? 1 : 0));
     int ncol = col + ((i == DEC_X) ? -1 : ((i == INC_X) ? 1 : 0));
     if (nrow >= 0 && nrow < _g->map_size.row &&
	 ncol >= 0 && ncol < _g->map_size.col)
	for (std::vector<Cellule*>::iterator it =
		_g->_cells.begin();
	     it != _g->_cells.end(); ++it)
	   if ((*it)->row == nrow && (*it)->col == ncol
	       && (*it)->Sante () != CELL_STATE_BEING_PHAGOCYTED)
	      if (!((*it)->Infectee()) && ++nb)
		 cell[i] = 1;
	      else
		 cell[i] = 2;
  }
  if (nb != 0) // Pas de cellules saine dans les parages
  {
     int n;
     do
	n = _g->rand() % 4;
     while (cell[n] != 1);
     int nrow = row + ((n == DEC_Y) ? -1 : ((n == INC_Y) ? 1 : 0));
     int ncol = col + ((n == DEC_X) ? -1 : ((n == INC_X) ? 1 : 0));
     for (std::vector<Cellule*>::iterator it =
	     _g->_cells.begin();
	  it != _g->_cells.end(); ++it)
	if ((*it)->row == nrow && (*it)->col == ncol)
	  {
	    _hote = (*it);
	    _hote->setSante(CELL_STATE_BEING_INFECTED);
	  }
     _etat_infection = 1;
  }
  else
  {
     // Move aleatoire
     int nrow, ncol, n;
     int tab[4] = {0};
     int nb = 0;
     do
     {
       if (nb == 4)
	 return;
	n = _g->rand() % 4;
	nrow = row + ((n == DEC_Y) ? -1 : ((n == INC_Y) ? 1 : 0));
	ncol = col + ((n == DEC_X) ? -1 : ((n == INC_X) ? 1 : 0));
	if (tab[n] == 0)
	  {
	    tab[n] = 1;
	    nb++;
	  }
     }
     while (nrow < 0 || nrow >= _g->map_size.row ||
	    ncol < 0 || ncol >= _g->map_size.col ||
	    cell[n] == 2 /* Cellule infectée */  || !_g->TestLeucocyte(nrow, ncol)
	    || !_g->TestVirus (nrow, ncol));
     LOG3("Virus decided to move to %1, %2", nrow, ncol);
     row = nrow;
     col = ncol;
  }
}
