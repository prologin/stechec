
#include "Constant.hh"
#include "GameData.hh"
#include "leucocyte.hh"

Leucocyte::Leucocyte() :
  Object (),
  score_(0),
  state_(STATE_NORMAL),
  nb_sent_messages_(0),
  max_messages_sendable_(0),
  last_message_(0),
  phagocytose_turn_(PHAGOCYTOSIS_DURATION),
  v_(0),
  c_(0),
  l_(0),
  g_(0)
{
  for (int i = 0; i < LAST_COMPETENCE; ++i)
    competences[i] = 0;
}

# define MULT_ANTIBODY 5

void	Leucocyte::addAntibody ()
{
  int	nb = competences[ANTIBODY_NB] * MULT_ANTIBODY;

  LOG4("Dropping %1 antibodies", nb);
  antibodies[row][col] += nb;
  for (int i = 0; i < 4; ++i)
    {
      int n_col = col;
      int n_row = row;
      n_row += (i == DEC_Y) ? -1 : ((i == INC_Y) ? 1 : 0);
      n_col += (i == DEC_X) ? -1 : ((i == INC_X) ? 1 : 0);
      if (n_row < 0 || n_row == g_->map_size.row ||
	  n_col < 0 || n_col == g_->map_size.col)
	continue;
      for (int j = 0; j < 4; ++j)
	{
	  int nn_col = n_col;
	  int nn_row = n_row;
	  nn_row += (j == DEC_Y) ? -1 : ((j == INC_Y) ? 1 : 0);
	  nn_col += (j == DEC_X) ? -1 : ((j == INC_X) ? 1 : 0);
	  if (nn_row < 0 || nn_row == g_->map_size.row ||
	      nn_col < 0 || nn_col == g_->map_size.col ||
	      (nn_row == row && nn_col == col))
	    continue;
	  antibodies[nn_row][nn_col] += (unsigned)(0.25 * nb);
	}
      antibodies[n_row][n_col] += (unsigned)(0.5 * nb);
    }
// spread should be done in PlayTurn
}

void	Leucocyte::PlayTurn ()
{
  if (state_ == STATE_DEAD)
    return;
  // antibody attack
  LOG3("Antibody map, player :%1", get_id ());

//   std::stringstream s;
//   s << std::endl;
//   LOG3("g_->map_size :%1", g_->map_size.row);
//   assert(g_->map_size.row <= MAX_MAP_SIZE);
//   assert(g_->map_size.col <= MAX_MAP_SIZE);
//   for (int y = 0; y < g_->map_size.row; ++y)
//     {
//       LOG1("antibody[%1] = %2", y, antibodies+y);
//       for (int x = 0; x < g_->map_size.col; ++x)
// 	{
// 	  LOG1("antibodies[%1][%2] = %3", y, x, antibodies[y]+x);
// 	  s << antibodies[y][x] << "|";
// 	}
//       s << std::endl;
//     }
//   LOG2(s.str().c_str());
  LOG4("Leucocyte %1 antibodies", get_id ());
  for (int y = 0; y < g_->map_size.row; ++y)
    for (int x = 0; x < g_->map_size.col; ++x)
      {
	int n;

	// cellules
	std::vector<Cellule*>::iterator cell;
	for (cell = g_->_cells.begin ();
	     cell != g_->_cells.end (); ++cell)
	  {
	    if ((*cell)->col == x && (*cell)->row == y &&
		(*cell)->Sante () == CELL_STATE_INFECTED &&
		knows_type((*cell)->Maladie ()) == SUCCESS)
	      {
		n = (*cell)->kill (antibodies[y][x]);
		antibodies[y][x] -= (antibodies[y][x] < n) ?
		  antibodies[y][x] : n;
		if ((*cell)->getLife() <=0)
		  (*cell)->killedBy (get_id ());
	      }
	  }
	// virus
	std::vector<Virus*>::iterator it;
	for (it = g_->_virus.begin();
	     it != g_->_virus.end(); ++it)
	  if ((*it)->col == x && (*it)->row == y &&
	      knows_type((*it)->Maladie ()) == SUCCESS)
	    {
	      n = (*it)->kill (antibodies[y][x]);
	      antibodies[y][x] -= (antibodies[y][x] < n) ?
		antibodies[y][x] : n;
	      if ((*it)->getLife() <=0)
		(*it)->killedBy (get_id ());
	    }
	// bacterias

	if (g_->bacterias[y][x] == 0)
	  continue;
	n = g_->bacterias[y][x]->kill (antibodies[y][x]);
	//	g_->bacterias_killed_by_[get_id ()] += n;
	if (g_->bacterias[y][x]->getPopulation () <= 0 && n)
	    g_->bacterias[y][x]->killedBy (get_id ());
	antibodies[y][x] -= (antibodies[y][x] < n) ?
	  antibodies[y][x] : n;
      }
  // spread
  for (int y = g_->map_size.row - 1; y >= 0 ; --y)
    for (int x = g_->map_size.col -1; x >= 0 ; --x)
//       {
// 	for (int i = 0; i < 4; ++i)
// 	  {
// 	    int n_col = x;
// 	    int n_row = y;
// 	    n_row += (i == DEC_Y) ? -1 : ((i == INC_Y) ? 1 : 0);
// 	    n_col += (i == DEC_X) ? -1 : ((i == INC_X) ? 1 : 0);
// 	    //	    LOG3("antibodies[%1,%2] spreading to [%3,%4]", y, x, n_row, n_col);
// 	    if (n_row < 0 || n_row == g_->map_size.row ||
// 		n_col < 0 || n_col == g_->map_size.col)
// 	      continue;
// 	    antibodies[n_row][n_col] += 0.38 * antibodies[y][x];

// 	    //  	    if (antibodies[n_row][n_col] && antibodies[y][x])
// 	    //  	      antibodies[n_row][n_col] = 1;
// 	  }
      if (g_->player_turn % 2)
	antibodies[y][x] = 0;
      else
	antibodies[y][x] /= 2;
//       }
  if (state_ == STATE_PHAGOCYTOSING)
    {
      LOG3("Phagocytose turn %1: %2", get_id (), phagocytose_turn_);
      if (phagocytose_turn_ <= 0)
	{
	  if (l_)
	    {
	      l_->setState (STATE_DEAD);
	      l_->killedBy(get_id ());
	      l_ = 0;
	    }
	  else if (v_)
	    {
	      known_types.push_back (v_->Maladie ());
	      LOG3("Me %1, I've learned : %2", get_id (),v_->Maladie ());
	      v_ = 0;
	      g_->virus_killed_by_[get_id ()]++;
	      delete v_;
	    }
	  else if (c_)
	    {
	      c_->setSante (CELL_STATE_DEAD);
	      c_->killedBy(get_id ());
	      if (c_->keep_ == CELL_STATE_INFECTED)
		{
		  known_types.push_back (c_->Maladie ());
		  LOG3("I've learned : %1",c_->Maladie ());
		}
	      LOG1("Killed a cell :)");
	      c_ = 0;
	    }
	  state_ = STATE_NORMAL;
	}
      phagocytose_turn_ -= competences[PHAGO_SPEED];
    }
}

void	Leucocyte::setGameData (GameData*g)
{
  g_ = g;
  for (int y = 0; y < g_->map_size.row; ++y)
    for (int x = 0; x < g_->map_size.col; ++x)
      antibodies[y][x] = 0;
}

void  Leucocyte::Phagocyte(int y, int x, Virus& v)
{
  Phagocyte ();
  LOG3("Phagocytosing virus");
  v.StopActions ();
  v_ = &v;

}

void  Leucocyte::Phagocyte(int y, int x, Cellule& c)
{
  Phagocyte ();
  LOG3("Phagocytosing cell");
  c.setSante (CELL_STATE_BEING_PHAGOCYTED);
  c.StopActions ();
  c_ = &c;
}

void  Leucocyte::Phagocyte(int y, int x, Leucocyte& l)
{
  Phagocyte ();
  LOG3("Phagocytosing Leucocyte : %1", l.get_id ());
  l.StopActions ();
  l.setState (STATE_BEING_PHAGOCYTED);
  l_ = &l;
}

void		Leucocyte::StopActions ()
{
  if (state_ == STATE_PHAGOCYTOSING)
    {
      if (l_)
	{
	  l_->setState (STATE_NORMAL);
	  l_ = 0;
	}
      else if (v_)
	{
	  g_->_virus.push_back(v_);
	  LOG3("Replacing virus");
	  v_ = 0;
	}
      else if (c_)
	{
	  c_->setSante(c_->keep_);
	  c_ = 0;
	}
    }
}
