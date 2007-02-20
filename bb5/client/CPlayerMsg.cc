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
#include "CPlayerMsg.hh"

CPlayerMsg::CPlayerMsg(CRules *r)
  : r_(r)
{
  for (int i = 0; i < MAX_PLAYER; i++)
    {
      p_[0][i] = NULL;
      p_[1][i] = NULL;
    }

  r_->HANDLE_WITH(MSG_DECLARE, CPlayerMsg, this, msgDeclareAction, GS_COACHBOTH);
  r_->HANDLE_WITH(MSG_PLAYERPOS, CPlayerMsg, this, msgPlayerPos, GS_ALL);
  r_->HANDLE_WITH(MSG_MOVE, CPlayerMsg, this, msgPlayerMove, GS_COACHBOTH | GS_REROLL);
  r_->HANDLE_WITH(MSG_PLAYERKNOCKED, CPlayerMsg, this, msgPlayerKnocked, GS_COACHBOTH | GS_REROLL);
  r_->HANDLE_WITH(MSG_PLAYERSTATUS, CPlayerMsg, this, msgPlayerStatus, GS_ALL);
  r_->HANDLE_WITH(MSG_PLAYERKO, CPlayerMsg, this, msgPlayerKO, GS_INITKICKOFF);
}

CPlayerMsg::~CPlayerMsg()
{
}

void CPlayerMsg::setPlayer(int team_id, int player_id, CPlayer* p)
{
  assert(team_id == 0 || team_id == 1);
  assert(player_id >= 0 && player_id < MAX_PLAYER);
  p_[team_id][player_id] = p;
}



CPlayer* CPlayerMsg::getPlayer(int token, int team_id, int player_id)
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

void CPlayerMsg::msgDeclareAction(const MsgDeclare* m)
{
  CPlayer* p = getPlayer(m->token, m->client_id, m->player_id);
  if (p != NULL)
    p->msgDeclareAction(m);
}

void CPlayerMsg::msgPlayerPos(const MsgPlayerPos* m)
{
  CPlayer* p = getPlayer(m->token, m->client_id, m->player_id);
  if (p != NULL)
    p->msgPlayerPos(m);
}

void CPlayerMsg::msgPlayerMove(const MsgMove* m)
{
  CPlayer* p = getPlayer(m->token, m->client_id, m->player_id);
  if (p != NULL)
    p->msgPlayerMove(m);
}

void CPlayerMsg::msgPlayerKnocked(const MsgPlayerKnocked* m)
{
  CPlayer* p = getPlayer(m->token, m->client_id, m->player_id);
  if (p != NULL)
    p->msgPlayerKnocked(m);
}

void CPlayerMsg::msgPlayerStatus(const MsgPlayerStatus* m)
{
  CPlayer* p = getPlayer(m->token, m->client_id, m->player_id);
  if (p != NULL)
    p->msgPlayerStatus(m);
}

void CPlayerMsg::msgPlayerKO(const MsgPlayerKO* m)
{
  CPlayer* p = getPlayer(m->token, m->client_id, m->player_id);
  if (p != NULL)
    p->msgPlayerKO(m);
}
