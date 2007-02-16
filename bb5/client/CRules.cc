/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2006, 2007 The TBT Team.
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
#include "xml/xml_config.hh"


CRules::CRules(const xml::XMLConfig& cfg)
  : cfg_(cfg),
    cur_turn_(0),
    cur_half_(0)
{
  // Register tokens that we must handle ourself.
  HANDLE_WITH(MSG_INITGAME, CRules, this, msgInitGame, GS_WAIT);
  HANDLE_WITH(MSG_INITHALF, CRules, this, msgInitHalf, GS_ALL);
  HANDLE_WITH(MSG_INITKICKOFF, CRules, this, msgInitKickoff, GS_ALL);
  HANDLE_WITH(MSG_GIVEBALL, CRules, this, msgGiveBall, GS_INITKICKOFF);
  HANDLE_WITH(MSG_NEWTURN, CRules, this, msgPlayTurn, GS_ALL);
  HANDLE_WITH(MSG_ENDGAME, CRules, this, msgEndGame, GS_ALL);
  HANDLE_WITH(MSG_TURNOVER, CRules, this, msgTurnOver, GS_ALL);
  HANDLE_WITH(MSG_ILLEGAL, CRules, this, msgIllegal, GS_ALL);
  HANDLE_WITH(MSG_CHAT, CRules, this, msgChatMessage, GS_ALL);
  HANDLE_WITH(MSG_MOVETURNMARKER, CRules, this, msgMoveTurnMarker, GS_ALL);
  HANDLE_WITH(CUSTOM_EVENT, CRules, this, msgCustomEvent, GS_ALL);
  HANDLE_WITH(MSG_RESULT, CRules, this, msgResult, GS_ALL);
  HANDLE_WITH(MSG_BLOCKRESULT, CRules, this, msgBlockResult, GS_COACHBOTH | GS_REROLL | GS_BLOCK);
  HANDLE_WITH(MSG_BLOCKDICE, CRules, this, msgBlockDice, GS_BLOCK);
  HANDLE_WITH(MSG_BLOCKPUSH, CRules, this, msgBlockPush, GS_PUSH | GS_COACHBOTH | GS_BLOCK);
  HANDLE_WITH(MSG_FOLLOW, CRules, this, msgFollow, GS_PUSH | GS_COACHBOTH | GS_BLOCK | GS_FOLLOW);
  
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

Api*    CRules::getApi()
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
  // weather_ = new Weather;
  ball_ = new CBall(this);
  field_ = new CField;
  HANDLE_WITH(MSG_WEATHER, Weather, weather_, setWeather, GS_INITGAME);
  HANDLE_WITH(MSG_BALLPOS, CBall, ball_, setPosition, GS_ALL);

  // Create, populate our team from the xml file, and send it.
  our_team_ = new CTeam(getTeamId(), this);
  our_team_->loadConfig(cfg_.getData<std::string>("client", "team"));

  // Other team.
  other_team_ = new CTeam(getTeamId() == 0 ? 1 : 0, this);

  // Kludge. Allow UI to do something while in GS_INITGAME.
  onEvent(eInitGame);
  
  // Done. Inform the server that we are ok
  // by rethrowing the packet.
  sendPacket(*m);
}

void        CRules::msgInitHalf(const MsgInitHalf* m)
{
  cur_turn_ = 0;
  cur_half_ = m->cur_half;
  LOG2("-- CRules: Initialize the half %1", cur_half_);

  our_team_->initReroll();
  other_team_->initReroll();

  onEvent(m);
}

void        CRules::msgInitKickoff(const MsgInitKickoff* m)
{
  // Exit if is not for our team.
  if (m->client_id != getTeamId())
    {
      onEvent(m);
      return;
    }

  if (m->place_team)
    {
      assert(!(getState() & GS_INITKICKOFF));
      setState(GS_INITKICKOFF);
      api_->selectTeam(US);

      // Our team has to enter the field
      // FIXME: do it automatically, for now.
      our_team_->placeTeam(1);
      sendPacket(*m);
      return;
    }
  else
    {
      assert(getState() & GS_INITKICKOFF);
      // Inform UI that he has to place the ball
      onEvent(m);
    }
}

void        CRules::msgGiveBall(const MsgGiveBall* m)
{
  setState(GS_TOUCHBACK);
  onEvent(m);
}

void        CRules::msgPlayTurn(const MsgNewTurn* m)
{
  cur_turn_ = m->cur_turn;

  // Who will play.
  setState(m->client_id == 0 ? GS_COACH1 : GS_COACH2);
  if (m->client_id == getTeamId())
    {
      LOG2("-- CRules: change state: GS_COACH (Our turn, turn %1, half %2)",
          m->cur_turn, m->cur_half);
      our_team_->resetTurn();
    }
  else
    {
      LOG2("-- CRules: change state: GS_COACH (Their turn, turn %1, half %2)",
          m->cur_turn, m->cur_half);
      other_team_->resetTurn();
    }
  onEvent(m);
}

void        CRules::msgEndGame(const MsgEndGame* m)
{
  setState(GS_END);
  LOG2("-- CRules: change state: GS_END");
  onEvent(m);
}

void        CRules::msgTurnOver(const MsgTurnOver* m)
{
  onEvent(m);
}

void        CRules::msgChatMessage(const MsgChat* m)
{
  onEvent(m);
}

void        CRules::msgMoveTurnMarker(const MsgMoveTurnMarker* m)
{
  onEvent(m);
}

void        CRules::msgCustomEvent(const CustomEvent* m)
{
  onEvent(m);
}


void CRules::msgResult(const MsgResult* m)
{
  if (m->client_id == getTeamId() && m->reroll)
    {
      setState(GS_REROLL);
      LOG2("-- CRules: change state: GS_REROLL");
    }
  onEvent(m);
}


void CRules::msgBlockResult(const MsgBlockResult* m)
{
  if (m->client_id == getTeamId())
    {
      if (! m->reroll)
        our_team_->getPlayer(m->player_id)->subMa(1);
      if (m->strongest_team_id == getTeamId())
        setState(GS_BLOCK);
      else if (m->reroll)
        setState(GS_REROLL);
    }
  else
    {
      if (! m->reroll)
        other_team_->getPlayer(m->player_id)->subMa(1);
      if (m->strongest_team_id == getTeamId() && !m->reroll)
        setState(GS_BLOCK);
    }
  onEvent(m);
}

void CRules::msgBlockDice(const MsgBlockDice* m)
{
  setState(m->client_id == 0 ? GS_COACH1 : GS_COACH2);
}

void CRules::msgBlockPush(const MsgBlockPush* m)
{
  if (getState() == GS_PUSH)
    {
      setState(m->client_id == 0 ? GS_COACH1 : GS_COACH2);
    }
  else if (m->client_id == getTeamId())
    {
      setState(GS_PUSH);
      onEvent(m);
    }
}

void CRules::msgFollow(const MsgFollow* m)
{
  if (getState() == GS_FOLLOW)
    {
      setState(m->client_id == 0 ? GS_COACH1 : GS_COACH2);
    }
  else if (m->client_id == getTeamId())
    {
      setState(GS_FOLLOW);
      onEvent(m);
    }
}

// If client_rules is loaded as a dynamic library, this is the
// method to call to get an instance of bb5 CRules.
extern "C" BaseCRules* load_client_rules(xml::XMLConfig* cfg)
{
  return new CRules(*cfg);
}
