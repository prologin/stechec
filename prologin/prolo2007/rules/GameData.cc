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

GameData::GameData ()
{
  player_turn = 0;
  max_date = 0;
  for (int i = 0; i < MAX_TEAM; ++i)
    teams_[i] = -1;
  for (int y = 0; y < MAX_MAP_SIZE; ++y)
    for (int x = 0; x < MAX_MAP_SIZE; ++x)
      bacterias[y][x] = 0;
  _cells.clear ();
  _virus.clear ();
  LOG1("Creating game data, virus vector size is : %1", _virus.size ());
}

// we cant put this into the destructor, as we aren't sure
// that data will be malloc'ed
void	GameData::FreeData ()
{

}


void	GameData::InitMap ()
{
}

int	GameData::GetNextMessage (int id)
{
  if (players[id].GetLastMessage () >= messages_.size ())
    return UNKNOWN;
  players[id].MessageReaded ();
  return players[id].GetLastMessage () - 1;
}

unsigned int	GameData::rand ()
{
  // Debian original random
  next_ = next_ * 1103515245 + 12345;
  return (next_ >> 16) & 32767;
}

int	GameData::GetWrapUid ()
{
  return uidWrapper[getUid ()];
}

/// Returns true if a box is free, otherwise returns false
/// If a box is free, their coordinates are in dest_row and dest_col
/// Otherwise, these coordinates are NULL

bool GameData::TestVirus (int row, int col)
{
  for (std::vector<Virus*>::iterator i = _virus.begin ();
       i != _virus.end (); ++i)
    {
      if ((*i)->row == row && (*i)->col == col)
	return false;
    }
  return true;
}

bool GameData::TestCell (int row, int col)
{
  for (std::vector<Cellule*>::iterator i = _cells.begin ();
       i != _cells.end (); ++i)
    {
      if ((*i)->row == row && (*i)->col == col)
	return false;
    }
  return true;
}

bool GameData::TestLeucocyte (int row, int col)
{
  for (int i = 0; i < getNbPlayer (); ++i)
    if (players[i].col == col && players[i].row == row)
      return false;
  return true;
}

bool GameData::TestAround(int row, int col, int *dest_row, int *dest_col)
{
  for (int i = 0; i < 4; ++i)
    {
      int n_col = col;
      int n_row = row;
      n_row += (i == DEC_Y) ? -1 : ((i == INC_Y) ? 1 : 0);
      n_col += (i == DEC_X) ? -1 : ((i == INC_X) ? 1 : 0);

      if (terrain_type[n_row][n_col] != FLESH)
	{
	  LOG1("Echec Terrain");
	  continue;
	}
      if (!TestVirus (n_row, n_col))
	{
	  LOG1("Echec virus");
	  continue;
	}
      if (!TestCell (n_row, n_col))
	{
	  LOG1("Echec cellule");
	  continue;
	}

      // case validee
      (*dest_row) = n_row;
      (*dest_col) = n_col;
      return true;
    }
  return false;
}

void	GameData::AddPlayer (int team, int uid)
{
  int	i;
  int	j;

  LOG1("Transmitting client with team : %1 and uid :%2", team, uid);
  for (i = 0; i < MAX_TEAM; ++i)
    {
      if (teams_[i] == -1 || teams_[i] == team) // free case
	{
	  teams_[i] = team;
	  for (j = i * MAX_WHITE_CELL; j < (i + 1) * MAX_WHITE_CELL; ++j)
	    {
	      if (players[j].get_id() == -42)
		{
		  players[j].set_id (uid);
		  players[j].set_player (team);
		  players[j].SetRealUid (uid);
		  players[j].setGameData (this);
		  // send messages to client
		  LOG1("Detected player team : %1 and place to play :%2", i, j);
		  return;
		}
	    }
	  assert(j == (i + 1) * MAX_WHITE_CELL);
	}
    }
  assert(i == MAX_TEAM);
}

/*!
** @brief Module description
*/
extern "C" const struct RuleDescription rules_description = {
  "prolo2007",
  "Prologin 2007 final contest rules",
  1,
  0,
};

int GameData::knows_type(int type)
{
  for (std::vector<int>::iterator it = known_types.begin ();
       it != known_types.end (); ++it)
    if (*it == type)
      return 1;
  return 0;
}

void	GameData::PlayTurn ()
{

  /*
   * Evolution des nutriments : spread et ajout
   */
  for (int x = 0; x < map_size.row; x++)
    for (int y = 0; y < map_size.col; y++)
      {
	if (terrain_type[x][y] == VESSEL)	// on ne rajoute de
	  // nutiments que dans les vaiseaux
	  nutriments[x][y]->add(this->rand ());
	nutriments[x][y]->spread ();
      }

  LOG1("Nutriments map");
  for (int x = 0; x < map_size.row; x++)
    {
      for (int y = 0; y < map_size.col; y++)
	std::cout << nutriments[x][y]->material () << "|";
      std::cout << std::endl;
    }
  LOG1("END Nutriments map");
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
	bacterias[y][x]->PlayTurn ();
      }
  // Cellules
  for (std::vector<Cellule*>::iterator i = _cells.begin();
       i != _cells.end(); ++i)
    {
      LOG1("Cellule r: %1, c :%2",
	   (*i)->row, (*i)->col);
      if ((*i)->Sante() != CELL_STATE_DEAD)
	(*i)->PlayTurn ();
      // on supprime les cellules mortes
      if ((*i)->Sante() == CELL_STATE_DEAD)
	{
	  _cells.erase(i);
	  --i;
	}
    }
  // done in playturn
//   int size = this->_cells.size();
//   for (int i = 0; i < size; ++i)
//     _cells[i]->Mitose();
}


void	GameData::init ()
{
   for (int x = 0; x < map_size.row; x++)
     for (int y = 0; y < map_size.col; y++)
       nutriments[x][y] = new Nutriments(x, y, this->rand (), this);
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
	  LOG1("New bacteria seed [%1, %2]", r, c);
	}
      else
	--i;
    }
}
