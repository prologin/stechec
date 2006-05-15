/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2006 The TBT Team.
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
#include "STeam.hh"

STeam::STeam(int team_id, SRules* r)
  : Team<SPlayer>(team_id),
    state_(GS_INITGAME),
    r_(r)
{
  r_->HANDLE_WITH(MSG_TEAMINFO, STeam, this, msgTeamInfo, GS_INITGAME);
  r_->HANDLE_WITH(MSG_PLAYERINFO, STeam, this, msgPlayerInfo, GS_INITGAME);
  r_->HANDLE_WITH(MSG_PLAYERPOS, STeam, this, msgPlayerPos, GS_INITHALF);

  r_->HANDLE_WITH(ACT_MOVE, STeam, this, msgMove, GS_COACHBOTH);
  r_->HANDLE_WITH(ACT_BLOCK, STeam, this, msgBlock, GS_COACHBOTH);
  r_->HANDLE_WITH(ACT_PASS, STeam, this, msgPass, GS_COACHBOTH);
}

void STeam::msgTeamInfo(const MsgTeamInfo* m)
{
  if (m->client_id != team_id_)
    return;

  team_name_ = packetToString(m->team_name);
  coach_name_ = packetToString(m->coach_name);
  reroll_ = m->reroll;

  // Ok. Resend to everybody.
  r_->sendPacket(*m);
}

void STeam::msgPlayerInfo(const MsgPlayerInfo* m)
{
  if (m->client_id != team_id_)
    return;

  // Check that we aren't out of vector bound.
  if (m->player_id <= 0
      || m->player_id > (int)player_.size() + 1
      || m->player_id > 16)
    {
      r_->sendIllegal(m->token, m->client_id);
      return;
    }
  if (m->player_id == (int)player_.size() + 1)
    player_.push_back(SPlayer(m->player_id, team_id_, r_));

  // Dispatch to the right player.
  player_[m->player_id - 1].msgPlayerInfo(m);
}

// Receive players initial position, on kick-off.
void STeam::msgPlayerPos(const MsgPlayerPos* m)
{
  if (m->client_id != team_id_)
    return;

  const Position pos(m->row, m->col);
  SPlayer* p = getPlayer(m->player_id);
  if (p == NULL || !r_->getField()->intoField(pos))
    r_->sendIllegal(m->token, m->client_id);
  else
    {
      // not in their field side - error.
      if (team_id_ == 1 && pos.row >= ROWS / 2
          || team_id_ == 2 && pos.row < ROWS / 2)
        r_->sendIllegal(m->token, m->client_id);
      else
        {
          LOG5("Allow player position on " << pos << " (team " << team_id_ << ")");
          p->setPosition(pos);
          r_->sendPacket(*m);
        }
    }
}

SPlayer* STeam::checkPlayerAction(const Packet* pkt, unsigned player_id)
{
  // Check that the message is for us.
  if (pkt->client_id != team_id_)
    return NULL;

  // Check if it's the team turn.
  if (r_->getCurrentTeamId() != pkt->client_id)
    {
      LOG4("Can't do action: not team turn");
      r_->sendIllegal(pkt->token, pkt->client_id);
      return NULL;
    }

  // Check that we aren't out of vector bound.
  SPlayer* p = getPlayer(player_id);
  if (p == NULL)
    {
      LOG4("Can't do action: player " << player_id << " doesn't exist.");
      r_->sendIllegal(pkt->token, pkt->client_id);
      return NULL;
    }

  // Check this player hasn't performed action yet.
  // FIXME: 
  if (p->hasDoneAction() && false)
    {
      LOG4("Can't do action: this player already done one this turn");
      r_->sendIllegal(pkt->token, pkt->client_id);
      return NULL;
    }

  // Ok.
  return p;
}

void STeam::checkActResult(int result)
{
  if (result == ACT_RES_TO)
    {
      // FIXME: Make the turnover.
      setTurnMarker();
    }
}

void STeam::msgMove(const ActMove* m)
{
  SPlayer* p = checkPlayerAction(m, m->player_id);
  if (p != NULL)
    checkActResult(p->doMove(m));
}

void STeam::msgBlock(const ActBlock* m)
{
  SPlayer* p = checkPlayerAction(m, m->player_id);
  if (p != NULL)
    checkActResult(p->doBlock(m));
}

void STeam::msgPass(const ActPass* m)
{
  SPlayer* p = checkPlayerAction(m, m->player_id);
  if (p == NULL)
    return;
  if (pass_done_)
    {
      r_->sendIllegal(ACT_PASS, m->player_id);
      return;
    }
  pass_done_ = true;
  checkActResult(p->doPass(m));
}
