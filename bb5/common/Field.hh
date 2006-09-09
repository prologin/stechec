/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2006 The TBT Team.
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

#ifndef FIELD_HH_
# define FIELD_HH_

# include "Constants.hh"

/*!
** Game playing ground.
**
** Parameter T: Player type (CPlayer or SPlayer)
*/
template <typename T>
class Field
{
public:
  Field();
  ~Field();

  //! @brief Check if the specified coordinate are in the field zone
  //!   (ie: not outside the border).
  bool          intoField(const Position& pos) const;

  //! @brief Check if the placement of the team is valid
  int          isPlacementValid(int team_id);

  //! @brief Get the number of tackle zones for the square made by
  //!    a team.
  //! @param team_id The team id of the team that tackle. (ie: The opposing team).
  //! @param pos Position in field to look for.
  int           getNbTackleZone(int team_id, const Position& pos);

  //! @brief Get the player as the specified position, or NULL.
  T*            getPlayer(const Position& pos);

  //! @brief Place a player on the field.
  //! @note Don't forget to manually set NULL its previous position.
  void          setPlayer(const Position& pos, T* p);

protected:
  T*            tab_[COLS * ROWS];
};

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
inline void Field<T>::setPlayer(const Position& pos, T* p)
{
  // Desynchronization between field and players for position can be
  // _very_ critical.
  if (p != NULL && tab_[pos.row * COLS + pos.col] != NULL)
    WARN("Dessynchronization bewteen field and players!");
      
  tab_[pos.row * COLS + pos.col] = p;
}

template <typename T>
inline T* Field<T>::getPlayer(const Position& pos)
{
  return tab_[pos.row * COLS + pos.col];
}

template <typename T>
inline int Field<T>::getNbTackleZone(int team_id, const Position& pos)
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

template <typename T>
inline int Field<T>::isPlacementValid(int team_id)
{
  Position p;

  // Check the other half of the field
  for (p.row = (1 - team_id)*13; p.row <= (1 - team_id)*13 + 12; p.row++)
    for (p.col = 0; p.col <= 14; p.col++)
      if (getPlayer(p) != NULL
	  && getPlayer(p)->getTeamId() == team_id)
	return false;

  int nb_player = 0;
  int nb_player_wide = 0;
  // Check the left wide zone
  for (p.row = team_id*12 + 1; p.row <= team_id*12 + 12; p.row++)
    for (p.col = 0; p.col <= 3; p.col++)
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
  for (p.row = team_id*12 + 1; p.row <= team_id*12 + 12; p.row++)
    for (p.col = 11; p.col <= 14; p.col++)
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
  p.row = team_id + 12;
  for (p.col = 4; p.col <= 10; p.col++)
    if (getPlayer(p) != NULL 
	&& getPlayer(p)->getTeamId() == team_id)
      {
	nb_player_LoS++;
	nb_player++;
      }

  // Check the end zone
  p.row = team_id*25;
  for (p.col = 0; p.col <= 14; p.col++)
    if (getPlayer(p) != NULL 
	&& getPlayer(p)->getTeamId() == team_id)
      {
	nb_player++;
      }
	
  // Check the middle of the field
  for (p.row = team_id*13 + 1; p.row <= team_id*13 + 11; p.row++)
    for (p.col = 4; p.col <= 10; p.col++)
      if (getPlayer(p) != NULL 
	  && getPlayer(p)->getTeamId() == team_id)
	{
	  nb_player++;
	}
	
  // The where the team has less than 11 players is treated in Team
  return (nb_player < 12 &&(nb_player_LoS >= 3 ||nb_player_LoS == nb_player));
}

#endif /* !FIELD_HH_ */
