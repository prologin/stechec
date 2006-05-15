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

CPlayer::CPlayer(int id, int team_id, CRules* r)
  : Player(id, team_id),
    r_(r)
{
}

CPlayer::~CPlayer()
{
}

// sig... wanted it to be on Player.
void CPlayer::setPosition(const Position& pos)
{
  CField* f = r_->getField();
  if (f->intoField(pos_))
    f->setPlayer(pos_, NULL);
  pos_ = pos;
  f->setPlayer(pos_, this);
}

void CPlayer::loadConfig(xml::XMLTeam& team)
{
  team.switchToPlayer(id_);

  MsgPlayerInfo pkt;
  pkt.player_id = id_;
  pkt.ma = team.getData<int>("ma");
  pkt.st = team.getData<int>("st");
  pkt.ag = team.getData<int>("ag");
  pkt.av = team.getData<int>("av");
  //init ma_remain_ to prevent random value
  ma_remain_ = 0;
  
  stringToPacket(pkt.name, team.getAttr<std::string>("player", "name"), 32);
  r_->sendPacket(pkt);
}

void CPlayer::msgPlayerInfo(const MsgPlayerInfo* m)
{
  ma_ = m->ma;
  st_ = m->st;
  ag_ = m->ag;
  av_ = m->av;
  name_ = packetToString(m->name);
}

bool CPlayer::move(const Position& to)
{
  if (to == pos_)
    {
      LOG2("You already are on " << pos_);
      return false;
    }
  if (!r_->getField()->intoField(to))
    {
      LOG2("Trying to move player outside of field: " << to);
      return false;
    }

  ActMove pkt;
  CField* f = r_->getField();
  const PosList& p = f->getPath(pos_, to, this);
  if (p.empty())
    {
      LOG2("Can't go from " << pos_ << " to " << to << ". Sorry.");
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

// lalala...
#include "Field.cc"
void fou()
{
  Field<CPlayer> f;
}
