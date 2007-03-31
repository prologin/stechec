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

#include "Dice.hh"
#include "SRules.hh"
#include "SBall.hh"

SBall::SBall(SRules* r)
  : r_(r),
    owner_(NULL),
    thrown_(false)
{
  r_->HANDLE_WITH(MSG_BALLPOS, SBall, this, msgPlaceBall, GS_KICKOFF);
  r_->HANDLE_WITH(MSG_GIVEBALL, SBall, this, msgGiveBall, GS_TOUCHBACK);
}

SPlayer* SBall::getOwner()
{
  return owner_;
}

void SBall::setPosition(const Position& pos, bool advertise_client)
{
  if (advertise_client && pos_ != pos)
    sendPosition();
  pos_ = pos;
}

void SBall::msgPlaceBall(const MsgBallPos* m)
{
  SPlayer* p;
  
  // Called on kick off.
  // Check if this is the kicking team that place the ball.
  if (r_->getCurrentOpponentTeamId() != m->client_id)
    {
      LOG4("Ball: unallowed kick-off from %1.", m->client_id);
      r_->sendIllegal(MSG_BALLPOS, m->client_id);
      return;
    }

  // First msg resend to client: where the kicker put it.
  r_->sendPacket(*m);
  
  // Check ball's wished placement.
  pos_.row = m->row;
  pos_.col = m->col;
  if (invalidBallPlacement())
    {
      LOG4("Ball: wrong kick-off from %1. Not in receiver's field: %2", m->client_id, pos_);
      return;
    }

  // Scatter the ball (still in air).
  scatter(r_->getDice()->roll("scatter len"));

  // Check ball's landing.
  if (invalidBallPlacement())
    {
      LOG4("Ball: kick-off from %1. Scattered out of receiver's field: %2", m->client_id, pos_);
      return;
    }

  // Catch it if there is somebody, else bounce it.
  // Can't use the methods catchBall and bounce : must control if it goes out.
  bool has_scattered = false;
  p = r_->getField()->getPlayer(pos_);
  while (p != NULL||!has_scattered)
    {
      if (p == NULL)
        {
          scatter(1);
          if (invalidBallPlacement())
            {
              LOG4("Ball: kick-off from %1. Bounced out of receiver's field: %2", m->client_id, pos_);
              return;
            }
          has_scattered = true;
          p = r_->getField()->getPlayer(pos_);
        }
      else
        {
          p->action_attempted_ = R_CATCH;
          int nb_tackles = r_->getField()->getNbTackleZone(1 - p->getTeamId(), pos_);
          if (!p->tryAction(0 - nb_tackles))
            {
              LOG5("Ball: player has failed to catch it at %1", pos_);
              scatter(1);
              if (invalidBallPlacement())
                {
                  LOG4("Ball: kick-off from %1. Rebounded out of receiver's field: %2", m->client_id, pos_);
                  return;
                }
              has_scattered = true;
              p = r_->getField()->getPlayer(pos_);
            }
          else
            {
              LOG5("Ball: player successfully catch it at %1", pos_);
              owner_ = p;
              p = NULL;
              has_scattered = true;
            }
        }
    }

  // First msg resent to client: where it eventually goes.
  sendPosition();

  r_->kickoffFinished();
}

void SBall::msgGiveBall(const MsgGiveBall* m)
{
  SPlayer* p;
  
  // Called on half init, when the ball goes out of the pitch
  if (r_->getCurrentTeamId() != m->client_id)
    {
      LOG4("Ball: coach %1 unallowed to give the ball.", m->client_id);
      r_->sendIllegal(MSG_GIVEBALL, m->client_id);
      return;
    }

  p = r_->getTeam(m->client_id)->getPlayer(m->player_id);

  if (p->getStatus() != STA_STANDING)
    {
      LOG4("Ball: player `%1' can't carry the ball.", p->getId());
      r_->sendIllegal(MSG_GIVEBALL, m->client_id);
      return;
    }

  owner_ = p;
  pos_ = Position(p->getPosition());

  // Second msg resent to client: where it goes.
  sendPosition();

  r_->kickoffFinished();
}

bool SBall::invalidBallPlacement()
{
  SField* f = r_->getField();
  if (!f->intoField(pos_)
      || (r_->getCurrentTeamId() == 0 && pos_.row >= ROWS / 2)
      || (r_->getCurrentTeamId() == 1 && pos_.row < ROWS / 2))
    {
      pos_.col = -1;
      pos_.row = -1;
      r_->setState(GS_TOUCHBACK);
      r_->sendPacket(MsgGiveBall(r_->getCurrentTeamId()));
      return true;
    }
  return false;
}

void SBall::afterBounce(const Position& delta, int amplitude)
{
  SField* f = r_->getField();
  Position to(pos_ + (amplitude * delta));

  LOG5("Ball: bounce from %1 to %2", pos_, to);
  assert(f->intoField(pos_)); // Ball must be in-field before.
  
  if (f->intoField(to))
    {
      pos_ = to;
      SPlayer *p = f->getPlayer(to);
      if (p != NULL)
        {
          if (p->getStatus() == STA_STANDING)
            {
              p->action_attempted_ = R_CATCH;
              p->reroll_enabled_ = (r_->getTeam(p->getTeamId())->canUseReroll()
                  &&p->getTeamId() == r_->getCurrentTeamId());
              if (catchBall(p, 0))
                {
                  if (thrown_ && p->getTeamId() != r_->getCurrentTeamId())
                    {
                      r_->getCurrentTeam()->turnover(TOM_LOSTBALL);
                    }
                  thrown_ = false;
                }
            }
          else
            bounce();
        }
      else
        {
          if (thrown_)
            {
              r_->getCurrentTeam()->turnover(TOM_LOSTBALL);
              thrown_ = false;
            }
        }
    }
  else
    {
      // Find the border line, where the ball disappear.
      for (int i = 0; i < amplitude; i++)
        {
          pos_ += delta;
          if (pos_.col == 0 || pos_.col == COLS - 1
              || pos_.row == 0 || pos_.row == ROWS - 1)
            {
              throwin();
              break;
            }
        }
    }

  sendPosition();
}

void SBall::bounce(int nb)
{
  owner_ = NULL;
  switch (r_->getDice()->roll("bounce", D8))
    {
    case N: afterBounce(Position(-1, 0), nb); break;
    case S: afterBounce(Position(+1, 0), nb); break;
    case E: afterBounce(Position(0, +1), nb); break;
    case W: afterBounce(Position(0, -1), nb); break;
    case NE: afterBounce(Position(-1, +1), nb); break;
    case NW: afterBounce(Position(-1, -1), nb); break;
    case SE: afterBounce(Position(+1, +1), nb); break;
    case SW: afterBounce(Position(+1, -1), nb); break;
    }
}

void SBall::scatter(int nb)
{
  switch (r_->getDice()->roll("scatter", D8))
    {
    case N: pos_ += Position(-nb, 0); break;
    case S: pos_ += Position(+nb, 0); break;
    case E: pos_ += Position(0, +nb); break;
    case W: pos_ += Position(0, -nb); break;
    case NE: pos_ += Position(-nb, +nb); break;
    case NW: pos_ += Position(-nb, -nb); break;
    case SE: pos_ += Position(+nb, +nb); break;
    case SW: pos_ += Position(+nb, -nb); break;
    }
}

// Player trying to catch the ball
// Note: a player has not to be at ball position.
bool SBall::catchBall(SPlayer *p, int modifier)
{
  SField* f = r_->getField();
  int opponent_team_id;
  if ( p->getTeamId() == r_->getCurrentTeamId())
    opponent_team_id = r_->getCurrentOpponentTeamId();
  else
    opponent_team_id = r_->getCurrentTeamId();
  int nb_tackles = f->getNbTackleZone(opponent_team_id, pos_);
  if (!p->tryAction(modifier - nb_tackles))
    {
      LOG5("Ball: player has failed to pick it at %1", pos_);
      if (p->reroll_enabled_)
        return false;
      bounce();
      return false;
    }
  LOG5("Ball: player successfully took it at %1", pos_);
  owner_ = p;
  return true;
}

// Spectators throwing the ball
void SBall::throwin()
{
  owner_ = NULL;
  Position d(0, 0); // Direction of the throw.
  int reach = r_->getDice()->roll("ball throwin", D6, 2) - 1;
  LOG5("Ball: gets throwed by spectators at %1", pos_);

  // Get the border we just crossed:
  if (pos_.col == 0)
    d.col = 1;
  else if (pos_.col == COLS - 1)
    d.col = -1;

  if (pos_.row == 0)
    d.row = 1;
  else if (pos_.row == ROWS - 1)
    d.row = -1;

  // Throw the ball
  if (d.row == 0)
    {
      switch (r_->getDice()->roll("throwin dir", D3))
      {
        case 1: afterBounce(Position(1, d.col), reach); break;
        case 2: afterBounce(Position(0, d.col), reach); break;
        case 3: afterBounce(Position(-1, d.col), reach); break;
      }
    }
  else
    {
      switch (r_->getDice()->roll("throwin dir", D3))
      {
        case 1: afterBounce(Position(d.row, 1), reach); break;
        case 2: afterBounce(Position(d.row, 0), reach); break;
        case 3: afterBounce(Position(d.row, -1), reach); break;
      }
    }
}

void SBall::removeFromField()
{
  owner_ = NULL;
  pos_ = Position(-1,-1);
  sendPosition();
}

void SBall::setThrown()
{
  thrown_ = true;
}

void SBall::sendPosition()
{
  MsgBallPos mesg;
  mesg.row = pos_.row;
  mesg.col = pos_.col;
  r_->sendPacket(mesg);
}

