/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2007 Prologin
*/

#include "GameData.hh"
#include "Server.hh"

GameData::GameData()
{
  last_score_ = 0;
  player_turn = 0;
  max_date = 0;
  cellules_killed_ = 0;
  virus_killed_ = 0;
  bacterias_killed_ = 0;

  for (int y = 0; y < MAX_MAP_SIZE; ++y)
    for (int x = 0; x < MAX_MAP_SIZE; ++x)
      bacterias[y][x] = 0;
  for (int x = 0; x < MAX_MAP_SIZE; x++)
    for (int y = 0; y < MAX_MAP_SIZE; y++)
      {
	nutriments[x][y] = new Nutriments(x, y, 0, this);
	bacterias[x][y] = 0;
      }
  for (int i = 0; i < MAX_TEAM * MAX_PLAYER; ++i)
    {
      players[i].setGameData(this);
      cellules_killed_by_.push_back(0);
      bacterias_killed_by_.push_back(0);
      virus_killed_by_.push_back(0);
      good_cellules_killed_by_.push_back(0);
    }
}

// we cant put this into the destructor, as we aren't sure
// that data will be malloc'ed
void	GameData::FreeData()
{

}


void	GameData::InitMap()
{
  this->chair = 0;
}

int	GameData::GetNextMessage(int id)
{
  if (players[id].getLastMessage() >= (int)messages_.size())
    return UNKNOWN;
  players[id].setLastMessageReaded();
  return players[id].getLastMessage() - 1;
}

unsigned int	GameData::rand()
{
  // Debian original random
  unsigned int res;

  next_ = next_ * 1103515245 + 12345;
  res = (next_ >> 16) & 32767;
  return res;
}

bool GameData::TestVirus (int row, int col)
{
  for (std::vector<Virus*>::iterator i = _virus.begin ();
       i != _virus.end (); ++i)
    if ((*i)->row == row && (*i)->col == col)
      return false;
  return true;
}

bool GameData::TestCell(int row, int col)
{
  for (std::vector<Cellule*>::iterator i = _cells.begin ();
       i != _cells.end (); ++i)
    if ((*i)->row == row && (*i)->col == col)
      return false;
  return true;
}

bool GameData::TestLeucocyte(int row, int col)
{
  for (int i = 0; i < getNbPlayer (); ++i)
    if (players[i].col == col && players[i].row == row &&
	players[i].getState () != STATE_DEAD)
      return false;
  return true;
}

/// Returns true if a box is free, otherwise returns false
/// If a box is free, their coordinates are in dest_row and dest_col
/// Otherwise, these coordinates are NULL

bool GameData::TestAround(int row, int col, int *dest_row, int *dest_col)
{
  for (int i = 0; i < 4; ++i)
    {
      int n_col = col;
      int n_row = row;
      n_row += (i == DEC_Y) ? -1 : ((i == INC_Y) ? 1 : 0);
      n_col += (i == DEC_X) ? -1 : ((i == INC_X) ? 1 : 0);

      if (n_row < 0 || n_col < 0 || n_col == map_size.col || n_row == map_size.row)
	continue;

      if (terrain_type[n_row][n_col] != FLESH)
	{
	  LOG4("Echec Terrain");
	  continue;
	}
      if (!TestVirus (n_row, n_col))
	{
	  LOG4("Echec virus");
	  continue;
	}
      if (!TestCell (n_row, n_col))
	{
	  LOG4("Echec cellule");
	  continue;
	}

      // case validee
      (*dest_row) = n_row;
      (*dest_col) = n_col;
      return true;
    }
  return false;
}


int GameData::knows_type(int type, int id)
{
  return players[id].isKnownDisease(type);
}

bool	GameData::PhagocyteVirus(int id, int y, int x, bool t)
{
  std::vector<Virus*>::iterator it;
  for (it = _virus.begin (); it != _virus.end ()
	 ;++it)
    if ((*it)->col == x && (*it)->row == y)
      {
	if (!t)
	  return true;
	Virus* tmp  = *it;
	_virus.erase (it);
	players[id].Phagocyte (y, x, *tmp);
	return true;
      }
  return false;
}

bool	GameData::PhagocyteCell(int id, int y, int x, bool t)
{
  std::vector<Cellule*>::iterator it;
  for (it = _cells.begin (); it != _cells.end ()
	 ;++it)
    if ((*it)->col == x && (*it)->row == y &&
	(*it)->Sante () != CELL_STATE_DEAD &&
	(*it)->Sante () != CELL_STATE_BEING_PHAGOCYTED)
      {
	if (!t)
	  return true;
	Cellule* tmp  = *it;
	players[id].Phagocyte (y, x, *tmp);
	return true;
      }
  return false;
}

bool	GameData::PhagocyteLeucocyte(int id, int y, int x, bool t)
{
  for (int i = 0; i < getNbPlayer (); ++i)
    if (players[i].col == x && players[i].row == y &&
	players[i].getState () != STATE_DEAD &&
	players[i].getState () != STATE_BEING_PHAGOCYTED)
      {
	if (!t)
	  return true;
	players[id].Phagocyte(y, x, players[i]);
	return true;
      }
  return false;
}

void	GameData::Phagocytose(int id, int y, int x)
{
  if (players[id].getState () == STATE_BEING_PHAGOCYTED)
    return;
  if (PhagocyteVirus (id, y, x))
    return;
  if (PhagocyteCell (id, y, x))
    return;
  if (PhagocyteLeucocyte (id, y, x))
    return;
}

void	GameData::PlayTurn()
{

  /*
  ** Ajout deather
  **
  ** A chaque tour, on memorise le nombre de cellules en vie.
  ** Victor: seulement cote serveur.
  */
  std::vector<Cellule*>::iterator citer;
  std::vector<Virus*>::iterator viter;
  int ccount; // cellules
  int tccount; // total cellules
  int vcount; // viruses
  if (getUid() == -1)
    {
      for (tccount = ccount = 0, citer = _cells.begin(); 
           citer != _cells.end(); 
           ++citer, tccount++)
        {
          if ((*citer)->Saine())
            ccount++;
        }
      _cells_count.push_back(ccount);
      for (vcount = 0, viter = _virus.begin();
           viter != _virus.end();
           ++viter, ++vcount)
        ;
      LOG1("%1 %2 %3 %4", LOG_PATTERN, ccount, tccount - ccount, vcount);
    }

  /*
   * Evolution des nutriments : spread et ajout
   */
  for (int x = 0; x < map_size.row; x++)
    for (int y = 0; y < map_size.col; y++)
      nutriments[x][y]->eat (NEW_NUTRIENT);
  for (int x = 0; x < map_size.row; x++)
    for (int y = 0; y < map_size.col; y++)
      {
	if (terrain_type[x][y] == VESSEL)	// on ne rajoute de
	  // nutiments que dans les vaiseaux
	  nutriments[x][y]->add(NEW_NUTRIENT);
	nutriments[x][y]->spread ();
      }

//   LOG1("Nutriments map");
//   for (int x = 0; x < map_size.row; x++)
//     {
//       for (int y = 0; y < map_size.col; y++)
// 	std::cerr << nutriments[x][y]->material () << "|";
//       std::cerr << std::endl;
//     }
//   LOG1("END Nutriments map");
  /*
   * Les cellules & bactéries "mangent"
   * (prélèvent des nutriments sur leurs cases)
   * Les bactéries jouent en premier
   */

  // bacteries
  int new_seeds = 0; // Nombres de nouveaux foyers de bacteries a ce tour-ci
  for (int y = 0; y < map_size.row; ++y)
    for (int x = 0; x < map_size.col; ++x)
      {
	if (!bacterias[y][x])
	  continue;
	if (bacterias[y][x]->getPopulation () <= 0)
	  {
	    if (bacterias[y][x]->getKilledBy () >= 0)
	      {
		bacterias_killed_by_[bacterias[y][x]->getKilledBy ()] += 1;
		LOG5("Bacterias died because of %1", bacterias[y][x]->getKilledBy ());
	      }
	    delete bacterias[y][x];
	    bacterias[y][x] = NULL;
	    bacterias_killed_++;
	    LOG4("Bacteria [%1, %2] died", y, x);
	  }
	else
	  bacterias[y][x]->PlayTurn ();
      }
  // Cellules
  int n = _cells.size ();
  int j = 0;
  std::vector<Cellule*>::iterator i = _cells.begin();
  while (j < n)
    {
      LOG5("Cellule r: %1, c :%2",
	   _cells[j]->row, _cells[j]->col);
      if (_cells[j]->Sante() != CELL_STATE_DEAD &&
	  _cells[j]->Sante() != CELL_STATE_BEING_PHAGOCYTED)
	_cells[j]->PlayTurn ();
      // on supprime les cellules mortes

      //  ++i;
      j++;
    }
  deleteCells ();

  // Virus
  std::vector<Virus*>::iterator it = _virus.begin();
  while (it != _virus.end())
    if ((*it)->getLife () <= 0)
      {
	Virus* tmp;
	tmp = (*it);
	it = _virus.erase (it);
	if (tmp->getKilledBy () >= 0)
	  {
	    virus_killed_by_[tmp->getKilledBy ()]++;
	    LOG5("Found a virus killed by %1", tmp->getKilledBy ());
	  }
	//	delete tmp;
	virus_killed_++;
	LOG4("A virus died");
      }
    else
      it++;
  for (it = _virus.begin();
       it != _virus.end(); ++it)
    {
      LOG6("Virus %1, [%2, %3]", (*it)->Maladie()
	   ,(*it)->row, (*it)->col);
      (*it)->PlayTurn();
    }
  for (int i = 0; i < getNbPlayer(); ++i)
    if (players[i].getState () == STATE_NORMAL ||
	players[i].getState () == STATE_PHAGOCYTOSING)
      players[i].PlayTurn();

  if (getUid() == -1)
    {
      LOG1("Current Score: %1 -- Currently alive cells: %2",
           this->calculScore(), ccount);
    }

  deleteCells();
}

void	GameData::end()
{
  // This function should only be called server-side
  if (getUid() >= 0)
    return;
    
  LOG1("************************************");
  LOG1("         Match statistics");
  LOG1("====================================");
  int n;
  int nb;
  n = 0;
  LOG1("####################################");
  LOG1("               Cells                ");
  LOG1("####################################");
  LOG1("Total cells alive                 %1", _cells.size ());
  for (int i = 0; i < _cells.size (); ++i)
    if (_cells[i]->Infectee ())
      ++n;
  LOG1("Infected cells                    %1", n);
  n = 0;
  for (int i = 0; i < getNbTeam (); ++i)
    {
      nb = 0;
      LOG1("Team %1", i);
      for (int j = 0; j < getNbPlayer (); ++j)
	if (players[j].get_player () == i)
	  {
	    nb += cellules_killed_by_[j];
	    LOG4("Leucocyte id %1                  :%2", j, cellules_killed_by_[j]);
	  }
      LOG1("------------------------------------");
      n += nb;
    }
  LOG1("Cells killed by the game          %1", cellules_killed_ - n);
  LOG1("                            Total %1", cellules_killed_);

  n = 0;
  LOG1("####################################");
  LOG1("             Bacterias              ");
  LOG1("####################################");
  for (int i = 0; i < getNbTeam (); ++i)
    {
      nb = 0;
      LOG1("Team %1", i);
      for (int j = 0; j < getNbPlayer (); ++j)
	if (players[j].get_player () == i)
	  {
	    nb += bacterias_killed_by_[j];
	    LOG4("Leucocyte id %1                  :%2", j, bacterias_killed_by_[j]);
	  }
      LOG1("------------------------------------");
      n += nb;
    }
  LOG1("Bacterias killed by the game      %1", bacterias_killed_ - n);
  LOG1("                            Total %1", bacterias_killed_);

  n = 0;
  LOG1("####################################");
  LOG1("               Viruses              ");
  LOG1("####################################");
  for (int i = 0; i < getNbTeam (); ++i)
    {
      nb = 0;
      LOG1("Team %1", i);
      for (int j = 0; j < getNbPlayer (); ++j)
	if (players[j].get_player () == i)
	  {
	    nb += virus_killed_by_[j];
	    LOG1("Leucocyte id %1                  :%2", j, virus_killed_by_[j]);
	  }
      LOG1("------------------------------------");
      n += nb;
    }
  LOG1("Viruses killed by the game          %1", virus_killed_ - n);
  LOG1("                              Total %1", virus_killed_);


  LOG1("************************************");
  LOG1("----> Score: %1", last_score_);
}

void	GameData::init()
{
  for (int x = 0; x < map_size.row; x++)
    for (int y = 0; y < map_size.col; y++)
      {
	delete nutriments[x][y];
	nutriments[x][y] = new Nutriments(x, y, terrain_type[x][y] == VESSEL ? NEW_NUTRIENT : 0, this);
      }
  for (int i = 0; i < max_new_seeds; ++i)
    {
      int r;
      int c;
      r = this->rand () % this->map_size.row;
      c = this->rand () % this->map_size.col;
      if (this->bacterias[r][c] == 0 &&
	  this->terrain_type[r][c] == VESSEL)
	{
	  this->bacterias[r][c] = new Bacterias(r, c, 1, this);
	  LOG6("New bacteria seed [%1, %2]", r, c);
	}
      else
	--i;
    }
}


void GameData::deleteCells()
{
  std::vector<Cellule*>::iterator i;
  i = _cells.begin();
  while (i != _cells.end())
    {
      if ((*i)->Sante() == CELL_STATE_DEAD)
	{
	  Cellule* tmp;
	  tmp = (*i);
	  i = _cells.erase(i);
	  if (tmp->getKilledBy () >= 0)
	    {
	      if(tmp->keep_ != CELL_STATE_HEALTHY)
		cellules_killed_by_[tmp->getKilledBy ()]++;
	      else
		good_cellules_killed_by_[tmp->getKilledBy ()]++;
	    }
	  delete tmp;
	  cellules_killed_++;
	}
      else
	++i;
    }
}

#define SCORE_CELL_INFECTED		15
#define SCORE_CELL_NOT_INFECTED		20
#define SCORE_BACTERIA			1
#define SCORE_VIRUS			5

//  How to compute the score
// Called by ServerEntry::getScore()
int	GameData::calculScore()
{
  int	score;
  int	i;
  int	alive_cells_t;
  int	alive_cells_c;
  int	pct;

  std::vector<int>::iterator iter;
  std::vector<Cellule *>::iterator iter2;

  alive_cells_t = 0;
  for (i = 1, iter = _cells_count.begin(); iter != _cells_count.end(); ++iter, i++)
    alive_cells_t += *iter;
  alive_cells_c = 0;
  for (iter2 = _cells.begin(); iter2 != _cells.end(); ++iter2)
    alive_cells_c += ((*iter2)->Saine() ? 1 : 0);
  if (alive_cells_c == 0)
    alive_cells_c++;
  score = (int)(1000.0 * ((((double)alive_cells_t / (this->getCurrentTurn())) + alive_cells_c) / 2.0) / (this->chair));
  // On assigne le meme score a tous les joueurs
  for (i = 0; i < this->getNbPlayer(); ++i)
    players[i].score_ = score;
  last_score_ = score;
  return (score);
}
