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

struct ScorePoint;

/*!
** Game playing ground.
*/
template <typename T>
class Field
{
  typedef T PlayerType;
  
public:
  Field();
  ~Field();

  //! @brief Check if the specified coordinate are in the field zone
  //!   (ie: not outside the border).
  bool          intoField(const Position& pos) const;

  //! @brief Get the player as the specified position, or NULL.
  T*            getPlayer(const Position& pos);
  //! @brief Same as getPlayer, but const.
  const T*      getPlayerConst(const Position& pos) const;

  //! @brief Get path from one point to another.
  //! @return A list of point, describing the path. This list will be
  //!   empty if no path is found.
  //! @note The list is internally allocated and disallocated. Do not try
  //!   to modify it.
  const PosList& getPath(const Position& start,
                         const Position& dest,
                         T* p);

  //! @brief Get the number of tackle zones for the square made by
  //!    a team.
  //! @param team_id The team id of the team that tackle. (ie: The opposing team).
  //! @param pos Position in field to look for.
  int           getNbTackleZone(int team_id, const Position& pos);

  //! @brief Place a player on the field.
  //! @note Don't forget to manually set NULL its previous position.
  void          setPlayer(const Position& pos, T* p);

private:
  T**           tab_;

  ScorePoint*   extractMin();
  int           getScoreModifier(const ScorePoint& cur,
                                 const ScorePoint& prev,
                                 int team_id);
  bool          getMinPath(int team_id);
  
  // Variables used by getPath()
  PosList       path_;
  ScorePoint*   path_tab_;
  std::multimap<int, ScorePoint*> cur_pt_list_;
  ScorePoint*   goal_;
};

template <typename T>
inline bool Field<T>::intoField(const Position& pos) const
{
  return pos.row >= 0 && pos.row < ROWS
    && pos.col >= 0 && pos.col < COLS;
}

template <typename T>
inline void Field<T>::setPlayer(const Position& pos, T* p)
{
  tab_[pos.row * COLS + pos.col] = p;
}

template <typename T>
inline T* Field<T>::getPlayer(const Position& pos)
{
  return tab_[pos.row * COLS + pos.col];
}

template <typename T>
inline const T* Field<T>::getPlayerConst(const Position& pos) const
{
  return tab_[pos.row * COLS + pos.col];
}

#endif /* !FIELD_HH_ */
