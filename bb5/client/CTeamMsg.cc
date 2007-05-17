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

#include "CRules.hh"
#include "CTeamMsg.hh"

CTeamMsg::CTeamMsg(CRules *r)
  : r_(r)
{
  t_[0] = NULL;
  t_[1] = NULL;

  r_->HANDLE_WITH(MSG_TEAMINFO, CTeamMsg, this, msgTeamInfo, GS_INITGAME);
  r_->HANDLE_WITH(MSG_PLAYERCREATE, CTeamMsg, this, msgPlayerCreate, GS_INITGAME);
  r_->HANDLE_WITH(MSG_REROLL, CTeamMsg, this, msgReroll, GS_REROLL);
  r_->HANDLE_WITH(MSG_TOUCHDOOOWN, CTeamMsg, this, msgTouchdooown, 0);
}

CTeamMsg::~CTeamMsg()
{
}

void CTeamMsg::setTeam(int team_id, CTeam* t)
{
  t_[team_id] = t;
}


CTeam* CTeamMsg::getTeam(int token, int team_id)
{
  if (team_id < 0 || team_id > 1 || t_[team_id] == NULL)
    {
      WARN("invalid coach id `%1' (token: `%2')",
	   team_id, r_->stringifyToken(token));
      return NULL;
    }
  return t_[team_id];
}

void CTeamMsg::msgTeamInfo(const MsgTeamInfo* m)
{
  CTeam *t = getTeam(m->token, m->client_id);
  if (t != NULL)
    t->msgTeamInfo(m);
}

void CTeamMsg::msgPlayerCreate(const MsgPlayerCreate* m)
{
  CTeam *t = getTeam(m->token, m->client_id);
  if (t != NULL)
    t->msgPlayerCreate(m);
}

void CTeamMsg::msgReroll(const MsgReroll* m)
{
  CTeam *t = getTeam(m->token, m->client_id);
  if (t != NULL)
    t->msgReroll(m);
}

void CTeamMsg::msgTouchdooown(const MsgTouchdooown* m)
{
  CTeam *t = getTeam(m->token, m->client_id);
  if (t != NULL)
    t->msgTouchdooown(m);
}
