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
#include "CPlayer.hh"

CPlayer::CPlayer(CRules* r, const MsgPlayerInfo* m)
  : Player(m->player_id, m->client_id),
    r_(r)
{
  r_->HANDLE_F_WITH(MSG_PLAYERPOS, CPlayer, this, msgPlayerPos, filterPlayerPos, GS_INITKICKOFF | GS_COACHBOTH);
  r_->HANDLE_F_WITH(ACT_MOVE, CPlayer, this, msgPlayerMove, filterPlayerMove, GS_COACHBOTH);
  r_->HANDLE_F_WITH(MSG_PLAYERKNOCKED, CPlayer, this, msgPlayerKnocked, filterPlayerKnocked, GS_COACHBOTH);
  r_->HANDLE_F_WITH(MSG_PLAYERSTATUS, CPlayer, this, msgPlayerStatus, filterPlayerStatus, GS_INITKICKOFF | GS_COACHBOTH);
  ma_ = m->ma;
  st_ = m->st;
  ag_ = m->ag;
  av_ = m->av;
  name_ = packetToString(m->name);
  player_position_ = m->player_position;
  player_picture_ = packetToString(m->player_img);
	status_ = STA_RESERVE;

  LOG6("Create player(" << (unsigned)this << "): id: " << id_ << " team_id " << team_id_);
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
  f->setPlayer(pos_, this);
}

void CPlayer::subMa(int dep)
{
  ma_remain_ -= dep;
}

bool CPlayer::standUp()
{
 	if (status_ != STA_PRONE)
    {
      LOG2("You are not prone.");
      return false;
    }
		
	ActStandUp pkt;
  pkt.player_id = id_;
  r_->sendPacket(pkt);
  return true;
}

bool CPlayer::move(const Position& to)
{
  CField* f = r_->getField();

  if (status_ != STA_STANDING)
    {
      LOG2("You are not in a standing position.");
      return false;
    }
  if (to == pos_)
    {
      LOG2("You are already on " << pos_);
      return false;
    }
  if (!r_->getField()->intoField(to))
    {
      LOG2("Trying to move player outside of field: " << to);
      return false;
    }

  ActMove pkt;
  const PosList& p = f->getPath(pos_, to, this);
  if (p.empty())
    {
      LOG2("Can not go from " << pos_ << " to " << to << ". Sorry.");
      return false;
    }
  pkt.player_id = id_;
  pkt.nb_move = 0;
  PosIter it;
  for (it = p.begin(); it != p.end(); ++it)
    {
      LOG3("Moving: add point " << *it);
      pkt.moves[pkt.nb_move].row = it->row;
      pkt.moves[pkt.nb_move].col = it->col;
      pkt.nb_move++;
    }
  r_->sendPacket(pkt);
  return true;
}

bool CPlayer::block(const Position& to)
{
  CField* f = r_->getField();

  CPlayer* opponent = f->getPlayer(to);
  if (opponent == NULL
      || opponent->getTeamId() == getTeamId()
      || !getPosition().isNear(opponent->getPosition()))
    return false;
  ActBlock pkt;
  pkt.player_id = id_;
  pkt.opponent_id = opponent->getId();
  r_->sendPacket(pkt);
  return true;
}


/*
** Messages.
*/

void CPlayer::msgPlayerPos(const MsgPlayerPos* m)
{
  Position pos(m->row, m->col);
  setPosition(pos);
  r_->onEvent(m);
}

void CPlayer::msgPlayerMove(const ActMove* m)
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
						&&status_ != STA_PRONE
						&&status_ != STA_STUNNED)
				break;
    case STA_KO:
    case STA_INJURED:
      r_->getField()->setPlayer(pos_, NULL);
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
/*
** Message filters.
*/

bool CPlayer::filterPlayerPos(const MsgPlayerPos* m)
{
  if (m->client_id != team_id_ || m->player_id != id_)
    return false;
  return true;
}

bool CPlayer::filterPlayerMove(const ActMove* m)
{
  if (m->client_id != team_id_ || m->player_id != id_)
    return false;
  return true;
}

bool CPlayer::filterPlayerKnocked(const MsgPlayerKnocked* m)
{
  if (m->client_id != team_id_ || m->player_id != id_)
    return false;
  return true;
}

bool CPlayer::filterPlayerStatus(const MsgPlayerStatus* m)
{
  if (m->client_id != team_id_ || m->player_id != id_)
    return false;
  return true;
}
