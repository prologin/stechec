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
  : r_(r)
{
  r_->HANDLE_WITH(MSG_BALLPOS, SBall, this, msgPlaceBall, GS_INITHALF);
}

void SBall::msgPlaceBall(const MsgBallPos* m)
{
  // Called on half init. Check if this is the kicking team that
  // place the ball.
  if (r_->getCurrentTeamId() == m->client_id)
    {
      LOG4("Unallowed kick-off from " << m->client_id << ".");
      r_->sendIllegal(MSG_BALLPOS, m->client_id);
      return;
    }

  SField* f = r_->getField();
  Position ball_pos(m->row, m->col);
  if (!f->intoField(ball_pos))
    {
      LOG4("Wrong kick-off from " << m->client_id << ". Not in field: " << ball_pos);
      r_->sendIllegal(MSG_BALLPOS, m->client_id);
      return;
    }
  SBall::scatter(ball_pos, Dice(6).roll());
  if (!f->intoField(ball_pos))
    {
      LOG4("kick-off from " << m->client_id << ". scattered out of field: " << ball_pos);
      r_->sendIllegal(MSG_BALLPOS, m->client_id);
      return;
    }
  MsgBallPos mesg;
  mesg.row = ball_pos.row;
  mesg.col = ball_pos.col;
  r_->sendPacket(mesg);
}

void SBall::moveDelta(const Position& delta)
{
  Position to(pos_ + delta);

  if (r_->getField()->intoField(to))
    {
      // FIXME: check if a player can catch it.
      pos_ = to;
    }
  else
    {
      throwin(pos_, to);
    }
}

void SBall::scatter(Position& pos, int nb)
{
  Dice d(8);
  switch(d.roll())
    {
    case N: pos += Position(-nb,0); break;
    case S: pos += Position(+nb,0); break;
    case E: pos += Position(0,+nb); break;
    case W: pos += Position(0,-nb); break;
    case NE: pos += Position(-nb,+nb); break;
    case NW: pos += Position(-nb,-nb); break;
    case SE: pos += Position(+nb,+nb); break;
    case SW: pos += Position(+nb,-nb); break;
    }
}

void SBall::bounce(int nb)
{
  Dice d(8);
  switch(d.roll())
    {
    case N: moveDelta(Position(-nb,0)); break;
    case S: moveDelta(Position(+nb,0)); break;
    case E: moveDelta(Position(0,+nb)); break;
    case W: moveDelta(Position(0,-nb)); break;
    case NE: moveDelta(Position(-nb,+nb)); break;
    case NW: moveDelta(Position(-nb,-nb)); break;
    case SE: moveDelta(Position(+nb,+nb)); break;
    case SW: moveDelta(Position(+nb,-nb)); break;
    }
}

//FIXME:get the to position into account
void SBall::throwin(const Position& from,const Position& to)
{
  Dice d(3);
  int drow=0, dcol=0; //used to determine direction of the throw
  int reach=Dice(6).roll();
  //get the border we just crossed :
  if (from.col == 0)
  {
    dcol = 1;
  }
  else if (from.col == (COLS-1))
  {
    dcol = -1;
  }
  if (from.row == 0)
  {
    drow = 1;
  }
  else if (from.row == (ROWS-1))
  {
    drow = -1;
  }
  //throw the ball
  if (dcol)
  {
      switch(d.roll())
      {
      case 1: moveDelta(Position(reach,reach*dcol));  break;
      case 2: moveDelta(Position(0,reach*dcol));      break;
      case 3: moveDelta(Position(-reach,reach*dcol)); break;
      }
  }
  else //drow
  {
      switch(d.roll())
      {
      case 1: moveDelta(Position(reach*drow,reach));  break;
      case 2: moveDelta(Position(reach*drow,0));      break;
      case 3: moveDelta(Position(reach*drow,-reach)); break;
      }
  }
}
