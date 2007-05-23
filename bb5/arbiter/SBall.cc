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
  pos_ = Position(-1,-1);
  r_->HANDLE_WITH(MSG_BALLPOS, SBall, this, msgPlaceBall, GS_KICKOFF);
  r_->HANDLE_WITH(MSG_GIVEBALL, SBall, this, msgGiveBall, GS_TOUCHBACK);
}

SPlayer* SBall::getOwner() const
{
  return owner_;
}

void SBall::setOwner(SPlayer* p)
{
  owner_ = p;
}

void SBall::setPosition(const Position& pos, bool advertise_client)
{
  if (advertise_client && pos_ != pos)
  {
    pos_ = pos;
    sendPosition();
  }
  else
    pos_ = pos;
}

/*
** Kick-off.
*/

void SBall::msgPlaceBall(const MsgBallPos* m)
{
  SPlayer* p;
  Position to;
  // Checks that the request comes from the kicking team.
  if (r_->getCurrentOpponentTeamId() != m->client_id)
    {
      LOG4("Team %1 is not allowed to kick off the ball.", m->client_id);
      r_->sendIllegal(MSG_BALLPOS, m->client_id, ERR_WRONGCONTEXT);
      return;
    }
  // Checks requested ball placement.
  to = Position(m->row, m->col);
  if (invalidKickoffDestination(to))
    {
      LOG4("Team %1 can't place the ball at %1, out of receiver's field.", m->client_id, pos_);
      r_->sendIllegal(MSG_BALLPOS, m->client_id, ERR_INVALIDBALLPLACEMENT);
      return;
    }
  r_->setState(GS_KICKOFF);
  setPosition(to, true);
  // Scatters the ball (still in the air).
  scatter(r_->getDice()->roll("scatter length"));
  // Checks if the ball lands out of the field.
  if (invalidKickoffDestination(pos_))
    {
      LOG4("Kick-off from team %1 is scattered out of receiver field: %2.", m->client_id, pos_);
      touchback();
      return;
    }

  sendPosition();
  p = r_->getField()->getPlayer(pos_);
  if (p != NULL && p->getStatus() == STA_STANDING)
    {
      p->tryCatchBall(false);
    }
  else
    {
      bounce();
    }
}

void SBall::touchback()
{
  LOG5("Coach %1 is awarded a touchback.", r_->getCurrentTeamId());
  r_->setState(GS_TOUCHBACK);
  r_->sendPacket(MsgGiveBall(r_->getCurrentTeamId()));
}

void SBall::msgGiveBall(const MsgGiveBall* m)
{
  SPlayer* p;
  if (r_->getCurrentTeamId() != m->client_id)
    {
      LOG4("Coach %1 is not allowed to give the ball.", m->client_id);
      r_->sendIllegal(MSG_GIVEBALL, m->client_id, ERR_WRONGCONTEXT);
      return;
    }
  p = r_->getTeam(m->client_id)->getPlayer(m->player_id);
  if (p->getStatus() != STA_STANDING)
    {
      LOG4("Player `%1' can not carry the ball.", p->getId());
      r_->sendIllegal(MSG_GIVEBALL, m->client_id, ERR_CANNOTCARRYTHEBALL);
      return;
    }
  owner_ = p;
  setPosition(p->getPosition(), true);
  r_->finishKickoff();
}

inline bool SBall::invalidKickoffDestination(const Position& pos) const
{
  return (!r_->getField()->intoField(pos)
      || (r_->getCurrentTeamId() == 0 && pos.row >= ROWS / 2)
      || (r_->getCurrentTeamId() == 1 && pos.row < ROWS / 2));
}

/*
**
*/

void SBall::afterBounce(const Position& delta, int amplitude)
{
  SField* f = r_->getField();
  SPlayer* p;
  Position to(pos_ + (amplitude * delta));
  LOG5("Ball bounces from %1 to %2", pos_, to);
  if ((r_->getState() == GS_KICKOFF) && invalidKickoffDestination(to))
    {
      touchback();
    }
  else if (f->intoField(to))
    {
      setPosition(to, true);
      p = f->getPlayer(to);
      if (p != NULL)
        {
          if (p->getStatus() == STA_STANDING)
            {
              p->tryCatchBall(false);
            }
          else
            {
              bounce();
            }
        }
      else
        {
          if (thrown_)
            {
              r_->turnover(TOM_LOSTBALL);
            }
          else
            {
              r_->getActionHandler()->process();
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
              sendPosition();
              throwIn();
              break;
            }
        }
    }
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

// Spectators throwing the ball
void SBall::throwIn()
{
  owner_ = NULL;
  Position d(0, 0); // Direction of the throw.
  int reach = r_->getDice()->roll("ball throw in", D6, 2) - 1;
  LOG5("Ball gets thrown by spectators from %1", pos_);

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
      switch (r_->getDice()->roll("throw in dir", D3))
      {
        case 1: afterBounce(Position(1, d.col), reach); break;
        case 2: afterBounce(Position(0, d.col), reach); break;
        case 3: afterBounce(Position(-1, d.col), reach); break;
      }
    }
  else
    {
      switch (r_->getDice()->roll("throw in dir", D3))
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
  thrown_ = false;
  pos_ = Position(-1,-1);
  sendPosition();
}

void SBall::resetTurn()
{
  thrown_ = false;
}

void SBall::setThrown()
{
  thrown_ = true;
}

bool SBall::hasBeenThrown()
{
  return thrown_;
}

void SBall::sendPosition()
{
  MsgBallPos mesg;
  mesg.row = pos_.row;
  mesg.col = pos_.col;
  r_->sendPacket(mesg);
}

