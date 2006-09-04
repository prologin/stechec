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

#include "Constants.hh"
#include "Event.hh"
#include "Api.hh"
#include "CRules.hh"

CRules::CRules(const xml::XMLConfig& cfg)
  : cfg_(cfg),
    cur_turn_(0),
    cur_half_(0)
{
  // Register tokens that we must handle ourself.
  HANDLE_WITH(MSG_INITGAME, CRules, this, msgInitGame, GS_WAIT);
  HANDLE_WITH(MSG_INITHALF, CRules, this, msgInitHalf, GS_ALL);
  HANDLE_WITH(MSG_NEWTURN, CRules, this, msgPlayTurn, GS_ALL);
  HANDLE_WITH(MSG_ENDGAME, CRules, this, msgEndGame, GS_ALL);
  HANDLE_WITH(MSG_TIMEEXCEEDED, CRules, this, msgTimeExceeded, GS_ALL);
  HANDLE_WITH(MSG_ILLEGAL, CRules, this, msgIllegal, GS_ALL);
  HANDLE_WITH(MSG_CHAT, CRules, this, msgChatMessage, GS_ALL);
  HANDLE_WITH(ACT_MOVETURNMARKER, CRules, this, msgMoveTurnMarker, GS_ALL);
  HANDLE_WITH(CUSTOM_EVENT, CRules, this, msgCustomEvent, GS_ALL);
  
  api_ = new Api(this);
}

CRules::~CRules()
{
  delete api_;
}

void        CRules::serialize(std::ostream& os) const
{
  os << "CRules" << std::endl;
  weather_->serialize(os);
}

void        CRules::unserialize(std::istream& is)
{
  weather_->unserialize(is);
}

Api*        CRules::getApi()
{
  return api_;
}


/*
** Message management.
*/

void        CRules::msgIllegal(const MsgIllegal* m)
{
  onEvent(m);
}

void        CRules::msgInitGame(const MsgInitGame* m)
{
  // We _must_ have our uid at this time.
  assert(getTeamId() >= 0);

  // Change our game state.
  setState(GS_INITGAME);
  LOG2("-- CRules: change state: GS_INITGAME");

  // Create some objects.
  weather_ = new Weather;
  ball_ = new CBall(this);
  field_ = new CField;
  HANDLE_WITH(MSG_WEATHER, Weather, weather_, setWeather, GS_INITGAME);
  HANDLE_WITH(MSG_BALLPOS, CBall, ball_, setPosition, GS_ALL);

  // Create, populate our team from the xml file, and send it.
  our_team_ = new CTeam(getTeamId(), this);
  our_team_->loadConfig(cfg_.getData<std::string>("team"));

  // Other team.
  other_team_ = new CTeam(getTeamId() == 0 ? 1 : 0, this);
  
  // Done. Inform the server that we are ok
  // by rethrowing the packet.
  sendPacket(*m);
}

void        CRules::msgInitHalf(const MsgInitHalf* m)
{
  setState(GS_INITHALF);
  LOG2("-- CRules: change state: GS_INITHALF");

  // Now, you can safely use Api fonctions referring to teams.
  api_->select_team(US);

  if (m->client_id != getTeamId())
    {
      // Place our team on the field.
      our_team_->placeTeam(1);
      // Ask the UI to place the ball.
      onEvent(eKickOff);
    }
  else
    {
      // Otherwise, place our team and say we have finished.
      // FIXME: wait that the first team has set up,
      // then place our team, then kick-off...
      our_team_->placeTeam(1);
      sendPacket(*m);
    }
}

void        CRules::msgPlayTurn(const MsgNewTurn* m)
{
  // Who will play.
  setState(m->client_id == 0 ? GS_COACH1 : GS_COACH2);
  if (m->client_id == getTeamId())
    {
      LOG2("-- CRules: change state: GS_COACH (Our turn)");
      our_team_->resetTurn();
      onEvent(eOurTurn);
    }
  else
    {
      LOG2("-- CRules: change state: GS_COACH (Their turn)");
      other_team_->resetTurn();
      onEvent(eTheirTurn);
    }
}

void        CRules::msgEndGame(const MsgEndGame* m)
{
  setState(GS_END);
  LOG2("-- CRules: change state: GS_END");
  onEvent(m);
}

void        CRules::msgTimeExceeded(const MsgTimeExceeded* m)
{
  onEvent(m);
}

void        CRules::msgChatMessage(const MsgChat* m)
{
  onEvent(m);
}

void        CRules::msgMoveTurnMarker(const ActMoveTurnMarker* m)
{
  onEvent(m);
}

void        CRules::msgCustomEvent(const CustomEvent* m)
{
  onEvent(m);
}

// If client_rules is loaded as a dynamic library, this is the
// method to call to get an instance of bb5 CRules.
extern "C" BaseCRules* load_client_rules(xml::XMLConfig* cfg)
{
  return new CRules(*cfg);
}
