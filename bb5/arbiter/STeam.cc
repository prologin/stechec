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
    curr_acting_player_(-1),
    r_(r)
{
  r_->HANDLE_F_WITH(MSG_TEAMINFO, STeam, this, msgTeamInfo, filterTeamInfo, GS_INITGAME);
  r_->HANDLE_F_WITH(MSG_PLAYERINFO, STeam, this, msgPlayerInfo, filterPlayerInfo, GS_INITGAME);
  r_->HANDLE_F_WITH(MSG_PLAYERPOS, STeam, this, msgPlayerPos, filterPlayerPos, GS_INITKICKOFF);
  r_->HANDLE_F_WITH(MSG_REROLL, STeam, this, msgReroll, filterReroll, GS_COACHBOTH);
  r_->HANDLE_F_WITH(MSG_BLOCKDICE, STeam, this, msgBlockDice, filterBlockDice, GS_COACHBOTH);
  r_->HANDLE_F_WITH(MSG_FOLLOW, STeam, this, msgFollow, filterFollow, GS_COACHBOTH);
  r_->HANDLE_F_WITH(ACT_BLOCKPUSH, STeam, this, msgBlockPush, filterBlockPush, GS_COACHBOTH);
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
	  if (p->getStatus() == STA_RESERVE)
	    {
	      p->setStatus(STA_STANDING);
	      p->setPosition(pos, false);
	      r_->sendPacket(*m);
	    }
        }
    }
}

bool STeam::filterPlayerPos(const MsgPlayerPos* m)
{
  if (m->client_id != team_id_ || m->player_id < 0 || m->player_id >= MAX_PLAYER )
    return false;
  return true;
}

// Want to use a reroll
void STeam::msgReroll(const MsgReroll* m)
{
  if (m->reroll&(reroll_used_||reroll_remain_ == 0))
    {
      r_->sendIllegal(MSG_REROLL, m->client_id);
      return;
    }
	
  if (!canUseReroll()||(state_ != GS_REROLL&&state_ != GS_BLOCK))
    {
      r_->sendIllegal(MSG_REROLL, m->client_id);
      return;
    }

  r_->sendPacket(*m);
  if (state_ != GS_BLOCK)
    state_ = m->client_id == 0 ? GS_COACH1 : GS_COACH2;

  if (m->reroll)
    {
      reroll_used_ = true;
      reroll_remain_ = 	reroll_remain_ - 1;
    }
  concerned_player_->finishAction(m->reroll);
}

bool STeam::filterReroll(const MsgReroll* m)
{
  if (m->client_id != team_id_)
    return false;
  return true;
}

void STeam::msgBlockDice(const MsgBlockDice* m)
{
  if (concerned_player_->nb_dice_ <= m->dice)
    {
      r_->sendIllegal(MSG_BLOCKDICE, m->client_id);
      return;
    }

  MsgBlockDice msg(r_->getCurrentTeamId());
  r_->sendPacket(msg);
  r_->getTeam(m->client_id)->state_ = m->client_id == 0 ? GS_COACH1 : GS_COACH2;
  concerned_player_->resolveBlock(m->dice);
}

bool STeam::filterBlockDice(const MsgBlockDice*)
{
  if (r_->getCurrentTeamId() != team_id_)
    return false;
  return true;
}

void STeam::msgFollow(const MsgFollow* m)
{
  r_->sendPacket(*m);
  state_ = team_id_ == 0 ? GS_COACH1 : GS_COACH2;
  concerned_player_->follow(m->follow);
}

bool STeam::filterFollow(const MsgFollow* m)
{
  if (m->client_id != team_id_)
    return false;
  return true;
}

void STeam::msgBlockPush(const ActBlockPush* m)
{
  r_->sendPacket(*m);
  state_ = team_id_ == 0 ? GS_COACH1 : GS_COACH2;
  concerned_player_->blockPush(m->square_chosen);
}

bool STeam::filterBlockPush(const ActBlockPush* m)
{
  if (m->client_id != team_id_)
    return false;
  return true;
}

void STeam::resetTurn()
{
  curr_acting_player_ = -1;
  concerned_player_ = NULL;
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

void STeam::prepareKickoff()
{
  for (int i = 0; i < MAX_PLAYER; i++)
    if (player_[i] != NULL)
      player_[i]->prepareKickoff();  
}

void STeam::setConcernedPlayer(SPlayer* p)
{
  concerned_player_ = p;
}

bool STeam::canDoAction(const Packet* pkt, SPlayer* p, enum eAction action)
{
  // Check if it's the team turn.
  if (r_->getCurrentTeamId() != pkt->client_id)
    {
      LOG4("Cannot do action: not team turn");
      LOG4(r_->getCurrentTeamId() << ", " << r_->getState());
      r_->sendIllegal(pkt->token, pkt->client_id);
      return false;
    }

  // Check if the player has allready played this turn.
  if (p->hasPlayed())
    {
      LOG4("Cannot do action: this player already does something this turn");
      r_->sendIllegal(pkt->token, pkt->client_id);
      return false;
    }

  // If it is not the current player, the previous one finishes this turn.
  if (curr_acting_player_ != -1 && curr_acting_player_ != p->getId())
    {
      player_[curr_acting_player_]->setHasPlayed();
    }

  // If he is stunned or out of the field, he can't do it
  if ((p->getStatus() != STA_STANDING	||pkt->token == ACT_STANDUP)
      &&(p->getStatus() != STA_PRONE	||pkt->token != ACT_STANDUP))
    return false;

  // Check the action declared is fit to the action
  if (  (action == MOVE 	&&(	pkt->token == ACT_PASS	||pkt->token == ACT_BLOCK))
	||(action == BLOCK 	&&(	pkt->token == ACT_PASS	||pkt->token == ACT_MOVE))
	||(action == BLITZ 	&&	pkt->token == ACT_PASS)
	||(action == PASS 	&&	pkt->token == ACT_BLOCK))
    return false;

  // If he has not played before
  if (p->getAction() == NONE)
    {
      if (action == BLITZ)
	{ 
	  if (blitz_done_)
	    {
	      LOG4("Cannot do more than one blitz in a single turn.");
		r_->sendIllegal(pkt->token, p->getId());
	      return false;
	    }
	  blitz_done_ = true;
	}
				
      if (action == PASS)
	{ 
	  if (pass_done_)
	    {
	      LOG4("Cannot do more than one pass in a single turn.");
		r_->sendIllegal(pkt->token, p->getId());
	      return false;
	    }
	  pass_done_ = true;
	}
				
      p->setAction(action);
    }
  // If he has played before
  else
    {
      // He must play the action he declared
      if (action != p->getAction())
	return false;
				
      // Cannot try to stand up more than once
      if (pkt->token == ACT_STANDUP)
	{
	  if (curr_acting_player_ == p->getId()) 
	    {
	      LOG4("Cannot try to stand up more than once.");
		r_->sendIllegal(pkt->token, p->getId());
	      return false;
	    }
	}
    }

  curr_acting_player_ = p->getId();
  return true;
}
