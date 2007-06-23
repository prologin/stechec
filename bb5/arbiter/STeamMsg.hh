/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2007 The TBT Team.
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

#ifndef STEAMMSG_HH_
# define STEAMMSG_HH_

# include "SRules.hh"

/*!
** @brief Message handler for STeam.
** @ingroup tbt_server_rules
**
** Forwards messages to requested team, send messages as requested.
** Checks team existence and game state before forwarding to team.
**
** You will have only one instance of this class at runtime,
** handled by the SRules class.
*/
class STeamMsg
{
public:
  STeamMsg(SRules *r);
  ~STeamMsg();

  void setTeam(int team_id, STeam* t);

private:
  STeam* getTeam(int token, int team_id);

  void msgTeamInfo(const MsgTeamInfo* m);
  void msgPlayerCreate(const MsgPlayerCreate* m);
  void msgReroll(const MsgReroll* m);
  void msgBlockDice(const MsgBlockDice* m);
  void msgFollow(const MsgFollow* m);
  void msgBlockPush(const MsgBlockPush* m);
  
  SRules* r_;
  STeam* t_[2];
};

# include "STeamMsg.hxx"

#endif /* !STEAMMSG_HH_ */
