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

#ifndef STEAM_HH_
# define STEAM_HH_

# include "SPlayer.hh"
# include "Team.hh"

class SRules;

/*! @brief Team management for the server.
**
** Now it's time to implement THE rules :)
** It seems to be a good place to start here.
**
** You will have 2 instances of this class at runtime,
** handled by the SRules class.
*/
class STeam : public Team<SPlayer>
{  
public:
  STeam(int team_id, SRules* r);

  int state_;	///< Team state on client side.

  //! @brief Check if this player can do this action
  bool canDoAction(const Packet* pkt, SPlayer* p, enum eAction action);

  void resetTurn();
  void setProneStunned();
  void prepareKickoff();
  void setConcernedPlayer(SPlayer* p);
  int curr_acting_player_;

private:
  void msgTeamInfo(const MsgTeamInfo* m);
  void msgPlayerCreate(const MsgPlayerCreate* m);
  void msgPlayerPos(const MsgPlayerPos* m);
  void msgReroll(const MsgReroll* m);
  void msgBlockDice(const MsgBlockDice* m);
  void msgFollow(const MsgFollow* m);
  void msgBlockPush(const ActBlockPush* m);
  bool filterTeamInfo(const MsgTeamInfo* m);
  bool filterPlayerCreate(const MsgPlayerCreate* m);
  bool filterPlayerPos(const MsgPlayerPos* m);
  bool filterReroll(const MsgReroll* m);
  bool filterBlockDice(const MsgBlockDice* m);
  bool filterFollow(const MsgFollow* m);
  bool filterBlockPush(const ActBlockPush* m);

  SRules* r_;
  SPlayer* concerned_player_; // When we are waiting for a reroll decision
};

#endif /* !STEAM_HH_ */
