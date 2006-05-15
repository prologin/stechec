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

#ifndef SPLAYER_HH_
# define SPLAYER_HH_

# include "Team.hh"
# include "Player.hh"

class SRules;

class SPlayer : public Player
{
public:
  SPlayer(int id, int team_id, SRules* r);

  // These function give an int that evaluates their results.
  int doMove(const ActMove* m);
  int doBlock(const ActBlock* m);
  int doPass(const ActPass* m);

  void msgPlayerInfo(const MsgPlayerInfo* m);

  void setPosition(const Position& pos);

private:
  // Related action.
  bool tryAction(int modifier);
  bool pickBall(int modifier);

  //! @brief Check for armor and eventually injury.
  void checkArmor(int arMod, int injMod);
  //! @brief Roll for injury.
  void rollInjury(int injMod);
  //! @brief Roll for casualty.
  int rollCasualty();
  
  SRules* r_;
};


#endif /* !SPLAYER_HH_ */
