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

#ifndef CPLAYERMSG_HH_
# define CPLAYERMSG_HH_

# include "CPlayer.hh"

/*!
** @brief Handle messages for CPlayer objects.
**
** There is a single instance of this class, that dispatch
** message to the good CPlayer object.
*/
class CPlayerMsg
{
public:
  CPlayerMsg(CRules* r);
  ~CPlayerMsg();

  void setPlayer(int team_id, int player_id, CPlayer* p);
  
private:
  CPlayer* getPlayer(int token_id, int team_id, int player_id);
  
  void msgDeclareAction(const MsgDeclare* m);
  void msgPlayerPos(const MsgPlayerPos* m);
  void msgPlayerMove(const MsgMove* m);
  void msgPlayerKnocked(const MsgPlayerKnocked* m);
  void msgPlayerStatus(const MsgPlayerStatus* m);
  void msgPlayerKO(const MsgPlayerKO* m);

  CRules* r_;
  CPlayer* p_[2][MAX_PLAYER];
};

#endif /* !CPLAYERMSG_HH_ */
