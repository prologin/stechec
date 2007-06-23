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

#ifndef TEAM_HH_
# define TEAM_HH_

# include "Constants.hh"
# include "PacketHandler.hh"

/*!
** @struct MsgReroll
** @brief Team reroll use, or result acceptation.
** @ingroup tbt_rules
** Client <-> Server.
*/
DECLARE_PACKET(MSG_REROLL, MsgReroll)
  bool reroll; ///< Whether use a team reroll or accept the result.
END_PACKET;

/*!
** @struct MsgTeamInfo
** @brief Create a Team, at the very beginning of the game.
** @ingroup tbt_rules
** Client <-> Server.
** FIXME: 'race' info is missing. (note: many others too...)
*/
DECLARE_PACKET(MSG_TEAMINFO, MsgTeamInfo)
  int team_name[8];
  int coach_name[8];
  int reroll;
END_PACKET;

/*!
** @struct MsgTouchdooown
** @brief Touchdooown!
** @ingroup tbt_rules
** Server -> Client.
*/
DECLARE_PACKET(MSG_TOUCHDOOOWN, MsgTouchdooown)
  int player_id;  ///< Scoring Player id.
END_PACKET;

/*!
** @brief Base class describing a BB team.
** @ingroup tbt_rules
*/
template <typename T>
class Team
{
public:
  Team(int team_id);
  virtual ~Team();
  
  int getTeamId() const;
  int getNbPlayer() const;
  T* getPlayer(int id);
  const std::string& getTeamName() const;
  const std::string& getCoachName() const;
  int getScore() const;
  int getReroll() const;
  int getRemainingReroll() const;

  //! @brief Check if team has not used reroll during this turn,
  //!  and if it has at least a remaining reroll.
  //! @note It doesn't check if it is team's turn (which is also
  //!  required to use reroll) !
  bool canUseReroll() const;
  void useReroll();
  void initReroll();

  bool hasDoneBlitz() const;
  bool hasDonePass() const;

  void resetTurn();
  bool isPlacementValid() const;

  void incrementScore();
  
protected:
  int team_id_;   ///< Team id (0 or 1).
  T* player_[MAX_PLAYER];  ///< List of this team players.

  std::string team_name_;
  std::string coach_name_;

  int score_;
  int reroll_;
  int reroll_remain_;

  bool blitz_done_;
  bool pass_done_;

  bool reroll_used_;
};  

# include "Team.hxx"

#endif /* !TEAM_HH_ */
