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
#include "STeamMsg.hh"

STeamMsg::STeamMsg(SRules *r)
  : r_(r)
{
  t_[0] = NULL;
  t_[1] = NULL;

  r_->HANDLE_WITH(MSG_TEAMINFO, STeamMsg, this, msgTeamInfo, GS_INITGAME);
  r_->HANDLE_WITH(MSG_PLAYERCREATE, STeamMsg, this, msgPlayerCreate, GS_INITGAME);
  r_->HANDLE_WITH(MSG_PLAYERPOS, STeamMsg, this, msgPlayerPos, GS_INITKICKOFF);
  r_->HANDLE_WITH(MSG_REROLL, STeamMsg, this, msgReroll, 0);
  r_->HANDLE_WITH(MSG_BLOCKDICE, STeamMsg, this, msgBlockDice, 0);
  r_->HANDLE_WITH(MSG_FOLLOW, STeamMsg, this, msgFollow, 0);
  r_->HANDLE_WITH(MSG_BLOCKPUSH, STeamMsg, this, msgBlockPush, 0);
}

STeamMsg::~STeamMsg()
{
}

void STeamMsg::setTeam(int team_id, STeam* t)
{
  t_[team_id] = t;
}


STeam* STeamMsg::getTeam(int token, int team_id)
{
  if (team_id < 0 || team_id > 1 || t_[team_id] == NULL)
    {
      WARN("invalid coach id `%1' (token: `%2')",
	   team_id, r_->stringifyToken(token));
      return NULL;
    }
  return t_[team_id];
}

void STeamMsg::msgTeamInfo(const MsgTeamInfo* m)
{
  STeam *t = getTeam(m->token, m->client_id);
  if (t != NULL)
    t->msgTeamInfo(m);
}

void STeamMsg::msgPlayerCreate(const MsgPlayerCreate* m)
{
  STeam *t = getTeam(m->token, m->client_id);
  if (t != NULL)
    t->msgPlayerCreate(m);
}

void STeamMsg::msgPlayerPos(const MsgPlayerPos* m)
{
  STeam *t = getTeam(m->token, m->client_id);
  if (t != NULL)
    t->msgPlayerPos(m);
}

void STeamMsg::msgReroll(const MsgReroll* m)
{
  STeam* t;

  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      WARN("bad game state (%1)", r_->getState());
      return;
    }
  t = getTeam(m->token, m->client_id);
  if (t == NULL)
    return;
  t->msgReroll(m);
}

void STeamMsg::msgBlockDice(const MsgBlockDice* m)
{
  STeam* t;

  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      WARN("bad game state (%1)", r_->getState());
      return;
    }
  t = getTeam(m->token, m->client_id);
  if (t == NULL)
    return;
  t->msgBlockDice(m);
}

void STeamMsg::msgFollow(const MsgFollow* m)
{
  STeam* t;

  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      WARN("bad game state (%1)", r_->getState());
      return;
    }
  t = getTeam(m->token, m->client_id);
  if (t == NULL)
    return;
  t->msgFollow(m);
}

void STeamMsg::msgBlockPush(const MsgBlockPush* m)
{
  STeam* t;

  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      WARN("bad game state (%1)", r_->getState());
      return;
    }
  t = getTeam(m->token, m->client_id);
  if (t == NULL)
    return;
  t->msgBlockPush(m);
}
