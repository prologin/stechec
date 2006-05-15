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
#include "CTeam.hh"

CTeam::CTeam(int team_id, CRules* r)
  : Team<CPlayer>(team_id),
    r_(r)
{
  r_->HANDLE_WITH(MSG_TEAMINFO, CTeam, this, msgTeamInfo, GS_INITGAME);
  r_->HANDLE_WITH(MSG_PLAYERINFO, CTeam, this, msgPlayerInfo, GS_INITGAME);
  r_->HANDLE_WITH(MSG_PLAYERPOS, CTeam, this, msgPlayerPos, GS_INITHALF);
  r_->HANDLE_WITH(ACT_MOVE, CTeam, this, msgPlayerMove, GS_COACHBOTH);
  r_->HANDLE_WITH(MSG_PLAYERKNOCKED, CTeam, this, msgPlayerKnocked, GS_COACHBOTH);
}

// Parse our xml team file, send this to server, and wait that
// it return it to us to fill our data structure.
void CTeam::loadConfig(const std::string& team_file)
{
  xml_team_.parse(team_file);
  xml_team_.switchToTeamGlobals();

  MsgTeamInfo pkt;
  pkt.reroll = xml_team_.getData<int>("reroll");
  stringToPacket(pkt.team_name, xml_team_.getAttr<std::string>("team", "name"), 32);
  stringToPacket(pkt.coach_name, xml_team_.getAttr<std::string>("team", "coach"), 32);
  r_->sendPacket(pkt);
  
  xml_formation_.parse(xml_team_.getData<std::string>("formation"));

  for (int i = 0; i < 16; i++)
    {
      if (!xml_team_.switchToPlayer(i + 1))
        break;
      CPlayer p(i + 1, team_id_, r_);
      p.loadConfig(xml_team_);
      player_.push_back(p);
    }
}

// Send the content of formation.xml to the server. On half inits.
void CTeam::placeTeam(int formation_id)
{
  xml_formation_.switchFormationId(formation_id);
  PlayerIter it;
  for (it = player_.begin(); it != player_.end(); ++it)
    {
      MsgPlayerPos pkt;
      Position pos = xml_formation_.getPos(it->getId());
      pkt.player_id = it->getId();
      pkt.row = pos.row;
      pkt.col = pos.col;
      // if this is team 2, mirror on rows [|13-25|].
      if (team_id_ == 2)
        pkt.row = ROWS - pkt.row - 1;
      r_->sendPacket(pkt);
    }
}

bool CTeam::movePlayer(int player_id, const Position& to)
{
  CPlayer* p = getPlayer(player_id);
  if (p == NULL)
    return false;
  return p->move(to);
}




void CTeam::msgTeamInfo(const MsgTeamInfo* m)
{
  if (m->client_id != team_id_)
    return;

  team_name_ = packetToString(m->team_name);
  coach_name_ = packetToString(m->coach_name);
  reroll_ = m->reroll;
}

void CTeam::msgPlayerInfo(const MsgPlayerInfo* m)
{
  if (m->client_id != team_id_)
    return;

  // other_team_: players are not created yet.
  if (r_->getTeamId() != team_id_)
    player_.push_back(CPlayer(m->player_id, team_id_, r_));

  player_[m->player_id - 1].msgPlayerInfo(m);
}

void CTeam::msgPlayerPos(const MsgPlayerPos* m)
{
  if (m->client_id != team_id_)
    return;

  Position pos(m->row, m->col);
  player_[m->player_id - 1].setPosition(pos);
}

void CTeam::msgPlayerMove(const ActMove* m)
{
  if (m->client_id != team_id_)
    return;

  Position pos;
  pos.row = m->moves[m->nb_move - 1].row;
  pos.col = m->moves[m->nb_move - 1].col;
  player_[m->player_id - 1].setPosition(pos);
  r_->onEvent(m);
}

void CTeam::msgPlayerKnocked(const MsgPlayerKnocked* m)
{
  if (m->client_id != team_id_)
    return;

  r_->onEvent(m);
}
