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

#include "SPlayer.hh"

inline SPlayer* SPlayerMsg::getPlayer(int token, int team_id, int player_id)
{
  if (team_id < 0 || team_id > 1 ||
      player_id < 0 || player_id >= MAX_PLAYER ||
      p_[team_id][player_id] == NULL)
    {
      LOG3("Token `%1' is addressed to non-existent player `%2' of team `%3'.",
          r_->stringifyToken(token), player_id, team_id);
      r_->sendIllegal(token, team_id, ERR_NONEXISTENTPLAYER);
      return NULL;
    }
  return p_[team_id][player_id];
}

/*
** Messages senders.
*/

inline void SPlayerMsg::sendMsgBlockPush(int nb_choice,
    const Position choices[], const SPlayer* target) const
{
  MsgBlockPush pkt(r_->getCurrentTeamId());
  pkt.target_row = target->getPosition().row;
  pkt.target_col = target->getPosition().col;
  pkt.nb_choice = nb_choice;
  for (int i = 0; i < nb_choice; i++)
    {
      pkt.choice[i].row = choices[i].row;
      pkt.choice[i].col = choices[i].col;
    }
  r_->waitForCurrentOpponentChoice(pkt.client_id);
  r_->sendPacket(pkt);
}

inline void SPlayerMsg::sendMsgKnocked(const SPlayer* p) const
{
  MsgPlayerKnocked pkt(p->getTeamId());
  pkt.player_id = p->getId();
  r_->sendPacket(pkt);
}

inline void SPlayerMsg::sendMsgKO(int dice, const SPlayer* p) const
{
  MsgPlayerKO msg;
  msg.player_id = p->getId();
  msg.dice = dice;
  r_->sendPacket(msg);
}      

inline void SPlayerMsg::sendPosition(const SPlayer* p) const
{
  MsgPlayerPos pkt(p->getTeamId());
  pkt.player_id = p->getId();
  pkt.row = p->getPosition().row;
  pkt.col = p->getPosition().col;
  r_->sendPacket(pkt);
}

inline void SPlayerMsg::sendBlockResult(bool can_reroll, int strongest_team_id, int nb_dices,
    const enum eBlockDiceFace results[], const SPlayer* defender, const SPlayer* attacker) const
{
  MsgBlockResult msg(attacker->getTeamId());
  msg.player_id = attacker->getId();
  msg.opponent_id = defender->getId();
  msg.reroll = can_reroll;
  msg.strongest_team_id = strongest_team_id;
  msg.nb_dice = nb_dices;
  for (int i = 0; i < nb_dices; ++i)
    msg.results[i] = results[i];
  r_->waitForCurrentOpponentChoice(strongest_team_id);
  r_->sendPacket(msg);
}

inline void SPlayerMsg::sendRoll(enum eRoll type, int result, int modifier,
    int required, int reroll, enum eSkill skill, const SPlayer* p) const
{
  MsgResult msg(p->getTeamId());
  msg.player_id = p->getId();
  msg.roll_type = type;
  msg.result = result;
  msg.modifier = modifier;
  msg.required = required;
  msg.reroll = reroll;
  msg.skill = skill;
  if (reroll || (skill != SK_UNASSIGNED))
    r_->waitForCurrentOpponentChoice(msg.client_id);
  r_->sendPacket(msg);
}

inline void SPlayerMsg::sendSkillQuestion(enum eSkill skill, const SPlayer* p) const
{
  MsgSkill msg(p->getTeamId());
  msg.player_id = p->getId();
  msg.skill = skill;
  msg.choice = -1;
  r_->waitForCurrentOpponentChoice(msg.client_id);
  r_->sendPacket(msg);
}

inline void SPlayerMsg::sendStatus(enum eStatus status, const SPlayer* p) const
{
  MsgPlayerStatus pkt(p->getTeamId());
  pkt.player_id = p->getId();
  pkt.status = status;
  r_->sendPacket(pkt);
}

inline void SPlayerMsg::sendTouchdooown(const SPlayer* p) const
{
  MsgTouchdooown msg(p->getTeamId());
  msg.player_id = p->getId();
  r_->sendPacket(msg);
}
