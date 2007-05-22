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

/*!
** @brief Message handler for SPlayer.
**
** Forwards messages to requested player, send messages as requested.
** Checks player existence and game state before forwarding to player.
**
** You will have only one instance of this class at runtime,
** handled by the SRules class.
*/
class SPlayerMsg
{
public:
  SPlayerMsg(SRules *r);
  ~SPlayerMsg();

  void setPlayer(int team_id, int player_id, SPlayer* p);
  
  void sendMsgBlockPush(int nb_choice, const Position choices[], const SPlayer* target_) const;
  void SPlayerMsg::sendBlockResult(bool can_reroll, int strongest_team_id, int nb_dices,
      const enum eBlockDiceFace results[], const SPlayer* defender, const SPlayer* attacker) const;
  void sendMsgKnocked(const SPlayer* p) const;
  void sendMsgKO(int dice, const SPlayer* p) const;
  void sendPosition(const SPlayer* p) const;
  void sendRoll(enum eRoll type, int result, int modifier, int required,
      int reroll, enum eSkill skill, const SPlayer* p) const;
  void sendSkillQuestion(enum eSkill skill, const SPlayer* p) const;
  void sendStatus(enum eStatus status, const SPlayer* p) const;
  void sendTouchdooown(const SPlayer* p) const;

private:
  SPlayer* getPlayer(int token, int team_id, int player_id);

  void msgBlock(const MsgBlock* m);
  void msgDeclare(const MsgDeclare* m);
  void msgMove(const MsgMove* m);
  void msgPass(const MsgPass* m);
  void msgPlayerPos(const MsgPlayerPos* m);
  void msgSkill(const MsgSkill* m);
  void msgStandUp(const MsgStandUp* m);
  
  SRules* r_;
  SPlayer* p_[2][MAX_PLAYER];
};

#include "SPlayerMsg.hxx"

#endif /* !SPLAYERMSG_HH_ */
