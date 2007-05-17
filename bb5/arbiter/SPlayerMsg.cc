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

#include "SRules.hh"
#include "SPlayerMsg.hh"

SPlayerMsg::SPlayerMsg(SRules* r)
  : r_(r)
{
  for (int i = 0; i < MAX_PLAYER; i++)
    {
      p_[0][i] = NULL;
      p_[1][i] = NULL;
    }
  r_->HANDLE_WITH(MSG_PLAYERPOS, SPlayerMsg, this, msgPlayerPos, GS_INITKICKOFF);
  r_->HANDLE_WITH(MSG_DECLARE, SPlayerMsg, this, msgDeclare, 0);
  r_->HANDLE_WITH(MSG_MOVE, SPlayerMsg, this, msgMove, 0);
  r_->HANDLE_WITH(MSG_STANDUP, SPlayerMsg, this, msgStandUp, 0);
  r_->HANDLE_WITH(MSG_BLOCK, SPlayerMsg, this, msgBlock, 0);
  r_->HANDLE_WITH(MSG_PASS, SPlayerMsg, this, msgPass, 0);
  r_->HANDLE_WITH(MSG_SKILL, SPlayerMsg, this, msgSkill, 0);
}

SPlayerMsg::~SPlayerMsg()
{
}

void SPlayerMsg::setPlayer(int team_id, int player_id, SPlayer* p)
{
  assert(team_id == 0 || team_id == 1);
  assert(player_id >= 0 && player_id < MAX_PLAYER);
  p_[team_id][player_id] = p;
}


/*
** Messages senders.
*/

void SPlayerMsg::sendMsgBlockPush(int nb_choice, Position choices[], SPlayer* target) {
  MsgBlockPush pkt(r_->getCurrentTeamId());
  pkt.target_row = target->getPosition().row;
  pkt.target_col = target->getPosition().col;
  pkt.nb_choice = nb_choice;
  for (int i = 0; i < nb_choice; i++)
    {
      pkt.choice[i].row = choices[i].row;
      pkt.choice[i].col = choices[i].col;
    }
  r_->sendPacket(pkt);
}

void SPlayerMsg::sendMsgKnocked(SPlayer* p)
{
  MsgPlayerKnocked pkt(p->getTeamId());
  pkt.player_id = p->getId();
  r_->sendPacket(pkt);
}

void SPlayerMsg::sendMsgKO(int dice, SPlayer* p)
{
  MsgPlayerKO msg;
  msg.player_id = p->getId();
  msg.dice = dice;
  r_->sendPacket(msg);
}      

void SPlayerMsg::sendPosition(SPlayer* p)
{
  MsgPlayerPos pkt(p->getTeamId());
  pkt.player_id = p->getId();
  pkt.row = p->getPosition().row;
  pkt.col = p->getPosition().col;
  r_->sendPacket(pkt);
}

void SPlayerMsg::sendRoll(enum eRoll type, int result, int modifier, int required, int reroll, enum eSkill skill, SPlayer* p)
{
  MsgResult msg(p->getTeamId());
  msg.player_id = p->getId();
  msg.roll_type = type;
  msg.result = result;
  msg.modifier = modifier;
  msg.required = required;
  msg.reroll = reroll;
  msg.skill = skill;
  r_->sendPacket(msg);
}

void SPlayerMsg::sendSkillQuestion(enum eSkill skill, SPlayer* p)
{
  MsgSkill msg(p->getTeamId());
  msg.player_id = p->getId();
  msg.skill = skill;
  msg.choice = -1;
  r_->sendPacket(msg);
}

void SPlayerMsg::sendStatus(enum eStatus status, SPlayer* p)
{
  MsgPlayerStatus pkt(p->getTeamId());
  pkt.player_id = p->getId();
  pkt.status = status;
  r_->sendPacket(pkt);
}

void SPlayerMsg::sendTouchdooown(SPlayer* p)
{
  MsgTouchdooown msg(p->getTeamId());
  msg.player_id = p->getId();
  r_->sendPacket(msg);
}

/*
** Messages receivers.
*/

SPlayer* SPlayerMsg::getPlayer(int token, int team_id, int player_id)
{
  if (team_id < 0 || team_id > 1 ||
      player_id < 0 || player_id >= MAX_PLAYER ||
      p_[team_id][player_id] == NULL)
    {
      WARN("invalid player: team `%1', id `%2' (token: `%2')",
          team_id, player_id, r_->stringifyToken(token));
      r_->sendIllegal(token, team_id, ERR_NONEXISTENTPLAYER);
      return NULL;
    }
  return p_[team_id][player_id];
}

void SPlayerMsg::msgBlock(const MsgBlock* m)
{
  SPlayer* p = getPlayer(m->token, m->client_id, m->player_id);
  if (p == NULL)
    return;
  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      WARN("bad game state (%1)", r_->getState());
      r_->sendIllegal(m->token, m->client_id, ERR_WRONGCONTEXT);
      return;
    }
  p->msgBlock(m);
}

void SPlayerMsg::msgDeclare(const MsgDeclare* m)
{
  SPlayer* p = getPlayer(m->token, m->client_id, m->player_id);
  if (p == NULL)
    return;
  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      WARN("bad game state (%1)", r_->getState());
      r_->sendIllegal(m->token, m->client_id, ERR_WRONGCONTEXT);
      return;
    }
  p->msgDeclare(m);
}

void SPlayerMsg::msgMove(const MsgMove* m)
{
  SPlayer* p = getPlayer(m->token, m->client_id, m->player_id);
  if (p == NULL)
    return;
  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      WARN("bad game state (%1)", r_->getState());
      r_->sendIllegal(m->token, m->client_id, ERR_WRONGCONTEXT);
      return;
    }
  p->msgMove(m);
}

void SPlayerMsg::msgPass(const MsgPass* m)
{
  SPlayer* p = getPlayer(m->token, m->client_id, m->player_id);
  if (p == NULL)
    return;
  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      WARN("bad game state (%1)", r_->getState());
      r_->sendIllegal(m->token, m->client_id, ERR_WRONGCONTEXT);
      return;
    }
  p->msgPass(m);
}

void SPlayerMsg::msgPlayerPos(const MsgPlayerPos* m)
{
  SPlayer* p = getPlayer(m->token, m->client_id, m->player_id);
  if (p == NULL)
    return;
  if (r_->getState() != GS_INITKICKOFF)
    {
      WARN("bad game state (%1)", r_->getState());
      r_->sendIllegal(m->token, m->client_id, ERR_WRONGCONTEXT);
      return;
    }
  p->msgPlayerPos(m);
}

void SPlayerMsg::msgStandUp(const MsgStandUp* m)
{
  SPlayer* p = getPlayer(m->token, m->client_id, m->player_id);
  if (p == NULL)
    return;
  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      WARN("bad game state (%1)", r_->getState());
      r_->sendIllegal(m->token, m->client_id, ERR_WRONGCONTEXT);
      return;
    }
  p->msgStandUp(m);
}

void SPlayerMsg::msgSkill(const MsgSkill* m)
{
  SPlayer* p = getPlayer(m->token, m->client_id, m->player_id);
  if (p == NULL)
    return;
  if (r_->getState() != GS_COACH1
      && r_->getState() != GS_COACH2
      && r_->getState() != GS_KICKOFF)
    {
      WARN("bad game state (%1)", r_->getState());
      r_->sendIllegal(m->token, m->client_id, ERR_WRONGCONTEXT);
      return;
    }
  p->msgSkill(m);
}

