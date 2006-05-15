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

// Hard limit of move number, for a single packet.
const int MAX_MOVE = 16;

//FIXME : arbitrary length team and coach name
DECLARE_PACKET(MSG_TEAMINFO, MsgTeamInfo)
  int team_name[8];
  int coach_name[8];
  int reroll;
END_PACKET

DECLARE_PACKET(ACT_MOVE, ActMove)
  int player_id;
  int nb_move;
  struct {
    int row;
    int col;
  } moves[MAX_MOVE];
END_PACKET

    //block messages are location based
DECLARE_PACKET(ACT_BLOCK, ActBlock)
  int player_id;
  Position target_location;
END_PACKET
    
DECLARE_PACKET(ACT_MULTIBLOCK, Act_MultiBlock)
  int player_id;
  int target_location;
  int second_location;
END_PACKET

DECLARE_PACKET(ACT_PASS, ActPass)
  int player_id;
  int dest_row;
  int dest_col;
END_PACKET

/*!
** @brief Base class describing a BB team.
*/
template <typename T>
class Team
{
public:
  Team(int team_id);

  int getTeamId() const;
  int getNbPlayer() const;
  const T* getPlayerConst(int id) const;
  T* getPlayer(int id);
  const std::string& getTeamName() const;
  const std::string& getCoachName() const;
  
  void resetTurn();
  
protected:
  int team_id_;   ///< Team id (0 or 1).
  
  typedef typename std::vector<T> PlayerList;
  typedef typename PlayerList::iterator PlayerIter;

  PlayerList player_;  ///< List of this team players.

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
