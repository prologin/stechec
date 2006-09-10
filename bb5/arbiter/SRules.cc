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
#include "Dice.hh"
#include "SRules.hh"

SRules::SRules()
  : cur_turn_(0),
    cur_half_(0),
    weather_(NULL),
    ball_(NULL),
    field_(NULL)
{
  team_[0] = NULL;
  team_[1] = NULL;
  timer_.setAllowedTime(60 * 4); // 4min per turn.

  dice_ = new Dice(this);
  
  // Register tokens that we must handle ourself.
  HANDLE_WITH(MSG_INITGAME, SRules, this, msgInitGame, GS_INITGAME);
  HANDLE_WITH(MSG_INITKICKOFF, SRules, this, msgInitKickoff, GS_INITKICKOFF);
  HANDLE_WITH(MSG_ENDTURN, SRules, this, msgPlayTurn, GS_COACHBOTH);
  HANDLE_WITH(MSG_CHAT, SRules, this, msgForwardChat, GS_ALL);
  HANDLE_WITH(ACT_MOVETURNMARKER, SRules, this, msgMoveTurnMarker, GS_COACHBOTH);
  HANDLE_WITH(ACT_ILLEGALPROC, SRules, this, msgIllegalProcedure, GS_COACHBOTH);
}

SRules::~SRules()
{
  delete team_[0];
  delete team_[1];
  delete weather_;
  delete ball_;
  delete field_;
  delete dice_;
}

void        SRules::serialize(std::ostream& os) const
{
  os << "foo";
}

void        SRules::unserialize(std::istream& is)
{
  int foo;
  is >> foo;
}

void SRules::serverStartup()
{
  initGame();
}

void SRules::serverProcess()
{
  if (timer_.isTimeElapsed())
    {
      sendPacket(MsgTimeExceeded());
      // go on next turn...
      msgPlayTurn(NULL);
    }
}

void SRules::initGame()
{
  // Tell everybody that we are about to initialize the game.
  setState(GS_INITGAME);
  sendPacket(MsgInitGame());

  team_[0] = new STeam(0, this);
  team_[1] = new STeam(1, this);

  //  weather_ = new SWeather();
  ball_ = new SBall(this);
  field_ = new SField();

  // Send some objects to clients -> weather wil be introduce with advanced Rules
  /*  MsgWeather pkt;
      pkt.weather = weather_->getWeather();
      sendPacket(pkt);*/

  // Decide who is the kicking team
  coach_begin_ = dice_->roll("kicking team", D2) - 1; // team_id: base 0.
  LOG3("Coach " << coach_begin_ << " plays first (receiving team).");
}

void SRules::initHalf()
{
  cur_turn_ = 0;
  cur_half_++;
  
  // If there is a winner after 2 or 3 halfs
  if (cur_half_ > 2 && team_[0]->getScore() != team_[1]->getScore())
    {
      LOG4("Team " << (team_[0]->getScore() > team_[1]->getScore() ? 0 : 1)
	   << " win the game.");
      LOG4("End of game.");
      setState(GS_END);
      MsgEndGame pkt;
      sendPacket(pkt);
      return;
    }
		
  // Switch the kicking team on second period.
  if (cur_half_ == 2)
    coach_begin_ = (coach_begin_ + 1) % 2;
		
  // New toss for the third period
  if (cur_half_ == 3)
    coach_begin_ = dice_->roll("kicking team",  D2) - 1;
	
  // Match is decided by a penalty shoot-out
  if (cur_half_ > 3) 
    {
      int coach0 = dice_->roll("3th half coach0") + team_[0]->getRerollsRemain();
      int coach1 = dice_->roll("3th half coach1") + team_[1]->getRerollsRemain();
      while (coach0 == coach1)
	{
	  coach0 = dice_->roll("3th half coach0") + team_[0]->getRerollsRemain();
	  coach1 = dice_->roll("3th half coach1") + team_[1]->getRerollsRemain();
	}
      LOG4("Team " << (team_[0]->getScore() > team_[1]->getScore() ? 0 : 1)
	   << " win the game.");
 
      LOG4("End of game.");
      setState(GS_END);
      MsgEndGame pkt;
      sendPacket(pkt);
      return;
    }
  	
  // Coaches recover their Rerolls, except in overtime
  if (cur_half_ != 3) 
    {
      team_[0]->initRerolls();
      team_[1]->initRerolls();
    }
	
  // Say we are in a new half	
  MsgInitHalf pkt;
  pkt.cur_half = cur_half_;
  sendPacket(pkt);
	
  LOG3("Initialize half " << cur_half_ << ".");

  coach_receiver_ = coach_begin_;
  initKickoff();
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


  LOG3("Kicking team: " << (getCurrentTeamId() + 1) % 2);

  // Say that we are about to initialize the kickoff.
  MsgInitKickoff pkt((getCurrentTeamId() + 1) % 2);
  sendPacket(pkt);
}

void SRules::turnOver()
{	
  msgPlayTurn(NULL);
}

void SRules::touchdown()
{
  LOG3("TOUCHDOWN!!!!!!!");
  // For the moment, only the case of touchdowns score during the good turn
  // TODO : update scores...
	
	
  // If it is the third period, the match is over
  initHalf();
	
  // Check it is not the last turn of the half
  if (cur_turn_ == 8
      &&(getState() == GS_COACH1 && coach_begin_ == 1
	 || getState() == GS_COACH2 && coach_begin_ == 0))
    {
      initHalf();
      return;
    }
		
  coach_receiver_ = getState() == GS_COACH1 ? 1 : 0;
  initKickoff();
}

/*
** Handle messages received from the client
*/

// A coach has finished to set up his game.
void SRules::msgInitGame(const MsgInitGame* m)
{
  team_[m->client_id]->state_ = GS_INITKICKOFF;

  if (team_[0]->state_ == GS_INITKICKOFF &&
      team_[1]->state_ == GS_INITKICKOFF)
    {
      initHalf();
    }
}

void SRules::msgInitKickoff(const MsgInitKickoff* m)
{
  // The team is placed on the field
  // Check the placement
  if (!team_[m->client_id]->isPlacementValid()
      ||!field_->isPlacementValid(m->client_id))
    {
      sendIllegal(MSG_INITKICKOFF, m->client_id);
      return;
    }

  team_[m->client_id]->state_ = GS_COACH1;


  if (team_[0]->state_ == GS_COACH1 &&
      team_[1]->state_ == GS_COACH1)
    {
      // The two teams are ready, message to kick the ball
      MsgInitKickoff pkt((getCurrentTeamId() + 1) % 2);
      sendPacket(pkt);
    }
  else
    {
      // The receiving team has to be placed
      MsgInitKickoff pkt(getCurrentTeamId());
      sendPacket(pkt);
    }
}

void SRules::kickoffFinished()
{
  if (coach_receiver_ == 0)
    setState(GS_COACH2);
  else
    setState(GS_COACH1);
		
  msgPlayTurn(NULL);
}

void SRules::msgPlayTurn(const MsgEndTurn*)
{
  // Next turn if this is the receiving team.
  if (getState() == GS_COACH1 && coach_begin_ == 1
      || getState() == GS_COACH2 && coach_begin_ == 0)
    {
      cur_turn_++;
      LOG2("=== Go on turn `" << cur_turn_ << "'");
    }
  else
    LOG2("=== Next team play turn `" << cur_turn_ << "'");

  // Finished ? Go on the next half ? 
  // FIXME: 3 is for tests. must be 8.
  if (cur_turn_ > 3)
    {
      initHalf();
      return;
    }
  // Switch playing team
  setState(getState() == GS_COACH1 ? GS_COACH2 : GS_COACH1);
	
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
  pkt.cur_turn = cur_turn_;
  sendPacket(pkt);
  timer_.restart();
}

void SRules::msgForwardChat(const MsgChat* m)
{
  sendPacket(*m);
}

void SRules::msgMoveTurnMarker(const ActMoveTurnMarker* m)
{
  // If its not its turn, warn.
  if (getState() == GS_COACH1 && m->client_id == 1
      || getState() == GS_COACH2 && m->client_id == 0)
    {
      sendIllegal(ACT_MOVETURNMARKER, m->client_id);
      return;
    }

  // Ok. Acknowledge.
  //team_[m->client_id]->setTurnMarker();
  sendPacket(*m);
}

void SRules::msgIllegalProcedure(const ActIllegalProc*)
{
  // FIXME: todo
}


// If arbiter is loaded as a dynamic library, this is the
// method to call to get an instance of bb5 SRules.
extern "C" BaseSRules* load_server_rules(xml::XMLConfig*)
{
  return new SRules;
}
