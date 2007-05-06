#include "Contest.hh"
#include "GameData.hh"
#include "cellule.hh"

static unsigned int id_cell = 0;

Cellule::Cellule(int row, int col, int matiere, GameData *g) :
  Object(row, col), _sante(CELL_STATE_HEALTHY), _matiere(matiere),
  _etat_production_virus(0), _etat_mitose(0), g_(g), maladie_ (-1),
  duration_(-1), life_(CELL_LIFE), keep_(CELL_STATE_HEALTHY)
{
  set_id (id_cell++);
}

int Cellule::Sante() const
{
  return _sante;
}

void Cellule::Infect()
{
  _sante = CELL_STATE_INFECTED;
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

void	Cellule::StopActions ()
{
  _etat_mitose = 0;
  duration_ = -1;
}


// Division cellulaire
void Cellule::Mitose()
{
  // La mitose est-elle finie ?
  if (_etat_mitose >= MITOSIS_DURATION)
    {
      // Oui : la nouvelle cellule peut etre crée

      if (_matiere / 2 < CELL_NUTRIENT * 2)
	return;
      if (g_->rand() % 100 >= MITOSE_RANDOM)
	{
	  LOG3(">>>>>>>>>>>>>>>>>MITOSYS<<<<<<<<<<<<<<<<<<<<<<");
	  // S'il y a la place, la nouvelle cellule est créée (sinon on attend)
	  int dest_row, dest_col;
	  if (g_->TestAround(row, col, &dest_row, &dest_col))
	    {
	      LOG3("I found a new place for my daughter : [%1 ,%2]", dest_row, dest_col);
	      // La division a réussi, on divise la matière par deux (et on se
	      // tape de l'arrondi)
	      _matiere /= 2;
	      _etat_mitose = 0;
	      Cellule* new_cell = new Cellule(dest_row, dest_col, _matiere, g_);
	      // Si la cellule est infectée, elle crée une fille infectée
	      if (_sante == CELL_STATE_INFECTED)
		{
		  new_cell->Infect();
		  new_cell->setInfection (maladie_);
		}
	      g_->_cells.push_back(new_cell);
	      LOG3("Mitose : new cell in %1,%2", dest_row, dest_col);
	    }
	  LOG3(">>>>>>>>>>>>>>>>>END MITOSYS<<<<<<<<<<<<<<<<<<<<<<");
	}
    }
  else
    {
      // Non : la mitose continue
      ++_etat_mitose;
    }
}

bool Cellule::HasACellule(int nrow, int ncol)
{
  for (std::vector<Cellule*>::iterator it =
	 g_->_cells.begin();
       it != g_->_cells.end(); ++it)
    if ((*it)->row == nrow && (*it)->col == ncol)
      return true;
  return false;
}

bool Cellule::HasAVirus(int nrow, int ncol)
{
  for (std::vector<Virus*>::iterator it =
	 g_->_virus.begin();
       it != g_->_virus.end(); ++it)
    if ((*it)->row == nrow && (*it)->col == ncol)
      return true;
  return false;
}

// Production de virii
void Cellule::Production_virus()
{
  if (duration_ == -1)
    duration_ = 0;
  ++duration_;
  if (duration_ < VIRUS_PRODUCTION_DURATION)
    return;
  if (_matiere < VIRUS_MATERIAL + CELL_NUTRIENT)
    return;
  duration_ = -1;
  int tab[4] = { 0 };
  int n = 0;
  // Case disponible pour le nouveau virus
  for (int i = 0; i < 4; ++i)
    {
      int nrow = row + ((i == DEC_Y) ? -1 : ((i == INC_Y) ? 1 : 0));
      int ncol = col + ((i == DEC_X) ? -1 : ((i == INC_X) ? 1 : 0));
      if (nrow >= 0 && nrow < g_->map_size.row &&
	  ncol >= 0 && ncol < g_->map_size.col)
	{
	  if (HasACellule(nrow, ncol))
	    continue;
	  else if (HasAVirus(nrow, ncol))
	    continue;
	  else if (!g_->TestLeucocyte(nrow, ncol))
	    continue;
	  tab[i] = 1;
	  ++n;
	}
    }
  // Pas de cases dispo
  if (n == 0)
    return;
  int pos;
  do
    pos = g_->rand() % 4;
  while (tab[pos] == 0);
  int nrow = row + ((pos == DEC_Y) ? -1 : ((pos == INC_Y) ? 1 : 0));
  int ncol = col + ((pos == DEC_X) ? -1 : ((pos == INC_X) ? 1 : 0));

  Virus* new_vir = new Virus(nrow, ncol, maladie_, g_);
  _matiere -= VIRUS_MATERIAL;
  LOG3("Sommoning new virus type %1, [%2, %3]", maladie_, nrow, ncol);
  g_->_virus.push_back(new_vir);
}

void	Cellule::PlayTurn ()
{
  int	eated;
  if (_matiere <= 0 || life_ <= 0)
    {
      _sante = CELL_STATE_DEAD;
      LOG1("A cell is dead");
      return;
    }

  eated = g_->nutriments[row][col]->eat (CELL_NUTRIENT * 2);
  _matiere += eated;
  // la cellule utilise ses nutriments pour survivre
  LOG1("I ate : %1 I'm happy", eated);
  // la cellule consomme plus de nutriment si infectee
  _matiere -= CELL_NUTRIENT * (Sante () == CELL_STATE_INFECTED ? 1.5 : 1);

  if (_sante == CELL_STATE_INFECTED)
    {
      if (!_etat_mitose && (duration_ >= 0 || g_->rand () % 3 != 0))
	{
	  LOG3("duration : %1", duration_);
	  Production_virus();
	}
      else
	{
	  LOG3("Infected Mitosis %1", _etat_mitose);
	  Mitose ();
	}
    }
  else
    {
      Mitose ();
      LOG1("End Mitose");
    }

}
