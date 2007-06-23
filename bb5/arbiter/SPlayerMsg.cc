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

#include "SPlayerMsg.hh"

#include "SPlayer.hh"

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
** Messages receivers.
*/

void SPlayerMsg::msgBlock(const MsgBlock* m)
{
  SPlayer* p = getPlayer(m->token, m->client_id, m->player_id);
  if (p == NULL)
    return;
  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      LOG3("Token `%1' is not allowed in game state `%2'.",
          r_->stringifyToken(m->token), r_->getState());
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
      LOG3("Token `%1' is not allowed in game state `%2'.",
          r_->stringifyToken(m->token), r_->getState());
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
      LOG3("Token `%1' is not allowed in game state `%2'.",
          r_->stringifyToken(m->token), r_->getState());
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
      LOG3("Token `%1' is not allowed in game state `%2'.",
          r_->stringifyToken(m->token), r_->getState());
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
      LOG3("Token `%1' is not allowed in game state `%2'.",
          r_->stringifyToken(m->token), r_->getState());
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
      LOG3("Token `%1' is not allowed in game state `%2'.",
          r_->stringifyToken(m->token), r_->getState());
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
      LOG3("Token `%1' is not allowed in game state `%2'.",
          r_->stringifyToken(m->token), r_->getState());
      r_->sendIllegal(m->token, m->client_id, ERR_WRONGCONTEXT);
      return;
    }
  p->msgSkill(m);
}
