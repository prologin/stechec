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

#include "STeamMsg.hh"

#include "STeam.hh"

STeamMsg::STeamMsg(SRules *r)
  : r_(r)
{
  t_[0] = NULL;
  t_[1] = NULL;

  r_->HANDLE_WITH(MSG_TEAMINFO, STeamMsg, this, msgTeamInfo, GS_INITGAME);
  r_->HANDLE_WITH(MSG_PLAYERCREATE, STeamMsg, this, msgPlayerCreate, GS_INITGAME);
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

/*
** Messages receivers.
*/

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

void STeamMsg::msgReroll(const MsgReroll* m)
{
  STeam* t;

  if (r_->getState() != GS_COACH1
      && r_->getState() != GS_COACH2)
    {
      LOG3("Token `%1' is not allowed in game state `%2'.",
          r_->stringifyToken(m->token), r_->getState());
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  t = getTeam(m->token, m->client_id);
  if (t != NULL)
    t->msgReroll(m);
}

void STeamMsg::msgBlockDice(const MsgBlockDice* m)
{
  STeam* t;

  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      LOG3("Token `%1' is not allowed in game state `%2'.",
          r_->stringifyToken(m->token), r_->getState());
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  t = getTeam(m->token, m->client_id);
  if (t != NULL)
    t->msgBlockDice(m);
}

void STeamMsg::msgFollow(const MsgFollow* m)
{
  STeam* t;

  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      LOG3("Token `%1' is not allowed in game state `%2'.",
          r_->stringifyToken(m->token), r_->getState());
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  t = getTeam(m->token, m->client_id);
  if (t != NULL)
    t->msgFollow(m);
}

void STeamMsg::msgBlockPush(const MsgBlockPush* m)
{
  STeam* t;

  if (r_->getState() != GS_COACH1 && r_->getState() != GS_COACH2)
    {
      LOG3("Token `%1' is not allowed in game state `%2'.",
          r_->stringifyToken(m->token), r_->getState());
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  t = getTeam(m->token, m->client_id);
  if (t != NULL)
    t->msgBlockPush(m);
}
