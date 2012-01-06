/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2005, 2006, 2007 Prologin
*/

#include "Contest.hh"
#include "SRules.hh"
#include "Client.hh"

SRules::SRules(StechecGameData* data, StechecServer* server,
               StechecServerResolver* resolver, StechecServerEntry* serverep,
               int team_nb, int player_per_team_nb)
  : BaseSRules(player_per_team_nb * team_nb),
    nb_team_(team_nb),
    wait_nb_(0),
    wait_ok_(false),
    player_killed_nb_(0),
    league_tab_size_(0),
    data_(data),
    server_(server),
    resolver_(resolver),
    server_entry_(serverep)
{
  for (int i = 0; i < MAX_PLAYER; i++)
    player_error_[i] = NULL;
  
  HANDLE_WITH(STECHEC_PKT, SRules, this, msgStechecPkt, GS_PLAYTURN);
  HANDLE_WITH(MSG_INIT_GAME, SRules, this, msgInitGame, GS_INITGAME);
  HANDLE_WITH(MSG_BEFORE_TURN, SRules, this, msgBeforeTurn, GS_BEFORETURN);
  HANDLE_WITH(MSG_AFTER_TURN, SRules, this, msgAfterTurn, GS_PLAYTURN);
  HANDLE_WITH(MSG_CHAMPION_ERROR, SRules, this, msgChampionError, 0);

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

  std::for_each(player_error_, player_error_ + MAX_PLAYER, Deleter());
}

const char* SRules::tokenToString(unsigned token) const
{
  if (token >= sizeof (prologin_token_str) / sizeof (const char *))
    return "(overflow)";
  return prologin_token_str[token];
}

// Correspondance id <-> league_id will be kept in StechecGameData.
// Remap league_id to team_id in the range [0 - MAX_TEAM-1]
void SRules::addPlayer(int client_id, int league_id)
{
  if (client_id < 0 || client_id >= MAX_PLAYER || league_id < 0)
    return;

  int i;
  for (i = 0; i < league_tab_size_; i++)
    if (league_tab_[i].league_id == league_id)
      {
	league_tab_[i].count++;
	break;
      }
  if (i == league_tab_size_)
    {
      league_tab_[i].league_id = league_id;
      league_tab_[i].count = 1;
      if (++league_tab_size_ > MAX_TEAM)
	ERR("Too much team (max %1)", MAX_TEAM);
    }

  data_->team_[client_id] = i;
  MsgListTeam pkt(client_id);
  pkt.team_id = i;
  sendPacket(pkt);
}

// Check if a hook was successful, and send data to clients
bool SRules::afterHook(int res, const char* hook_name)
{
  if (res)
    {
      LOG3("Server hook `%1' failed.", hook_name);
      if (getState() != GS_BEFOREGAME && getState() != GS_AFTERGAME)
        {
          LOG4("Prematurly call afterGame()");
          setState(GS_AFTERGAME);
          server_->cleanCommitList();
          int r = server_entry_->afterGame();
          afterHook(r, "afterGame");
        }
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

// Check all teams are full, and we have the exact number of announced
// connected team / clients we should expect from parameters.
// Maybe we can remove this check, but rules must be made accordingly.
bool SRules::checkTeamFilling()
{
  const int nb_player = getTeamNumber();
  const int player_per_team = nb_player / nb_team_;

  if (league_tab_size_ != nb_team_)
    {
      ERR("Not enough, or too much teams connected (%1/%2)",
	  league_tab_size_, nb_team_);
      return false;
    }

  for (int i = 0; i < nb_player; i++)
    if (data_->team_[i] == -1)
      {
	ERR("Team id %1 is not filled", i);
	return false;
      }

  for (int i = 0; i < nb_team_; i++)
    if (league_tab_[i].count != player_per_team)
      {
	ERR("Not enough player connected to the team %1 (%2/%3)",
	    league_tab_[i].league_id, league_tab_[i].count, player_per_team);
	return false;
      }
  return true;
}

// Called _once_, when all clients are connected.
void SRules::serverStartup()
{
  data_->nb_player_ = getTeamNumber();
  data_->nb_team_ = nb_team_;

  if (!checkTeamFilling())
    {
      sendPacket(MsgAfterGame());
      setState(GS_END);
      return;
    }
  
  int r = server_entry_->beforeGame();
  if (!afterHook(r, "beforeGame"))
    return;

  MsgBeforeGame mbg;
  mbg.nb_player = data_->nb_player_;
  sendPacket(mbg);

  setState(GS_INITGAME);
}

// Called periodically, to handle things such as timer.
void SRules::serverProcess()
{
  // end of turn, and everybody's ready.
  if (getState() == GS_PLAYTURN && wait_ok_ &&
      (!(getViewerState() & VS_HAVEVIEWER) || getViewerState() & VS_READY))
    {
      setViewerState(getViewerState() & ~VS_READY);
      if (wait_nb_)
        WARN("%1: wait_nb_ %2", __func__, wait_nb_);
      wait_ok_ = false;

      resolver_->ApplyResolverPriv(command_list_);
      command_list_.clear();
  
      int r = server_entry_->afterNewTurn();
      if (!afterHook(r, "afterNewTurn"))
        return;

      if (server_entry_->isMatchFinished())
        {
          // end of match
          int r = server_entry_->afterGame();
          afterHook(r, "afterGame");
          sendPacket(MsgAfterGame());
          setState(GS_END);
        }
      else
        {
          sendPacket(MsgAfterTurn());
          setState(GS_BEFORETURN);

          // Go to the next turn.
          data_->current_turn_++;
          int turn = (data_->current_turn_ + 1)/ 2;
          int player = (data_->current_turn_ - 1) % 2;
          LOG2("================== Turn %1, player %2 ==================",
               turn, player + 1);

          int r = server_entry_->beforeNewTurn();
          if (!afterHook(r, "beforeNewTurn"))
            return;
        }
    }

  // Stuck on INITGAME, unlock.
  if (getState() == GS_INITGAME && wait_ok_)
    {
      MsgInitGame fake_init(-1);
      msgInitGame(&fake_init);
    }

  // Stuck on BEFORETURN, unlock.
  if (getState() == GS_BEFORETURN && wait_ok_)
    {
      MsgBeforeTurn fake_bt(-1);
      msgBeforeTurn(&fake_bt);
    }
}

bool  SRules::coachKilled(int coach_id, CoachErrorCustom*& cec)
{
  player_killed_nb_++;
  if (player_error_[coach_id] != NULL)
    {
      // We already have a failure reports from the client itself.
      cec = player_error_[coach_id];
      player_error_[coach_id] = NULL; 
    }
  else
    {
      // Probably a network error. Keep track of current_turn_.
      cec = new CoachErrorCustom;
      cec->fail_turn_ = data_->getCurrentTurn();
    }

  // Could be the last client we waited for. Unlock us.
  for (int i = 0; i < wait_nb_; i++)
    if (wait_tab_[i] == coach_id)
      wait_tab_[i] = wait_tab_[--wait_nb_];
  waitAllClient(-1);

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
  if (client_id >= 0)
    {
      // Check that the same client hasn't signaled twice
      for (int i = 0; i < wait_nb_; i++)
        if (wait_tab_[i] == client_id)
          {
            WARN("Client `%1' has signaled twice, state: %2", client_id, getState());
            return false;
          }
      
      wait_tab_[wait_nb_++] = client_id;
    }
  if (wait_nb_ >= data_->nb_player_ - player_killed_nb_)
    {
      wait_ok_ = true;
      wait_nb_ = 0;
    }
  return wait_ok_;
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
  if (wait_nb_)
    WARN("%1: wait_nb_ %2", __func__, wait_nb_);
  wait_ok_ = false;

  int r = server_entry_->initGame();
  if (!afterHook(r, "initGame"))
    return;

  sendPacket(MsgInitGame());
  setState(GS_BEFORETURN);

  // First turn
  data_->current_turn_ = 1;
  LOG2("================== Turn %1, player %1 ==================",
       1, 1);

  r = server_entry_->beforeNewTurn();
  if (!afterHook(r, "beforeNewTurn"))
    return;
}

void SRules::msgBeforeTurn(const MsgBeforeTurn* m)
{
  if (!waitAllClient(m->client_id))
    return;
  if (wait_nb_)
    WARN("%1: wait_nb_ %2", __func__, wait_nb_);
  wait_ok_ = false;

  sendPacket(MsgBeforeTurn());
  setState(GS_PLAYTURN);
}

void SRules::msgAfterTurn(const MsgAfterTurn* m)
{
  waitAllClient(m->client_id);
  // Next turn will be handled in serverProcess.
}

void SRules::msgChampionError(const MsgChampionError* m)
{
  if (player_error_[m->client_id] == NULL)
    {
      CoachErrorCustom* cec = new CoachErrorCustom;
      cec->fail_reason_ = packetToString(m->reason);
      cec->fail_turn_ = data_->getCurrentTurn();
      player_error_[m->client_id] = cec;
      LOG4("Champion `%1' has fatal error: %2",
           m->client_id, cec->fail_reason_);
    }
}
