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
  r_->HANDLE_F_WITH(MSG_TEAMINFO, CTeam, this, msgTeamInfo, filterTeamInfo, GS_INITGAME);
  r_->HANDLE_F_WITH(MSG_PLAYERINFO, CTeam, this, msgPlayerInfo, filterPlayerInfo, GS_INITGAME);
}

CTeam::~CTeam()
{
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
  for (int i = 0; i < MAX_PLAYER; i++)
    {
      if (!xml_team_.switchToPlayer(i + 1))
        break;
      loadPlayerConfig(xml_team_, i);
    }
}

// Load player info from xml.
// A player will be created when message will be returned by server.
void CTeam::loadPlayerConfig(xml::XMLTeam& xml_team, int player_id)
{
  xml_team.switchToPlayer(player_id + 1);

  MsgPlayerInfo pkt;
  pkt.player_id = player_id;
  stringToPacket(pkt.name, xml_team.getAttr<std::string>("player", "name"), 32);
  pkt.ma = xml_team.getData<int>("ma");
  pkt.st = xml_team.getData<int>("st");
  pkt.ag = xml_team.getData<int>("ag");
  pkt.av = xml_team.getData<int>("av");
  pkt.player_position = xml_team.getData<int>("positionid");
  stringToPacket(pkt.player_img, xml_team.getAttr<std::string>("player", "display"), 32);
  
  r_->sendPacket(pkt);
}


// Send the content of formation.xml to the server. On half inits.
void CTeam::placeTeam(int formation_id)
{
  xml_formation_.switchFormationId(formation_id);
  for (int i = 0; i < MAX_PLAYER; i++)
    if (player_[i] != NULL)
      {
	MsgPlayerPos pkt;
	Position pos = xml_formation_.getPos(player_[i]->getId() + 1);
	pkt.player_id = player_[i]->getId();
	pkt.row = pos.row;
	pkt.col = pos.col;
	// if this is team 2, mirror on rows [|13-25|].
	if (team_id_ == 1)
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

bool CTeam::standUpPlayer(int player_id)
{
  CPlayer* p = getPlayer(player_id);
  if (p == NULL)
    return false;
  return p->standUp();
}

bool CTeam::blockPlayer(int player_id, const Position& to)
{
  CPlayer* p = getPlayer(player_id);
  if (p == NULL)
    return false;
  return p->block(to);
}

void CTeam::msgTeamInfo(const MsgTeamInfo* m)
{
  team_name_ = packetToString(m->team_name);
  coach_name_ = packetToString(m->coach_name);
  reroll_ = m->reroll;
}

bool CTeam::filterTeamInfo(const MsgTeamInfo* m)
{
  if (m->client_id != team_id_)
    return false;
  return true;
}

void CTeam::msgPlayerInfo(const MsgPlayerInfo* m)
{
  player_[m->player_id] = new CPlayer(r_, m);
}

bool CTeam::filterPlayerInfo(const MsgPlayerInfo* m)
{
  if (m->client_id != team_id_)
    return false;
  return true;
}
