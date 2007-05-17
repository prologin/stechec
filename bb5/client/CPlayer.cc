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

#include "CRules.hh"
#include "CField.hh"
#include "CPlayer.hh"

CPlayer::CPlayer(CRules* r, const MsgPlayerCreate* m)
  : Player(m),
    r_(r)
{
  position_name_ = packetToString(m->position_name);
  player_position_ = m->player_position;
  player_picture_ = packetToString(m->player_img);
}

CPlayer::~CPlayer()
{
}

void CPlayer::setPosition(const Position& pos)
{
  CField* f = r_->getField();
  if (f->intoField(pos_))
    f->setPlayer(pos_, NULL);
  pos_ = pos;
  if (f->intoField(pos_))
    f->setPlayer(pos_, this);
}

void CPlayer::subMa(int dep)
{
  ma_remain_ -= dep;
}

int CPlayer::declareAction(enum eDeclaredAction action)
{
  if (has_played_)
    {
      LOG2("This player has already played.");
      return INVALID_ACTION;
    }
    
  if (action_ != DCL_NONE)
    {
      LOG2("This player is performing an action.");
      return INVALID_ACTION;
    }
  
  MsgDeclare pkt;
  pkt.player_id = id_;
  pkt.action = action;
  r_->sendPacket(pkt);
  return SUCCESS;
}

int CPlayer::move(const Position& to)
{
  CField* f = r_->getField();

  if (status_ != STA_STANDING)
    {
      LOG2("You are not in a standing position.");
      return INVALID_ACTION;
    }
  if (to == pos_)
    {
      LOG2("You are already on %1", pos_);
      return INVALID_ACTION;
    }
  if (!r_->getField()->intoField(to))
    {
      LOG2("Trying to move player outside of field: %1", to);
      return INVALID_ACTION;
    }
  if (has_played_)
    {
      LOG2("Player has already played this turn");
      return INVALID_ACTION;
    }
  if (action_ == DCL_NONE)
    {
      LOG2("Player must declare an action before moving");
      return INVALID_ACTION;
    }
  if (action_ == DCL_BLOCK)
    {
      LOG2("Player can't move in a block action");
      return INVALID_ACTION;
    }

  MsgMove pkt;
  const PosList& p = f->getPath(pos_, to, this);
  if (p.empty())
    {
      LOG2("Can not go from %1 to %2. Sorry.", pos_, to);
      return INVALID_ACTION;
    }
  pkt.player_id = id_;
  pkt.nb_move = 0;
  PosIter it;
  for (it = p.begin(); it != p.end(); ++it)
    {
      LOG3("Moving: add point %1", *it);
      pkt.moves[pkt.nb_move].row = it->row;
      pkt.moves[pkt.nb_move].col = it->col;
      pkt.nb_move++;
    }
  r_->sendPacket(pkt);
  return SUCCESS;
}

int CPlayer::standUp()
{
  if (status_ != STA_PRONE)
    return INVALID_ACTION;
  if (has_played_)
    {
      LOG2("Player has already played this turn");
      return INVALID_ACTION;
    }
  if (action_ == DCL_NONE)
    {
      LOG2("Player must declare an action before standing up");
      return INVALID_ACTION;
    }
  if (action_ == DCL_BLOCK)
    {
      LOG2("Player can't stand up in a block action");
      return INVALID_ACTION;
    }
		
  MsgStandUp pkt;
  pkt.player_id = id_;
  r_->sendPacket(pkt);
  return SUCCESS;
}

int CPlayer::block(CPlayer* opponent)
{
  if (opponent == NULL
      || opponent->getTeamId() == getTeamId()
      || !getPosition().isNear(opponent->getPosition())
      || opponent->getStatus() != STA_STANDING)
    return INVALID_ACTION;
  
  if (has_played_)
    {
      LOG2("Player has already played this turn");
      return INVALID_ACTION;
    }
  if (action_ == DCL_NONE)
    {
      LOG2("Player must declare an action before blocking");
      return INVALID_ACTION;
    }
  if (action_ == DCL_MOVE)
    {
      LOG2("Player can't block in a move action");
      return INVALID_ACTION;
    }
  if (action_ == DCL_PASS)
    {
      LOG2("Player can't block in a pass action");
      return INVALID_ACTION;
    }
      
  MsgBlock pkt;
  pkt.player_id = id_;
  pkt.opponent_id = opponent->getId();
  r_->sendPacket(pkt);
  return SUCCESS;
}

int CPlayer::pass(const Position& to)
{
  if (has_played_)
    {
      LOG2("Player has already played this turn");
      return INVALID_ACTION;
    }
  if (action_ == DCL_NONE)
    {
      LOG2("Player must declare an action before throwing");
      return INVALID_ACTION;
    }
  if (action_ != DCL_PASS)
    {
      LOG2("Player must declare a pass action to throw");
      return INVALID_ACTION;
    }
	
  MsgPass pkt;
  pkt.player_id = id_;
  pkt.dest_row = to.row;
  pkt.dest_col = to.col;
  r_->sendPacket(pkt);
  return SUCCESS;
}


int CPlayer::getPlayerPosition() const
{
  return player_position_;
}

const std::string& CPlayer::getPlayerPicture() const
{
  return player_picture_;
}


/*
** Messages.
*/

void CPlayer::msgDeclareAction(const MsgDeclare* m)
{
  // End of action
  if (m->action == DCL_NONE)
    {
      has_played_ = true;
      r_->onEvent(m);
      LOG2("player %1 ends a %2 action", id_, stringify(action_));
      return;
    }
  
  // Beginning of the action
  action_ = (enum eDeclaredAction)m->action;
  r_->onEvent(m);
  LOG2("player %1 begins a %2 action", id_, stringify(action_));
}

void CPlayer::msgPlayerPos(const MsgPlayerPos* m)
{
  Position pos(m->row, m->col);
  setPosition(pos);
  r_->onEvent(m);
}

void CPlayer::msgPlayerMove(const MsgMove* m)
{
  // FIXME: should consider all steps, to have a smothing graphical effect.
  Position pos;
  pos.row = m->moves[m->nb_move - 1].row;
  pos.col = m->moves[m->nb_move - 1].col;
  setPosition(pos);
  subMa(m->nb_move);
  r_->onEvent(m);
}

void CPlayer::msgPlayerKnocked(const MsgPlayerKnocked* m)
{
  r_->onEvent(m);
}

void CPlayer::msgPlayerStatus(const MsgPlayerStatus* m)
{

  switch ((enum eStatus)m->status)
    {
    case STA_STANDING: 
      if (status_ == STA_PRONE)
	ma_remain_ = ma_ - 3;
    case STA_PRONE:
    case STA_STUNNED:
      break;
    case STA_RESERVE:
      if (status_ != STA_STANDING
	  && status_ != STA_PRONE
	  && status_ != STA_STUNNED)
	break;
    case STA_KO:
    case STA_INJURED:
      r_->getField()->setPlayer(pos_, NULL);
      setPosition(Position(-1, -1));
      break;      
    case STA_UNASSIGNED:
      WARN("Can't set player in 'unassigned' state");
      break;
    default:
      LOG3("You can't set this state from outside...");
      break;
    }
  status_ = (enum eStatus)m->status;
  r_->onEvent(m);
}

void CPlayer::msgPlayerKO(const MsgPlayerKO* m)
{
  r_->onEvent(m);
}

void CPlayer::msgSkill(const MsgSkill* m)
{
  enum eSkill skill = (enum eSkill) m->skill;
  if (r_->getState() == GS_REROLL)
    {
      r_->setState(team_id_ == 0 ? GS_COACH1 : GS_COACH2);
      if (m->choice == 1)
        useSkill(skill);
    }
  else if (r_->getState() == GS_SKILL)
    r_->setState(team_id_ == 0 ? GS_COACH1 : GS_COACH2);
  else if ((m->choice == -1) && (m->client_id == r_->getOurTeamId()))
    r_->setState(GS_SKILL);
  r_->onEvent(m);
}

