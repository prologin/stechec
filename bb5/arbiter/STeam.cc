/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2006, 2007 The TBT Team.
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
#include "STeam.hh"

STeam::STeam(int team_id, SRules* r, SPlayerMsg* pm)
  : Team<SPlayer>(team_id),
    state_(GS_INITGAME),
    active_player_id_(-1),
    r_(r),
    pm_(pm)
{
}

void STeam::msgTeamInfo(const MsgTeamInfo* m)
{
  team_name_ = packetToString(m->team_name);
  coach_name_ = packetToString(m->coach_name);
  reroll_ = m->reroll;

  // Ok. Resend to everybody.
  r_->sendPacket(*m);
}

void STeam::msgPlayerCreate(const MsgPlayerCreate* m)
{
  // Create the player.
  if (player_[m->player_id] == NULL)
    {
      player_[m->player_id] = new SPlayer(r_, m, this, pm_);
      if (player_[m->player_id]->acceptPlayerCreation())
        {
          pm_->setPlayer(m->client_id, m->player_id, player_[m->player_id]);
          r_->sendPacket(*m);
        }
      else
        {
          // FIXME: maybe we should be more radical if a player is
          //        invalid, like stopping the game.
          delete player_[m->player_id];
          player_[m->player_id] = NULL;
          LOG2("Team %1. Player %2 creation refused.", team_id_, m->player_id);
          r_->sendIllegal(MSG_PLAYERCREATE, m->client_id);
        }
    }
  else
    {
      LOG2("Team %1. Player %2 already exists.", team_id_, m->player_id);
      r_->sendIllegal(MSG_PLAYERCREATE, m->client_id, ERR_ALREADYEXISTS);
    }
}

void STeam::msgReroll(const MsgReroll* m)
{
  if (state_ != GS_REROLL)
    {
      LOG2("Coach %1 has not to reroll or accept a roll now.");
      r_->sendIllegal(m->token, m->client_id, ERR_WRONGCONTEXT);
    }
  else if (m->reroll && !canUseReroll())
    {
      LOG2("Coach %1 can not use team reroll for this turn.", team_id_);
      r_->sendIllegal(m->token, m->client_id);
    }
  else
    {
      LOG2("Coach %1 %2.", team_id_, m->reroll?"uses a team reroll":"accepts the roll result");
      r_->sendPacket(*m);
      state_ = m->client_id == 0 ? GS_COACH1 : GS_COACH2;
      if (m->reroll)
        {
          useReroll();
        }
      r_->getActionHandler()->process(m->reroll);
    }
}

void STeam::msgBlockDice(const MsgBlockDice* m)
{
  if (state_ != GS_BLOCK)
    {
      LOG2("Coach %1 doesn't have to choose a block dice now.", team_id_);
      r_->sendIllegal(m->token, m->client_id, ERR_WRONGCONTEXT);
      return;
    }
  if (m->dice < 0 || nb_choices_ <= m->dice)
    {
      LOG2("The choice number %1 is out of range [0,%2[.",
          m->dice, nb_choices_);
      r_->sendIllegal(m->token, m->client_id, ERR_UNREADABLE);
      return;
    }

  MsgBlockDice msg(r_->getCurrentTeamId());
  r_->sendPacket(msg);
  state_ = m->client_id == 0 ? GS_COACH1 : GS_COACH2;
  r_->getActionHandler()->process(false, m->dice);
}

void STeam::msgFollow(const MsgFollow* m)
{
  if (state_ != GS_FOLLOW)
    {
      LOG2("Coach %1 doesn't have the choice to follow or not now.", team_id_);
      r_->sendIllegal(m->token, m->client_id, ERR_WRONGCONTEXT);
      return;
    }
  r_->sendPacket(*m);
  state_ = team_id_ == 0 ? GS_COACH1 : GS_COACH2;
  r_->getActionHandler()->process(m->follow);
}

void STeam::msgBlockPush(const MsgBlockPush* m)
{
  if (state_ != GS_PUSH)
    {
      r_->sendIllegal(MSG_BLOCKPUSH, m->client_id, ERR_WRONGCONTEXT);
      return;
    }
  if (m->square_chosen < 0 || nb_choices_ <= m->square_chosen)
    {
      LOG2("Invalid push choice.");
      r_->sendIllegal(MSG_BLOCKPUSH, m->client_id);
      return;
    }
  r_->sendPacket(*m);
  state_ = team_id_ == 0 ? GS_COACH1 : GS_COACH2;
  r_->getActionHandler()->process(false, m->square_chosen);
}

void STeam::resetTurn()
{
  active_player_id_ = -1;
  current_pusher_ = NULL;
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

SPlayer* STeam::getActivePlayer()
{
  return player_[active_player_id_];
}

void STeam::setPusher(SPlayer* p)
{
  current_pusher_ = p;
}

void STeam::setNbChoices(int nb)
{
  nb_choices_ = nb;
}

bool STeam::canDeclareAction(const MsgDeclare* pkt)
{
  SPlayer* p = player_[pkt->player_id];

  // Check the action exists.
  if (pkt->action != DCL_MOVE && pkt->action != DCL_BLOCK 
       && pkt->action != DCL_BLITZ && pkt->action != DCL_PASS)
    {
      LOG4("Must declare an existing action");
      r_->sendIllegal(pkt->token, pkt->client_id, ERR_UNREADABLE);
      return false;
    }

  // Check if it's the team turn.
  if (r_->getCurrentTeamId() != pkt->client_id)
    {
      LOG4("Cannot declare action: not team turn");
      LOG4("%1, %2",r_->getCurrentTeamId(), r_->getState());
      r_->sendIllegal(pkt->token, pkt->client_id, ERR_WRONGCONTEXT);
      return false;
    }

  // The previous one finishes this turn.
  if (active_player_id_ != -1)
    {
      player_[active_player_id_]->setHasPlayed();
      MsgDeclare m(pkt->client_id);
      m.player_id = active_player_id_;
      m.action = DCL_NONE;
      r_->sendPacket(m);
      active_player_id_ = -1;
    }

  // Check if the player has already played this turn.
  if (p->hasPlayed())
    {
      LOG4("Cannot declare action: this player already does something this turn");
      r_->sendIllegal(pkt->token, pkt->client_id, ERR_HASPLAYED);
      return false;
    }

  // Check if the player is not performing an other action.
  if (p->getAction() != DCL_NONE)
    {
      LOG4("Cannot declare action: this player is performing an action");
      r_->sendIllegal(pkt->token, pkt->client_id, ERR_ISPLAYING);
      return false;
    }

  // If he is stunned or out of the field, he can't do it
  if (p->getStatus() != STA_STANDING &&p->getStatus() != STA_PRONE)
    return false;

  // Check if he can declare this action
  if (pkt->action == DCL_BLITZ)
    { 
      if (blitz_done_)
        {
          LOG4("Cannot do more than one blitz in a single turn.");
          r_->sendIllegal(pkt->token, p->getId(), ERR_SINGLEACTIONUSED);
          return false;
        }
      blitz_done_ = true;
    }
  if (pkt->action == DCL_PASS)
    { 
      if (pass_done_)
        {
          LOG4("Cannot do more than one pass in a single turn.");
          r_->sendIllegal(pkt->token, p->getId(), ERR_SINGLEACTIONUSED);
          return false;
        }
      pass_done_ = true;
    }

  active_player_id_ = p->getId();
  return true;
}

bool STeam::canDoAction(const Packet* pkt, SPlayer* p)
{
  // Check if it's the team turn.
  if (r_->getCurrentTeamId() != pkt->client_id)
    {
      LOG4("Cannot do action: not team turn");
      LOG4("%1, %2",r_->getCurrentTeamId(), r_->getState());
      r_->sendIllegal(pkt->token, pkt->client_id, ERR_WRONGCONTEXT);
      return false;
    }
  // Check if it is the current player
  if (p != player_[active_player_id_])
    {
      LOG4("Cannot do action: an other player is performing an action");
      r_->sendIllegal(pkt->token, pkt->client_id);
      return false;
    }
  // If he is stunned or out of the field, he can't do it
  if (pkt->token == MSG_STANDUP && p->getStatus() != STA_PRONE)
    {
      LOG4("Cannot do action: player must be prone to stand up");
      r_->sendIllegal(pkt->token, pkt->client_id);
      return false;
    }
  if (pkt->token != MSG_STANDUP && p->getStatus() != STA_STANDING)
    {
      LOG4("Cannot do action: player must stand up");
      r_->sendIllegal(pkt->token, pkt->client_id);
      return false;
    }
  // Check the action declared is fit to the action
  if ((p->getAction() == DCL_MOVE
        && (pkt->token == MSG_PASS || pkt->token == MSG_BLOCK))
      || (p->getAction() == DCL_BLOCK
        && (pkt->token == MSG_PASS || pkt->token == MSG_MOVE))
      || (p->getAction() == DCL_BLITZ && pkt->token == MSG_PASS)
      || (p->getAction() == DCL_PASS && pkt->token == MSG_BLOCK))
    {
      LOG4("Cannot do action: declare action does not fit");
      r_->sendIllegal(pkt->token, pkt->client_id);
      return false;
    }
  // Cannot try to stand up more than once
  if (pkt->token == MSG_STANDUP)
    {
      if (p->getMaRemain() != p->getMa())
        {
          LOG4("Cannot try to stand up more than once.");
          r_->sendIllegal(pkt->token, p->getId());
          return false;
        }
    }
  return true;
}
