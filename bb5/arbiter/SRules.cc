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

#include "SRules.hh"

#include "Constants.hh"
#include "Dice.hh"
#include "SActionHandler.hh"
#include "SBall.hh"
#include "SPlayer.hh"
#include "SPlayerMsg.hh"
#include "STeam.hh"
#include "STeamMsg.hh"
#include "SWeather.hh"
#include "xml/xml_config.hh"

// 8 turns per turn.
# define NB_TURNS 8
// 4 min. per turn.
# define TIME_PER_TURN (4 * 60)

SRules::SRules()
  : cur_turn_(0),
    cur_half_(0),
    turnover_(TOM_UNASSIGNED),
    weather_(NULL),
    ball_(NULL),
    field_(NULL)
{
  team_[0] = NULL;
  team_[1] = NULL;
  scoring_team_ = -1;
  timer_.setAllowedTime(TIME_PER_TURN);

  dice_ = new Dice(this);
  team_msg_ = new STeamMsg(this);
  player_msg_ = new SPlayerMsg(this);
  action_handler_ = new SActionHandler(this);

  // Register tokens that we must handle ourself.
  HANDLE_WITH(MSG_INITGAME, SRules, this, msgInitGame, GS_INITGAME);
  HANDLE_WITH(MSG_DRAWKICKER, SRules, this, msgDrawKicker, GS_DRAWKICKER);
  HANDLE_WITH(MSG_INITKICKOFF, SRules, this, msgInitKickoff, GS_INITKICKOFF);
  HANDLE_WITH(MSG_ENDTURN, SRules, this, msgPlayTurn, 0);
  HANDLE_WITH(MSG_CHAT, SRules, this, msgForwardChat, 0);
  HANDLE_WITH(MSG_MOVETURNMARKER, SRules, this, msgMoveTurnMarker, 0);
  HANDLE_WITH(MSG_ILLEGALPROC, SRules, this, msgIllegalProcedure, 0);
}

SRules::~SRules()
{
  delete team_[0];
  delete team_[1];
  delete weather_;
  delete ball_;
  delete field_;
  delete dice_;
  delete team_msg_;
  delete player_msg_;
}

void SRules::serialize(std::ostream& os) const
{
  os << "foo";
}

void SRules::unserialize(std::istream& is)
{
  int foo;
  is >> foo;
}

const char* SRules::tokenToString(unsigned token) const
{
  if (token >= sizeof (bb5_token_str) / sizeof (const char *))
    return "(overflow)";
  return bb5_token_str[token];
}

void SRules::serverStartup()
{
  team_[0] = new STeam(0, this, player_msg_);
  team_[1] = new STeam(1, this, player_msg_);
  team_msg_->setTeam(0, team_[0]);
  team_msg_->setTeam(1, team_[1]);

  //  weather_ = new SWeather();
  ball_ = new SBall(this);
  field_ = new SField();

  // Tell everybody that we are about to initialize the game.
  setState(GS_INITGAME);
  MsgInitGame pkt;
  pkt.time_per_turn = TIME_PER_TURN;
  sendPacket(pkt);
}

void SRules::serverProcess()
{
  if (timer_.isTimeElapsed())
    {
      timer_.stop();
      turnover(TOM_TIMEEXCEEDED);
    }
}

void SRules::waitForCurrentOpponentChoice(int team_id)
{
  if ((!timer_.isPaused()) && (team_id == getCurrentOpponentTeamId()))
    {
      LOG4("Suspends turn timer, waiting for coach `%1' reply.", team_id);
      timer_.pause();
      MsgTimer msg(team_id);
      msg.time_remaining = timer_.getTimeRemaining();
      msg.pause = true;
      sendPacket(msg);
    }
}

void SRules::checkForCurrentOpponentChoice(int team_id)
{
  if (timer_.isPaused() && (team_id == getCurrentOpponentTeamId()))
    {
      LOG4("Resumes turn timer, since coach `%1' replied.", team_id);
      timer_.start();
      MsgTimer msg(team_id);
      msg.time_remaining = timer_.getTimeRemaining();
      msg.pause = false;
      sendPacket(msg);
    }
}

bool SRules::checkGameEnd()
{
  // Game ends if scores differ after 2 or 3 halfs.
  if (cur_half_ > 2 && team_[0]->getScore() != team_[1]->getScore())
    {
      LOG4("Team %1 win the game.", (team_[0]->getScore() > team_[1]->getScore() ? 0 : 1));
      LOG4("End of game.");
      setState(GS_END);
      MsgEndGame pkt;
      sendPacket(pkt);
      return true;
    }
  return false;
}

void SRules::initHalf()
{
  cur_turn_ = 0;
  cur_half_++;

  if (checkGameEnd())
    return;

  // After 3 halfs, match is decided by a penalty shoot-out.
  if (cur_half_ > 3) 
    {
      int coach0 = dice_->roll("3th half coach0") + team_[0]->getRemainingReroll();
      int coach1 = dice_->roll("3th half coach1") + team_[1]->getRemainingReroll();
      while (coach0 == coach1)
        {
          coach0 = dice_->roll("3th half coach0") + team_[0]->getRemainingReroll();
          coach1 = dice_->roll("3th half coach1") + team_[1]->getRemainingReroll();
        }
      team_[(coach0 > coach1) ? 0 : 1]->incrementScore();
      MsgTouchdooown msgt((coach0 > coach1) ? 0 : 1);
      msgt.player_id = -1;
      sendPacket(msgt);
      checkGameEnd();
      return;
    }

  // Coaches recover their Rerolls, except in overtime
  if (cur_half_ != 3) 
    {
      team_[0]->initReroll();
      team_[1]->initReroll();
    }

  // Say we are in a new half. No reply is expected.
  MsgInitHalf pkt;
  pkt.cur_half = cur_half_;
  sendPacket(pkt);

  LOG4("Initialize half %1.", cur_half_);

  setState(GS_DRAWKICKER);

  // Switch the kicking team on second period.
  if (cur_half_ == 2)
    {
      coach_begin_ = (coach_begin_ + 1) % 2;

      msgDrawKicker(NULL);
    }

  // New toss for the first and third period
  if (cur_half_ == 1 || cur_half_ == 3)
    {
      coach_begin_ = dice_->roll("choosing team",  D2) % 2;

      MsgDrawKicker msgdk(coach_begin_);
      msgdk.kickoff = -1;
      sendPacket(msgdk);
    }
}

void SRules::initKickoff()
{

  setState(GS_INITKICKOFF);
  team_[0]->state_ = GS_INITKICKOFF;
  team_[1]->state_ = GS_INITKICKOFF;

  // The ref takes the ball
  ball_->removeFromField();

  // Each KO player tries to regain consciousness
  // and players on field go to the reserve
  team_[0]->prepareKickoff();
  team_[1]->prepareKickoff();

  // Say that we are about to initialize the kickoff.
  int kicking_id = getCurrentOpponentTeamId();
  LOG4("Kicking team: %1.", kicking_id);
  MsgInitKickoff pkt(kicking_id);
  pkt.place_team = 1;
  sendPacket(pkt);
}

void SRules::turnover(enum eTurnOverMotive motive)
{
  if ((turnover_ == TOM_UNASSIGNED) || (motive == TOM_TIMEEXCEEDED) || (motive == TOM_TOUCHDOOOWN))
    {
      if (turnover_ != TOM_TOUCHDOOOWN || motive != TOM_TIMEEXCEEDED)
        turnover_ = motive;
      MsgTurnOver pkt(getCurrentTeamId());
      pkt.motive = motive;
      sendPacket(pkt);
    }
  if (turnover_ != TOM_TIMEEXCEEDED || action_handler_->isEmpty())
    action_handler_->process();
}

enum eTurnOverMotive SRules::getTurnoverMotive()
{
  return turnover_;
}

void SRules::touchdooown(SPlayer* p)
{
  scoring_team_ = p->getTeamId();
  turnover(TOM_TOUCHDOOOWN);
}

void SRules::afterTouchdooown()
{
  // Let enough time to the scoring coach to cheer his team and place the ball.
  timer_.stop();

  // Check for a touchdooown scored during the turn of the opponent team.
  if (getState() == GS_COACH1 && scoring_team_ == 1
      || getState() == GS_COACH2 && scoring_team_ == 0)
    {
      if (getState() == GS_COACH1 && coach_begin_ == 1
          || getState() == GS_COACH2 && coach_begin_ == 0)
        cur_turn_++;
      // Switch playing team
      setState(getState() == GS_COACH1 ? GS_COACH2 : GS_COACH1);
      LOG4("Skip turn `%1' of team `%2', because its scored during opponent turn.",
          cur_turn_, scoring_team_);
    }

  // Check it is not the last turn of the half
  if (cur_turn_ == NB_TURNS
      && (getState() == GS_COACH1 && coach_begin_ == 1
        || getState() == GS_COACH2 && coach_begin_ == 0))
    {
      initHalf();
      return;
    }
  if (checkGameEnd())
    return;

  turnover_ = TOM_UNASSIGNED;
  coach_receiver_ = getState() == GS_COACH1 ? 1 : 0;
  initKickoff();
}

/*
** Handle messages received from the client
*/

// A coach has finished to set up his game.
void SRules::msgInitGame(const MsgInitGame* m)
{
  //FIXME: Do some checks: min. number of players, etc.
  team_[m->client_id]->state_ = GS_DRAWKICKER;

  if (team_[0]->state_ == GS_DRAWKICKER && team_[1]->state_ == GS_DRAWKICKER)
    {
      // Send some objects to clients -> weather wil be introduce with advanced Rules
      /*
      MsgWeather pkt;
      pkt.weather = weather_->getWeather();
      sendPacket(pkt);
      */
      
      initHalf();
    }
}

void SRules::msgDrawKicker(const MsgDrawKicker* m)
{
  if (m != NULL)
    {
      if (m->client_id != coach_begin_)
        {
          LOG3("Coach %1 doesn't have the choice.", m->client_id);
          sendIllegal(m->token, m->client_id, ERR_WRONGCONTEXT);
          return;
        }
      coach_begin_ = (m->kickoff == 1) ? (coach_begin_ + 1) % 2 : coach_begin_;
    }

  LOG4("Coach %1 plays first (receiving team).", coach_begin_);

  MsgDrawKicker new_msg((coach_begin_ + 1) % 2);
  new_msg.kickoff = 1;
  sendPacket(new_msg);

  coach_receiver_ = coach_begin_;

  initKickoff();
}

// Coach has finished to place his team, before a kick-off.
void SRules::msgInitKickoff(const MsgInitKickoff* m)
{
  int receiving_id = getCurrentTeamId();
  int kicking_id = (receiving_id + 1) % 2;
    
  // Verify that teams set up in the right order.
  if (m->client_id == receiving_id &&
      (team_[kicking_id]->state_ == GS_INITKICKOFF ||
       team_[receiving_id]->state_ != GS_INITKICKOFF))
    {
      // Receiving sets up before kicking or is already set.
      sendIllegal(MSG_INITKICKOFF, m->client_id, ERR_WRONGCONTEXT);
      return;
    }
  if (m->client_id == kicking_id &&
      (team_[receiving_id]->state_ != GS_INITKICKOFF ||
       team_[kicking_id]->state_ != GS_INITKICKOFF))
    {
      // Kicking sets up after receiving or is already set.
      sendIllegal(MSG_INITKICKOFF, m->client_id, ERR_WRONGCONTEXT);
      return;
    }

  // The team is placed on the field, check the placement
  if (!team_[m->client_id]->isPlacementValid() ||
      !field_->isPlacementValid(m->client_id))
    {
      sendIllegal(MSG_INITKICKOFF, m->client_id);
      return;
    }

  team_[m->client_id]->state_ = GS_KICKOFF;

  if (m->client_id == kicking_id)
    {
      // The receiving team has to be placed
      MsgInitKickoff pkt(getCurrentTeamId());
      pkt.place_team = 1;
      sendPacket(pkt);
    }
  else
    {
      // The two teams are ready, message to kick the ball
      setState(GS_KICKOFF);

      MsgInitKickoff pkt(getCurrentOpponentTeamId());
      pkt.place_team = 0;
      sendPacket(pkt);
    }
}

void SRules::finishKickoff()
{
  if (coach_receiver_ == 0)
    setState(GS_COACH2);
  else
    setState(GS_COACH1);

  nextTurn();
}

void SRules::msgPlayTurn(const MsgEndTurn* m)
{
  // Checks if it's the right team that asked for an EndTurn.
  if ((getState() != GS_COACH1 && m->client_id == 0)
      || (getState() != GS_COACH2 && m->client_id == 1))
    {
      LOG3("Coach `%1' is not allowed to end the turn of team `%2'.",
          m->client_id, getCurrentTeamId());
      sendIllegal(m->token, m->client_id, ERR_WRONGCONTEXT);
    }
  else if (!action_handler_->isEmpty())
    {
      LOG3("Coach `%1' can not end his turn, since there are actions pending.", m->client_id);
      sendIllegal(m->token, m->client_id);
    }
  else
    {
      nextTurn();
    }
}

void SRules::nextTurn()
{
  // Next turn if this is the receiving team.
  if (getState() == GS_COACH1 && coach_begin_ == 1
      || getState() == GS_COACH2 && coach_begin_ == 0)
    {
      cur_turn_++;
      LOG4("=== Go on turn `%1`", cur_turn_);
    }
  else
    LOG4("=== Next team plays turn `%1`", cur_turn_);

  // Finished ? Go on the next half ? 
  if (cur_turn_ > NB_TURNS)
    {
      initHalf();
      return;
    }

  // Switch playing team
  setState(getState() == GS_COACH1 ? GS_COACH2 : GS_COACH1);

  turnover_ = TOM_UNASSIGNED;
  ball_->resetTurn();
  if (getState() == GS_COACH1)
    {
      team_[1]->setProneStunned();
      team_[0]->resetTurn();    
    }
  else
    {
      team_[0]->setProneStunned();
      team_[1]->resetTurn();
    }

  // Send the playing team_id to clients.
  MsgNewTurn pkt;
  pkt.client_id = getState() == GS_COACH1 ? 0 : 1;
  pkt.cur_half = cur_half_;
  pkt.cur_turn = cur_turn_;
  sendPacket(pkt);
  timer_.restart();
}

void SRules::msgForwardChat(const MsgChat* m)
{
  sendPacket(*m);
}

void SRules::msgMoveTurnMarker(const MsgMoveTurnMarker* m)
{
  // If its not its turn, warn.
  if (getState() == GS_COACH1 && m->client_id == 1
      || getState() == GS_COACH2 && m->client_id == 0)
    {
      sendIllegal(MSG_MOVETURNMARKER, m->client_id, ERR_WRONGCONTEXT);
      return;
    }

  // Ok. Acknowledge.
  //team_[m->client_id]->setTurnMarker();
  sendPacket(*m);
}

void SRules::msgIllegalProcedure(const MsgIllegalProc*)
{
  // FIXME: todo
}


// If arbiter is loaded as a dynamic library, this is the
// method to call to get an instance of bb5 SRules.
extern "C" BaseSRules* load_server_rules(xml::XMLConfig*)
{
  return new SRules;
}
