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

  void setTurnMarker() {}

  int        state_;

private:
  SRules* r_;

  SPlayer* checkPlayerAction(const Packet* pkt, unsigned player_id);

  void msgTeamInfo(const MsgTeamInfo* m);
  void msgPlayerInfo(const MsgPlayerInfo* m);
  void msgPlayerPos(const MsgPlayerPos* m);

  void msgMove(const ActMove* m);
  void msgBlock(const ActBlock* m);
  void msgPass(const ActPass* m);

  void checkActResult(int result);
};

#endif /* !STEAM_HH_ */
