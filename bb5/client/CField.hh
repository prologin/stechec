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

#include "CPlayer.hh"
#include "Field.hh"

#ifndef CFIELD_HH_
# define CFIELD_HH_


struct ScorePoint;

class CField : public Field<CPlayer>
{
public:
  CField();
  ~CField();

  //! @brief Get path from one point to another.
  //! @return A list of point, describing the path. This list will be
  //!   empty if no path is found.
  //! @note The list is internally allocated and disallocated. Do not try
  //!   to modify it.
  const PosList& getPath(const Position& start,
                         const Position& dest,
                         CPlayer* p);

  //! @brief Get Player at specified position.
  //! @note Temporary, to be deleted when API will be ok.
  const CPlayer* getPlayerConst(const Position& pos) const;
  
private:
  bool          getMinPath(int team_id);
  ScorePoint*   extractMin();
  int           getScoreModifier(const ScorePoint& cur,
                                 const ScorePoint& prev,
                                 int team_id);

  // Variables used by getPath()
  PosList       path_;
  ScorePoint*   path_tab_;
  std::multimap<int, ScorePoint*> cur_pt_list_;
  ScorePoint*   goal_;
};

inline const CPlayer* CField::getPlayerConst(const Position& pos) const
{
  return tab_[pos.row * COLS + pos.col];
}


#endif /* !CFIELD_HH_ */
