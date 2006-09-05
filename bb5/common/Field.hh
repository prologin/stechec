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



#endif /* !FIELD_HH_ */
