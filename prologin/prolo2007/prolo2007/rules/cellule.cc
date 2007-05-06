#include "Contest.hh"
#include "GameData.hh"
#include "cellule.hh"

static unsigned int id_cell = 0;

Cellule::Cellule(int row, int col, int matiere, GameData *g) :
  Object(row, col), _sante(CELL_STATE_HEALTHY), _matiere(matiere),
  _etat_production_virus(0), _etat_mitose(0), g_(g)
{
  set_id (id_cell++);
}


// Santé
int Cellule::Sante() const
{
  return _sante;
}


bool Cellule::Saine() const
{
  return CELL_STATE_HEALTHY == _sante;
}


bool Cellule::Infectee() const
{
  return CELL_STATE_DEAD != _sante && !Saine();
}


// Quantité de matière restante
int Cellule::Matiere() const
{
  return _matiere;
}


// Division cellulaire
void Cellule::Mitose()
{
  // La mitose est-elle finie ?
  if (_etat_mitose >= MITOSIS_DURATION)
    {
      // Oui : la nouvelle cellule peut etre crée

      // FIXME: Random ou pas ?
      if (g_->rand() % 100 >= MITOSE_RANDOM)
	{
	  LOG1(">>>>>>>>>>>>>>>>>MITOSYS<<<<<<<<<<<<<<<<<<<<<<");
	  // S'il y a la place, la nouvelle cellule est créée (sinon on attend)
	  int dest_row, dest_col;
	  if (g_->TestAround(row, col, &dest_row, &dest_col))
	    {
	      LOG1("I found a new place for my daughter : [%1 ,%2]", dest_row, dest_col);
	      // La division a réussi, on divise la matière par deux (et on se
	      // tape de l'arrondi)
	      _matiere /= 2;
	      _etat_mitose = 0;
	      // FIXED: Créer une cellule avec la moitié de la matière si possible
	      Cellule* new_cell = new Cellule(dest_row, dest_col, _matiere, g_);
	      g_->_cells.push_back(new_cell);
	      LOG1("Mitose : new cell in %1,%2", dest_row, dest_col);
	    }
	  LOG1(">>>>>>>>>>>>>>>>>END MITOSYS<<<<<<<<<<<<<<<<<<<<<<");
	}
    }
  else
    {
      // Non : la mitose continue
      ++_etat_mitose;
    }
}


// Production de virii
void Cellule::Production_virus()
{

}

void	Cellule::PlayTurn ()
{
  int	eated;
  if (_matiere <= 0)
    {
      _sante = CELL_STATE_DEAD;
      LOG1("A cell is dead");
      return;
    }
  // la cellule se nourrit
  eated = g_->nutriments[row][col]->eat (CELL_NUTRIENT * 2);
  _matiere += eated;
  // la cellule utilise ses nutriments pour survivre
  LOG1("I ate : %1 I'm happy", eated);
  _matiere -= CELL_NUTRIENT;
  Mitose ();
}
