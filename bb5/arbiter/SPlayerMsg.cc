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
  r_->HANDLE_WITH(MSG_DECLARE, SPlayerMsg, this, msgDeclare, 0);
  r_->HANDLE_WITH(MSG_MOVE, SPlayerMsg, this, msgMove, 0);
  r_->HANDLE_WITH(MSG_STANDUP, SPlayerMsg, this, msgStandUp, 0);
  r_->HANDLE_WITH(MSG_BLOCK, SPlayerMsg, this, msgBlock, 0);
  r_->HANDLE_WITH(MSG_PASS, SPlayerMsg, this, msgPass, 0);
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


SPlayer* SPlayerMsg::getPlayer(int token, int team_id, int player_id)
{
  if (team_id < 0 || team_id > 1 ||
      player_id < 0 || player_id >= MAX_PLAYER ||
      p_[team_id][player_id] == NULL)
    {
      WARN("invalid player: team `%1', id `%2' (token: `%2')",
	   team_id, player_id, r_->stringifyToken(token));
      return NULL;
    }
  return p_[team_id][player_id];
}

void SPlayerMsg::msgDeclare(const MsgDeclare* m)
{
  SPlayer* p;

  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      WARN("bad game state (%1)", r_->getState());
      return;
    }
  p = getPlayer(m->token, m->client_id, m->player_id);
  if (p == NULL)
    return;
  p->msgDeclare(m);
}

void SPlayerMsg::msgMove(const MsgMove* m)
{
  SPlayer* p;

  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      WARN("bad game state (%1)", r_->getState());
      return;
    }
  p = getPlayer(m->token, m->client_id, m->player_id);
  if (p == NULL)
    return;
  p->msgMove(m);
}

void SPlayerMsg::msgStandUp(const MsgStandUp* m)
{
  SPlayer* p;

  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      WARN("bad game state (%1)", r_->getState());
      return;
    }
  p = getPlayer(m->token, m->client_id, m->player_id);
  if (p == NULL)
    return;
  p->msgStandUp(m);
}

void SPlayerMsg::msgBlock(const MsgBlock* m)
{
  SPlayer* p;

  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      WARN("bad game state (%1)", r_->getState());
      return;
    }
  p = getPlayer(m->token, m->client_id, m->player_id);
  if (p == NULL)
    return;
  p->msgBlock(m);
}

void SPlayerMsg::msgPass(const MsgPass* m)
{
  SPlayer* p;

  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      WARN("bad game state (%1)", r_->getState());
      return;
    }
  p = getPlayer(m->token, m->client_id, m->player_id);
  if (p == NULL)
    return;
  p->msgPass(m);
}
