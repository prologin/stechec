/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2006, 2007 The TBT Team.
** 
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
** 
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
** 
** The TBT Team consists of people listed in the `AUTHORS' file.
*/

template <typename T>
inline Field<T>::Field()
{
  for (int i = 0; i < COLS * ROWS; i++)
    tab_[i] = NULL;
}

template <typename T>
inline Field<T>::~Field()
{
}

template <typename T>
inline bool Field<T>::intoField(const Position& pos) const
{
  return pos.row >= 0 && pos.row < ROWS
    && pos.col >= 0 && pos.col < COLS;
}

template <typename T>
inline Position Field<T>::dirToPos(enum eDirection d) const
{
  switch(d)
  {
    case DIR_NORTHWEST: return Position(-1, -1); break;
    case DIR_NORTH:     return Position(-1,  0); break;
    case DIR_NORTHEAST: return Position(-1, +1); break;
    case DIR_EAST:      return Position( 0, +1); break;
    case DIR_SOUTHEAST: return Position(+1, +1); break;
    case DIR_SOUTH:     return Position(+1,  0); break;
    case DIR_SOUTHWEST: return Position(+1, -1); break;
    case DIR_WEST:      return Position( 0, -1); break;
    default:            return Position( 0,  0); break;
  }
}

template <typename T>
inline void Field<T>::setPlayer(const Position& pos, T* p)
{
  // Desynchronization between field and players for position can be
  // _very_ critical.
  if (p != NULL && tab_[pos.row * COLS + pos.col] != NULL)
    WARN("Desynchronization bewteen field and players!");
      
  tab_[pos.row * COLS + pos.col] = p;
}

template <typename T>
inline T* Field<T>::getPlayer(const Position& pos) const
{
	if (!intoField(pos))
		return NULL;
  return tab_[pos.row * COLS + pos.col];
}

template <typename T>
inline int Field<T>::isPlacementValid(int team_id) const
{
  Position p;

  // Check the other half of the field
  for (p.row = (1 - team_id) * (ROWS / 2); p.row <= (1 - team_id) * (ROWS / 2) + (ROWS / 2) - 1; p.row++)
    for (p.col = 0; p.col <= COLS - 1; p.col++)
      if (getPlayer(p) != NULL
	  && getPlayer(p)->getTeamId() == team_id)
        return false;

  int nb_player = 0;
  int nb_player_wide = 0;
  // Check the left wide zone
  for (p.row = team_id * ((ROWS / 2) - 1) + 1; p.row <= team_id * ((ROWS / 2) - 1) + ((ROWS / 2) - 1); p.row++)
    for (p.col = 0; p.col <= SIDE - 1; p.col++)
      if (getPlayer(p) != NULL 
	  && getPlayer(p)->getTeamId() == team_id)
	{
	  nb_player_wide++;
	  nb_player++;
	}
  if (nb_player_wide > 2)
    return false;
	
  nb_player_wide = 0;
  // Check the right wide zone
  for (p.row = team_id * ((ROWS / 2) - 1) + 1; p.row <= team_id * ((ROWS / 2) - 1) + ((ROWS / 2) - 1); p.row++)
    for (p.col = COLS - SIDE; p.col <= COLS - 1; p.col++)
      if (getPlayer(p) != NULL 
	  && getPlayer(p)->getTeamId() == team_id)
	{
	  nb_player_wide++;
	  nb_player++;
	}
  if (nb_player_wide > 2)
    return false;
	
  int nb_player_LoS = 0;
  // Check the LoS
  p.row = team_id + (ROWS / 2) - 1;
  for (p.col = SIDE; p.col <= (COLS - 1) - SIDE; p.col++)
    if (getPlayer(p) != NULL 
	&& getPlayer(p)->getTeamId() == team_id)
      {
	nb_player_LoS++;
	nb_player++;
      }

  // Check the end zone
  p.row = team_id * (ROWS - 1);
  for (p.col = 0; p.col <= COLS - 1; p.col++)
    if (getPlayer(p) != NULL 
	&& getPlayer(p)->getTeamId() == team_id)
      {
	nb_player++;
      }
	
  // Check the middle of the field
  for (p.row = team_id * (ROWS / 2) + 1; p.row <= (team_id + 1) * (ROWS / 2) - 2; p.row++)
    for (p.col = SIDE; p.col <= (COLS - 1) - SIDE; p.col++)
      if (getPlayer(p) != NULL 
	  && getPlayer(p)->getTeamId() == team_id)
	{
	  nb_player++;
	}
	
  // The case where the team has less than 11 players is treated in Team
  return (nb_player < 12 && (nb_player_LoS >= 3 || nb_player_LoS == nb_player));
}

template <typename T>
inline bool Field<T>::hasAdjacentEmptySquare(const Position& pos) const
{
  Position next_square;
  enum eDirection dir;
  for (dir = DIR_LAST; dir = static_cast<enum eDirection>(dir - 1); dir > DIR_UNASSIGNED)
    {
      next_square = pos + dirToPos(dir);
      if (intoField(next_square)
          && getPlayer(next_square) == NULL)
        return true;
    }
  return false;
}

template <typename T>
inline PosList Field<T>::getAdjacentEmptySquares(const Position& pos) const
{
  PosList squares;
  Position next_square;
  enum eDirection dir;
  for (dir = DIR_LAST; dir = static_cast<enum eDirection>(dir - 1); dir > DIR_UNASSIGNED)
    {
      next_square = pos + dirToPos(dir);
      if (intoField(next_square)
          && getPlayer(next_square) == NULL)
        squares.push_back(next_square);
    }
  return squares;
}

template <typename T>
inline bool Field<T>::hasAdjacentPlayer(const Position& pos) const
{
  enum eDirection dir;
  for (dir = DIR_LAST; dir = static_cast<enum eDirection>(dir - 1); dir > DIR_UNASSIGNED)
    {
      if (getPlayer(pos + dirToPos(dir)) != NULL)
        return true;
    }
  return false;
}

template <typename T>
inline bool Field<T>::hasAdjacentPlayer(const Position& pos, enum eStatus s, int team_id) const
{
  T* player;
  enum eDirection dir;
  for (dir = DIR_LAST; dir = static_cast<enum eDirection>(dir - 1); dir > DIR_UNASSIGNED)
    {
      player = getPlayer(pos + dirToPos(dir));
      if (player != NULL
          && player->getStatus() == s
          && player->getTeamId() == team_id)
        return true;
    }
  return false;
}

template <typename T>
inline std::vector<T*> Field<T>::getAdjacentPlayers(const Position& pos) const
{
  std::vector<T*> neighbours;
  T* player;
  enum eDirection dir;
  for (dir = DIR_LAST; dir = static_cast<enum eDirection>(dir - 1); dir > DIR_UNASSIGNED)
    {
      player = getPlayer(pos + dirToPos(dir));
      if (player != NULL)
        neighbours.push_back(player);
    }
  return neighbours;
}

template <typename T>
inline std::vector<T*> Field<T>::getAdjacentPlayers(const Position& pos, enum eStatus s, int team_id) const
{
  std::vector<T*> neighbours;
  T* player;
  enum eDirection dir;
  for (dir = DIR_LAST; dir = static_cast<enum eDirection>(dir - 1); dir > DIR_UNASSIGNED)
    {
      player = getPlayer(pos + dirToPos(dir));
      if (player != NULL
          && player->getStatus() == s
          && player->getTeamId() == team_id)
        neighbours.push_back(player);
    }
  return neighbours;
}

template <typename T>
inline int Field<T>::getNbTackleZones(int team_id, const Position& pos) const
{
  int res = 0;
  Position p;

  for (p.row = pos.row - 1; p.row <= pos.row + 1; p.row++)
    for (p.col = pos.col - 1; p.col <= pos.col + 1; p.col++)
      if (intoField(p)
          && getPlayer(p) != NULL
          && getPlayer(p)->getTeamId() == team_id
	  && getPlayer(p)->getStatus() == STA_STANDING)
        res++;
  return res;
}
