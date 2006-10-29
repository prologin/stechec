/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#include "Contest.hh"
#include "SRules.hh"
#include "GameClient.hh"

SRules::SRules(StechecGameData* data, StechecServer* server,
               StechecServerResolver* resolver, StechecServerEntry* serverep)
  : wait_nb_(0),
    data_(data),
    server_(server),
    resolver_(resolver),
    server_entry_(serverep)
{
  for (int i = 0; i < MAX_COACH; i++)
    coach_error_[i] = NULL;
  
  HANDLE_WITH(STECHEC_PKT, SRules, this, msgStechecPkt, GS_AFTERTURN);
  HANDLE_WITH(MSG_INIT_GAME, SRules, this, msgInitGame, GS_INITGAME);
  HANDLE_WITH(MSG_BEFORE_TURN, SRules, this, msgBeforeTurn, GS_BEFORETURN);
  HANDLE_WITH(MSG_AFTER_TURN, SRules, this, msgAfterTurn, GS_AFTERTURN);
  HANDLE_WITH(MSG_CHAMPION_ERROR, SRules, this, msgChampionError, GS_ALL);

  setState(GS_BEFOREGAME);
}

SRules::~SRules()
{
  if (getState() != GS_END
      && getState() != GS_BEFOREGAME
      && getState() != GS_WAIT)
    server_entry_->afterGame();

  delete server_entry_;
  delete resolver_;
  delete server_;
  delete data_;

  std::for_each(coach_error_, coach_error_ + MAX_COACH, Deleter());
}

// Check if a hook was successful, and send data to clients
bool SRules::afterHook(int res, const char* hook_name)
{
  if (res)
    {
      LOG1("Server hook `%1' failed.", hook_name);
      if (getState() != GS_BEFOREGAME && getState() != GS_AFTERGAME)
        {
          LOG4("Prematurly call afterGame()");
          setState(GS_AFTERGAME);
          server_->cleanCommitList();
          int r = server_entry_->afterGame();
          afterHook(r, "afterGame");
        }
      if (getState() != GS_BEFOREGAME)
        sendPacket(MsgAfterGame());
      setState(GS_END);
      return false;
    }

  // Hook may have generated a list of packets, destined to the client. Send them.
  CommandList& cl = server_->getCommitList();
  CommandList::const_iterator it;
  for (it = cl.begin(); it != cl.end(); ++it)
    sendPacket(*it);
  server_->cleanCommitList();

  return true;
}

// Called _once_, when all clients are connected.
void SRules::serverStartup()
{
  data_->nb_player_ = getTeamNumber();

  int r = server_entry_->beforeGame();
  if (!afterHook(r, "beforeGame"))
    return;

  sendPacket(MsgBeforeGame());
  setState(GS_INITGAME);
}

// Called periodically, to handle things such as timer.
void SRules::serverProcess()
{
  if (getState() == GS_PLAYTURN
      && (!(getViewerState() & VS_HAVEVIEWER)
          || getViewerState() & VS_READY))
    {
      setViewerState(getViewerState() & ~VS_READY);

      // Go to the next turn.
      data_->current_turn_++;
      LOG2("================== Turn %1 ==================",
	   data_->getCurrentTurn());
      
      int r = server_entry_->beforeNewTurn();
      if (!afterHook(r, "beforeNewTurn"))
        return;
      
      sendPacket(MsgBeforeTurn());
      setState(GS_AFTERTURN);
    }
}

bool  SRules::coachKilled(int coach_id, CoachErrorCustom*& cec)
{
  LOG2("coach killed: %1 %2", coach_id, coach_error_[coach_id]);
  if (coach_error_[coach_id] != NULL)
    {
      // We already have a failure reports from the client itself.
      cec = coach_error_[coach_id];
      coach_error_[coach_id] = NULL; 
    }
  else
    {
      // Probably a network error. Keep track of current_turn_.
      cec = new CoachErrorCustom;
      cec->fail_turn_ = data_->getCurrentTurn();
    }
  return true;
}

void  SRules::outputStat(int coach_id, ClientStatistic& coach_stat)
{
  std::string fail_msg = "Success";
  int fail_turn = -1;

  LOG5("Output statistics for coach id `%1'", coach_id);
  if (coach_stat.custom_ != NULL && coach_stat.custom_->fail_reason_ != "")
    fail_msg = coach_stat.custom_->fail_reason_;
  else if (coach_stat.fail_reason_ != "")
    fail_msg = coach_stat.fail_reason_;
  if (coach_stat.custom_ != NULL)
    fail_turn = coach_stat.custom_->fail_turn_;

  std::cout << coach_stat.ext_id_ << " "
            << server_entry_->getScore(coach_id) << " "
            << fail_turn << " "
            << "(" << fail_msg << ")"
            << std::endl;
  delete coach_stat.custom_;
}

bool SRules::waitAllClient(int client_id)
{
  // Check that the same client hasn't signaled twice
  for (int i = 0; i < wait_nb_; i++)
    if (wait_tab_[i] == client_id)
      {
        WARN("Client `%1' has signaled twice, state: %2", client_id, getState());
        return false;
      }

  wait_tab_[wait_nb_++] = client_id;
  if (wait_nb_ == getTeamNumber())
    {
      wait_nb_ = 0;
      return true;
    }
  return false;
}

// All StechecPkt are for StechecServerServer.
void SRules::msgStechecPkt(const StechecPkt* m)
{
  StechecPkt pkt(*m);
  command_list_.push_back(pkt);
}

void SRules::msgInitGame(const MsgInitGame* m)
{
  if (!waitAllClient(m->client_id))
    return;

  int r = server_entry_->initGame();
  if (!afterHook(r, "initGame"))
    return;

  sendPacket(MsgInitGame());
  setState(GS_BEFORETURN);
}

void SRules::msgBeforeTurn(const MsgBeforeTurn* m)
{
  if (!waitAllClient(m->client_id))
    return;
  setState(GS_PLAYTURN);

  // Maybe we can begin next turn now, without waiting this function to be
  // automatically called.
  serverProcess();
}

void SRules::msgAfterTurn(const MsgAfterTurn* m)
{
  if (!waitAllClient(m->client_id))
    return;

  resolver_->ApplyResolverPriv(command_list_);
  command_list_.clear();
  
  int r = server_entry_->afterNewTurn();
  if (!afterHook(r, "afterNewTurn"))
    return;

  if (server_entry_->isMatchFinished())
    {
      int r = server_entry_->afterGame();
      afterHook(r, "afterGame");
      sendPacket(MsgAfterGame());
      setState(GS_END);
      return;
    }

  sendPacket(MsgAfterTurn());
  setState(GS_BEFORETURN);
}

void SRules::msgChampionError(const MsgChampionError* m)
{
  if (coach_error_[m->client_id] == NULL)
    {
      CoachErrorCustom* cec = new CoachErrorCustom;
      cec->fail_reason_ = packetToString(m->reason);
      cec->fail_turn_ = data_->getCurrentTurn();
      coach_error_[m->client_id] = cec;
      LOG4("Champion `%1' has fatal error: %2",
           m->client_id, cec->fail_reason_);
    }
}
