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

CPlayer::CPlayer(CRules* r, const MsgPlayerCreate* m)
  : Player(m),
    r_(r)
{
  player_position_ = m->player_position;
  player_picture_ = packetToString(m->player_img);

  r_->HANDLE_F_WITH(MSG_PLAYERPOS, CPlayer, this, msgPlayerPos, filterPlayerPos, GS_ALL | GS_INITGAME | GS_INITKICKOFF | GS_COACHBOTH);
  r_->HANDLE_F_WITH(ACT_MOVE, CPlayer, this, msgPlayerMove, filterPlayerMove, GS_COACHBOTH | GS_REROLL);
  r_->HANDLE_F_WITH(MSG_PLAYERKNOCKED, CPlayer, this, msgPlayerKnocked, filterPlayerKnocked, GS_COACHBOTH | GS_REROLL);
  r_->HANDLE_F_WITH(MSG_PLAYERSTATUS, CPlayer, this, msgPlayerStatus, filterPlayerStatus, GS_ALL);
  r_->HANDLE_F_WITH(MSG_PLAYERKO, CPlayer, this, msgPlayerKO, filterPlayerKO, GS_INITKICKOFF);
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

bool CPlayer::standUp(enum eAction action)
{
  if (status_ != STA_PRONE)
    {
      LOG2("You are not prone.");
      return false;
    }
		
  ActStandUp pkt;
  pkt.player_id = id_;
  pkt.action = action;
  r_->sendPacket(pkt);
  return true;
}

bool CPlayer::move(const Position& to, enum eAction action)
{
  CField* f = r_->getField();

  if (status_ != STA_STANDING)
    {
      LOG2("You are not in a standing position.");
      return false;
    }
  if (to == pos_)
    {
      LOG2("You are already on %1", pos_);
      return false;
    }
  if (!r_->getField()->intoField(to))
    {
      LOG2("Trying to move player outside of field: %1", to);
      return false;
    }

  ActMove pkt;
  const PosList& p = f->getPath(pos_, to, this);
  if (p.empty())
    {
      LOG2("Can not go from %1 to %2. Sorry.", pos_, to);
      return false;
    }
  pkt.player_id = id_;
  pkt.action = action;
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
  return true;
}

bool CPlayer::block(CPlayer* opponent, enum eAction action)
{
  if (opponent == NULL
      || opponent->getTeamId() == getTeamId()
      || !getPosition().isNear(opponent->getPosition())
      || opponent->getStatus() != STA_STANDING)
    return false;
  ActBlock pkt;
  pkt.player_id = id_;
  pkt.action = action;
  pkt.opponent_id = opponent->getId();
  r_->sendPacket(pkt);
  return true;
}

bool CPlayer::pass(const Position& to)
{
  ActPass pkt;
  pkt.player_id = id_;
  pkt.action = PASS;
  pkt.dest_row = to.row;
  pkt.dest_col = to.col;
  r_->sendPacket(pkt);
  return true;
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

void CPlayer::msgPlayerPos(const MsgPlayerPos* m)
{
  Position pos(m->row, m->col);
  setPosition(pos);
  r_->onEvent(m);
  LOG2("set pos for %1 id %2", team_id_, id_);
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
	  && status_ != STA_PRONE
	  && status_ != STA_STUNNED)
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

void CPlayer::msgPlayerKO(const MsgPlayerKO* m)
{
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

bool CPlayer::filterPlayerKO(const MsgPlayerKO* m)
{
  if (m->client_id != team_id_ || m->player_id != id_)
    return false;
  return true;
}
