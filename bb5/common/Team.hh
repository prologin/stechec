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

# include "PacketHandler.hh"

DECLARE_PACKET(MSG_TEAMINFO, MsgTeamInfo)
  int team_name[8];
  int coach_name[8];
  int reroll;
END_PACKET


/*!
** @brief Base class describing a BB team.
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
  
  void resetTurn();
  
protected:
  int team_id_;   ///< Team id (0 or 1).
  T* player_[MAX_PLAYER];  ///< List of this team players.

  std::string team_name_;
  std::string coach_name_;

  int score_;
  int reroll_;

  bool blitz_done_;
  bool pass_done_;

  bool reroll_used_;
};  

# include "Team.hxx"

#endif /* !TEAM_HH_ */
