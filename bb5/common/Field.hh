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

#ifndef FIELD_HH_
# define FIELD_HH_

/*!
** @file common/Field.hh
** @ingroup rules
*/

# include "Constants.hh"

//! @brief Scatter template.
//! @note It is slightly different from BB's template.
//!  123
//!  8B4
//!  765
enum eDirection {
  DIR_UNASSIGNED = 0,

  DIR_NORTHWEST,
  DIR_NORTH,
  DIR_NORTHEAST,
  DIR_EAST,
  DIR_SOUTHEAST,
  DIR_SOUTH,
  DIR_SOUTHWEST,
  DIR_WEST,

  DIR_LAST = DIR_WEST
};

/*!
** @ingroup rules
** @brief Game playing ground.
**
** Parameter T: Player type (CPlayer or SPlayer)
*/
template <typename T>
class Field
{
public:
  Field();
  virtual ~Field();

  //! @brief Check if the specified coordinate are in the field zone
  //!   (ie: not outside the border).
  bool          intoField(const Position& pos) const;

  //! @brief Convert a cardinal point direction to a Position.
  Position dirToPos(enum eDirection dir) const;

  //! @brief Place a player on the field.
  //! @note Don't forget to manually set NULL its previous position.
  void          setPlayer(const Position& pos, T* p);

  //! @brief Get the player at the specified position, or NULL.
  T*            getPlayer(const Position& pos) const;

  //! @brief Check if the placement of the team is valid
  int           isPlacementValid(int team_id) const;

  //! @brief Check if there is an empty square near the given position.
  bool          hasAdjacentEmptySquare(const Position& pos) const;

  //! @brief Get the players surrounding the given position.
  PosList       getAdjacentEmptySquares(const Position& pos) const;
  
  //! @brief Check if there is a player near the given position.
  bool          hasAdjacentPlayer(const Position& pos) const;

  //! @brief Check if there is a player of the specified team near the given position.
  bool          hasAdjacentPlayer(const Position& pos, enum eStatus s, int team_id) const;
  
  //! @brief Get the players surrounding the given position.
  std::vector<T*>  getAdjacentPlayers(const Position& pos) const;
  
  //! @brief Get the players of the specified team surrounding the given position.
  std::vector<T*>  getAdjacentPlayers(const Position& pos, enum eStatus s, int team_id) const;

  //! @brief Get the number of tackle zones for the square made by a team.
  //! @param team_id The team id of the team that tackles. (ie: The opposing team).
  //! @param pos Position in field to look for.
  int           getNbTackleZones(int team_id, const Position& pos) const;

protected:
  T*            tab_[COLS * ROWS];
};

# include "Field.hxx"

#endif /* !FIELD_HH_ */
