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
    r_(r),
    curr_acting_player_(-1)
{
  r_->HANDLE_F_WITH(MSG_TEAMINFO, STeam, this, msgTeamInfo, filterTeamInfo, GS_INITGAME);
  r_->HANDLE_F_WITH(MSG_PLAYERINFO, STeam, this, msgPlayerInfo, filterPlayerInfo, GS_INITGAME);
  r_->HANDLE_F_WITH(MSG_PLAYERPOS, STeam, this, msgPlayerPos, filterPlayerPos, GS_INITHALF);
}

void STeam::msgTeamInfo(const MsgTeamInfo* m)
{
  team_name_ = packetToString(m->team_name);
  coach_name_ = packetToString(m->coach_name);
  reroll_ = m->reroll;

  // Ok. Resend to everybody.
  r_->sendPacket(*m);
}

bool STeam::filterTeamInfo(const MsgTeamInfo* m)
{
  if (m->client_id != team_id_)
    return false;
  return true;
}


void STeam::msgPlayerInfo(const MsgPlayerInfo* m)
{
  // Create the player.
  if (player_[m->player_id] == NULL)
    {
      player_[m->player_id] = new SPlayer(r_, m, this);
      if (player_[m->player_id]->acceptPlayerCreation())
	{
	  r_->sendPacket(*m);
	}
      else
	{
	  // FIXME: maybe we should be more radical if a player is
	  //   invalid, like stopping the game.
	  delete player_[m->player_id];
	  player_[m->player_id] = NULL;
	}
    }
  
}

bool STeam::filterPlayerInfo(const MsgPlayerInfo* m)
{
  if (m->client_id != team_id_ || m->player_id < 0 || m->player_id >= MAX_PLAYER)
    return false;
  return true;
}

// Receive players initial position, on kick-off.
void STeam::msgPlayerPos(const MsgPlayerPos* m)
{
  const Position pos(m->row, m->col);
  SPlayer* p = getPlayer(m->player_id);
  if (p == NULL || !r_->getField()->intoField(pos))
    r_->sendIllegal(m->token, m->client_id);
  else
    {
      // not in their field side - error.
      if (team_id_ == 0 && pos.row >= ROWS / 2
          || team_id_ == 1 && pos.row < ROWS / 2)
        r_->sendIllegal(m->token, m->client_id);
      else
        {
          p->setPosition(pos, false);
          r_->sendPacket(*m);
        }
    }
}

bool STeam::filterPlayerPos(const MsgPlayerPos* m)
{
  if (m->client_id != team_id_ || m->player_id < 0 || m->player_id >= MAX_PLAYER )
    return false;
  return true;
}


void STeam::resetTurn()
{
  curr_acting_player_ = -1;
  blitz_done_ = false;
  pass_done_ = false;
  reroll_used_ = false;
  for (int i = 0; i < MAX_PLAYER; i++)
    if (player_[i] != NULL)
      player_[i]->resetTurn();
}

void STeam::setProneStunned()
{
  for (int i = 0; i < MAX_PLAYER; i++)
    if (player_[i] != NULL)
      player_[i]->setProne();  
}

bool STeam::canDoAction(const Packet* pkt, SPlayer* p)
{
  // Check if it's the team turn.
  if (r_->getCurrentTeamId() != pkt->client_id)
    {
      LOG4("Cannot do action: not team turn");
      r_->sendIllegal(pkt->token, pkt->client_id);
      return false;
    }

  if (p->hasDoneAction())
    {
      LOG4("Cannot do action: this player already does something this turn");
      r_->sendIllegal(pkt->token, pkt->client_id);
      return false;
    }

  if (curr_acting_player_ != -1 && curr_acting_player_ != p->getId())
    {
      player_[curr_acting_player_]->setHasPlayed();
    }

	if (pkt->token == ACT_STANDUP)
    {
      if (curr_acting_player_ == p->getId()) 
				{
					LOG4("Cannot try to stand up more than once.")
				  r_->sendIllegal(pkt->token, p->getId());
					return false;
				}
    }

  bool blitz_detected = false;
  if (pkt->token == ACT_BLOCK && p->hasDoneMove())
    blitz_detected = true;
  if (pkt->token == ACT_MOVE && p->hasDoneBlock())
    blitz_detected = true;
    
  if (pkt->token == ACT_PASS)
    {
      if (pass_done_)
	{
	  LOG4("Cannot do more than one pass in a single turn.")
	  r_->sendIllegal(pkt->token, p->getId());
	  return false;
	}
      pass_done_ = true;
    }
	
  if (blitz_detected)
    {
      if (blitz_done_)
	{
	  LOG4("Cannot do more than one blitz in a single turn.")
	  r_->sendIllegal(pkt->token, p->getId());
	  return false;
	}
      blitz_done_ = true;
    }

  curr_acting_player_ = p->getId();
  return true;
}
