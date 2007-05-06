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

#ifndef GAMEDATA_HH_
# define GAMEDATA_HH_

# include <list>
# include <map>
# include "Contest.hh"
# include "Constant.hh"

// List of includes
#include "virus.hh"
#include "cellule.hh"
#include "leucocyte.hh"
#include "nutriments.hh"
#include "bacterias.hh"

/*!
** This class is meant to contain all data, accessible from
** everywhere.
*/
class GameData: public StechecGameData
{
public:

  GameData();

  void	init();
  void	end();

  unsigned int	rand();
  void		srand(int s) {next_ = s;}

  struct Position	map_size;
  unsigned		player_turn; ///< Actual Date.
  int			max_date;

  uint8_t		terrain_type[MAX_MAP_SIZE][MAX_MAP_SIZE];
  // uint8_t		terrain_graphic[MAX_MAP_SIZE][MAX_MAP_SIZE];
  Leucocyte		players[MAX_TEAM * MAX_PLAYER];

  Nutriments		*nutriments[MAX_MAP_SIZE][MAX_MAP_SIZE];
  Bacterias		*bacterias[MAX_MAP_SIZE][MAX_MAP_SIZE];

  Leucocyte		*p;

  std::vector<Cellule*>	_cells;

  std::vector<Virus*>	_virus;

  int			max_new_seeds; //nombre de nouveau foyer de bacterie max par tour


  void			turnAction();

  void			InitMap();
  void			FreeData();

  void			PlayTurn(); /// used to factorize code for server and client Entry

  void			deleteCells ();

    /** Mitose Section **/
  bool TestAround(int row, int col, int *dest_row, int *dest_col);

  bool TestVirus(int row, int col);

  bool TestLeucocyte(int row, int col);

  bool TestCell(int row, int col);

  void	Phagocytose(int id, int y, int x);

  bool	PhagocyteVirus(int id, int y, int x, bool t = true);
  bool	PhagocyteCell(int id, int y, int x, bool t = true);
  bool	PhagocyteLeucocyte(int id, int y, int x, bool t = true);

  /** Messages Section **/
  int	GetNextMessage(int id);
  int	MessageExists(int msg_id) {return msg_id < (int)messages_.size(); }
  int	GetMessage(int msg_id, int arg_id) {return messages_[msg_id][arg_id];}
  void	AddMessage(int a, int b, int c, int d)
  {
    int	*t = new int[4];
    t[0] = a;
    t[1] = b;
    t[2] = c;
    t[3] = d;
    messages_.push_back(t);
  }
  int	knows_type (int type, int id);
  int   cellules_killed_;
  int   bacterias_killed_;
  int   virus_killed_;
  std::vector<int> cellules_killed_by_;
  std::vector<int> good_cellules_killed_by_;
  std::vector<int> bacterias_killed_by_;
  std::vector<int> virus_killed_by_;
private:
  unsigned int		next_;
  std::vector<int*>	messages_;
};

#endif // !GAMEDATA_HH_
