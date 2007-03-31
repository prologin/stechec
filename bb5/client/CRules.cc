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
#include "xml/xml_config.hh"
#include "Event.hh"
#include "Api.hh"
#include "CTeamMsg.hh"
#include "CPlayerMsg.hh"
#include "CRules.hh"


CRules::CRules(const xml::XMLConfig& cfg)
  : cfg_(cfg),
    cur_turn_(0),
    cur_half_(0)
{
  // Register tokens that we must handle ourself.
  HANDLE_WITH(MSG_INITGAME, CRules, this, msgInitGame, GS_WAIT);
  HANDLE_WITH(MSG_INITHALF, CRules, this, msgInitHalf, 0);
  HANDLE_WITH(MSG_DRAWKICKER, CRules, this, msgDrawKicker, 0);
  HANDLE_WITH(MSG_INITKICKOFF, CRules, this, msgInitKickoff, 0);
  HANDLE_WITH(MSG_GIVEBALL, CRules, this, msgGiveBall, 0);
  HANDLE_WITH(MSG_NEWTURN, CRules, this, msgPlayTurn, 0);
  HANDLE_WITH(MSG_ENDGAME, CRules, this, msgEndGame, 0);
  HANDLE_WITH(MSG_TURNOVER, CRules, this, msgTurnOver, 0);
  HANDLE_WITH(MSG_ILLEGAL, CRules, this, msgIllegal, 0);
  HANDLE_WITH(MSG_CHAT, CRules, this, msgChatMessage, 0);
  HANDLE_WITH(MSG_MOVETURNMARKER, CRules, this, msgMoveTurnMarker, 0);
  HANDLE_WITH(CUSTOM_EVENT, CRules, this, msgCustomEvent, 0);
  HANDLE_WITH(MSG_RESULT, CRules, this, msgResult, 0);
  HANDLE_WITH(MSG_BLOCKRESULT, CRules, this, msgBlockResult, 0);
  HANDLE_WITH(MSG_BLOCKDICE, CRules, this, msgBlockDice, 0);
  HANDLE_WITH(MSG_BLOCKPUSH, CRules, this, msgBlockPush, 0);
  HANDLE_WITH(MSG_FOLLOW, CRules, this, msgFollow, 0);

  api_ = new Api(this);
  team_msg_ = new CTeamMsg(this);
  player_msg_ = new CPlayerMsg(this);
}

CRules::~CRules()
{
  delete api_;
  delete team_msg_;
  delete player_msg_;
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

const char* CRules::tokenToString(int token) const
{
  token -= RULES_TOKEN_START;
  if (token >= sizeof (bb5_token_str) / sizeof (const char *))
    return "(overflow)";
  return bb5_token_str[token];
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
  HANDLE_WITH(MSG_BALLPOS, CBall, ball_, setPosition, 0);

  // Create, populate our team from the xml file, and send it.
  our_team_ = new CTeam(getTeamId(), this, player_msg_);
  team_msg_->setTeam(our_team_->getTeamId(), our_team_);
  our_team_->loadConfig(cfg_.getData<std::string>("client", "team"));

  // Other team.
  other_team_ = new CTeam(getTeamId() == 0 ? 1 : 0, this, player_msg_);
  team_msg_->setTeam(other_team_->getTeamId(), other_team_);

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

void        CRules::msgDrawKicker(const MsgDrawKicker* m)
{
  assert(getState() != GS_DRAWKICKER);

  if (m->client_id == getTeamId())
    {
      setState(GS_DRAWKICKER);
      LOG2("-- CRules: change state: GS_DRAWKICKER");
    }

  onEvent(m);
}

void        CRules::msgInitKickoff(const MsgInitKickoff* m)
{
  if (m->client_id != getTeamId())
    {
      // Inform UI that other team is concerned.
      onEvent(m);
    }
  else if (m->place_team)
    {
      assert(getState() != GS_INITKICKOFF);
      setState(GS_INITKICKOFF);
      api_->selectTeam(US);

      // Our team has to enter the field
      // FIXME: Automatically use default placement, for now.
      our_team_->placeTeam(1);
      // Inform UI that he has to place players.
      onEvent(m);
    }
  else
    {
      assert(getState() == GS_INITKICKOFF);
      setState(GS_KICKOFF);
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
