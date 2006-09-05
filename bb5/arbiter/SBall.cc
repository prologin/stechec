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
    owner_(NULL)
{
  r_->HANDLE_WITH(MSG_BALLPOS, SBall, this, msgPlaceBall, GS_INITKICKOFF);
}

SPlayer* SBall::getOwner()
{
  return owner_;
}

void SBall::msgPlaceBall(const MsgBallPos* m)
{
  MsgBallPos mesg;
  SPlayer* p;
  
  // Called on half init. Check if this is the kicking team that
  // place the ball.
  if (r_->getCurrentOpponentTeamId() != m->client_id)
    {
      LOG4("Ball: unallowed kick-off from " << m->client_id << ".");
      r_->sendIllegal(MSG_BALLPOS, m->client_id);
      return;
    }

  // So everybody will be aware of where the kicker put it.
  r_->sendPacket(*m);
  
  pos_.row = m->row;
  pos_.col = m->col;
  if (invalidBallPlacement())
    {
      LOG4("Ball: wrong kick-off from " << m->client_id
	   << ". Not in receiver's field: " << pos_);
      goto end;
    }

  scatter(Dice(6).roll());
  if (invalidBallPlacement())
    {
      LOG4("Ball: kick-off from " << m->client_id
	   << ". Scattered out of receiver's field: " << pos_);
      goto end;
    }

  // Catch it if there is somebody, else bounce it.
  p = r_->getField()->getPlayer(pos_);
  if (p != NULL)
    catchBall(p, 1);
  else
    bounce(1);

 end:
  mesg.row = pos_.row;
  mesg.col = pos_.col;
  r_->sendPacket(mesg);
}

bool SBall::invalidBallPlacement()
{
  SField* f = r_->getField();
  if (!f->intoField(pos_)
      || (r_->getCurrentTeamId() == 0 && pos_.row > ROWS / 2)
      || (r_->getCurrentTeamId() == 1 && pos_.row < ROWS / 2))
    {
      // FIXME: give the ball to one player of receiver team
      pos_.col = COLS / 2;
      pos_.row = r_->getCurrentTeamId() == 0 ? ROWS / 4 : ROWS * 3 / 4; 
      return true;
    }
  return false;
}

void SBall::afterBounce(const Position& delta, int amplitude)
{
  SField* f = r_->getField();
  Position to(pos_ + (amplitude * delta));

  LOG5("Ball: bounce from " << pos_ << " to " << to);
  assert(f->intoField(pos_)); // Ball must be in-field before.
  
  if (f->intoField(to))
    {
      pos_ = to;
      SPlayer *p = f->getPlayer(to);
      if (p != NULL)
	catchBall(p, 0);
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

  MsgBallPos mesg;
  mesg.row = pos_.row;
  mesg.col = pos_.col;
  r_->sendPacket(mesg);
}

void SBall::bounce(int nb)
{
  owner_ = NULL;
  Dice d(8);
  switch(d.roll())
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
   Dice d(8);
   switch(d.roll())
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
	if ( p->getTeamId() == r_->getCurrentOpponentTeamId())
		opponent_team_id = r_->getCurrentOpponentTeamId();
	else
	  opponent_team_id = r_->getCurrentTeamId();	
  int nb_tackles = f->getNbTackleZone(opponent_team_id, pos_);
  if (!p->tryAction(modifier - nb_tackles))
    {
      LOG5("Ball: player has failed to pick it at " << pos_);
      bounce();
      return false;
    }
  LOG5("Ball: player successfully took it at " << pos_);
  owner_ = p;
  return true;
}

// Spectators throwing the ball
void SBall::throwin()
{
  Position d(0, 0); // Direction of the throw.
  int reach = Dice(6).roll();
  LOG5("Ball: gets throwed by spectators at " << pos_);

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
  if (d.row)
    {
      switch(Dice(3).roll())
			{
				case 1: afterBounce(Position(1, d.col), reach); break;
				case 2: afterBounce(Position(0, d.col), reach); break;
				case 3: afterBounce(Position(-1, d.col), reach); break;
			}
    }
  else
    {
      switch(Dice(3).roll())
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
	MsgBallPos mesg;
	mesg.row = pos_.row;
  mesg.col = pos_.col;
  r_->sendPacket(mesg);
}
