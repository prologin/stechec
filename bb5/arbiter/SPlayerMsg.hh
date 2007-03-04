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

#ifndef SPLAYERMSG_HH_
# define SPLAYERMSG_HH_

class SPlayer;

class SPlayerMsg
{
public:
  SPlayerMsg(SRules *r);
  ~SPlayerMsg();

  void setPlayer(int team_id, int player_id, SPlayer* p);
  
  void sendPosition(SPlayer* p);
  void sendMsgKnocked(Splayer* p);
  void sendMsgBlockPush(int nb_choice, Position[] choices, SPlayer* target_);
  void sendRoll(int result, int modifier, int required, SPlayer* p);
  void sendStatus(enum eStatus status, SPlayer* p);
  void sendMsgKO(int dice, SPlayer* p);
  	
private:
  SPlayer* getPlayer(int token, int team_id, int player_id);

  void msgDeclare(const MsgDeclare* m);
  void msgMove(const MsgMove* m);
  void msgStandUp(const MsgStandUp* m);
  void msgBlock(const MsgBlock* m);
  void msgPass(const MsgPass* m);
  
  SRules* r_;
  SPlayer* p_[2][MAX_PLAYER];
};

#endif /* !SPLAYERMSG_HH_ */
