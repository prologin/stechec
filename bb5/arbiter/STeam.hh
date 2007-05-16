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

#ifndef STEAM_HH_
# define STEAM_HH_

# include "SPlayer.hh"
# include "Team.hh"

class SRules;
class SPlayerMsg;

/*!
** @brief Team management for the server.
**
** It symbolizes team and checks the validity of
** coach's decisions (reroll and block dices).
**
** You will have 2 instances of this class at runtime,
** handled by the SRules class.
*/
class STeam : public Team<SPlayer>
{  
public:
  STeam(int team_id, SRules* r, SPlayerMsg* pm);

  int state_; ///< Team state on client side.

  bool canDeclareAction(const MsgDeclare* pkt);

  //! @brief Check if this player can do this action
  bool canDoAction(const Packet* pkt, SPlayer* p);

  void resetTurn();
  void setProneStunned();
  void prepareKickoff();

  //! @brief Return current active player.
  SPlayer* getActivePlayer();
  //! @brief Set current pusher (not necessarily from this team).
  void setPusher(SPlayer* p);
  void setNbChoices(int nb);

  // Message handlers called by STeamMsg.
  void msgTeamInfo(const MsgTeamInfo* m);
  void msgPlayerCreate(const MsgPlayerCreate* m);
  void msgReroll(const MsgReroll* m);
  void msgBlockDice(const MsgBlockDice* m);
  void msgFollow(const MsgFollow* m);
  void msgBlockPush(const MsgBlockPush* m);

private:
  int active_player_id_; // Store ID is enough, since active player is from active team.
  SRules* r_;
  SPlayerMsg* pm_;

  int nb_choices_; // Number of block dices or squares to push to.
  SPlayer* current_pusher_; // When we are waiting for a blockPush choice.
};

#endif /* !STEAM_HH_ */
