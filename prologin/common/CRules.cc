/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2005, 2006, 2007 Prologin
*/

# include "Contest.hh"
# include "CRules.hh"

CRules::CRules(const ConfSection* cfg,
               StechecGameData* data,
               StechecClient* client,
               StechecApi* api,
               StechecClientDiffer* differ,
               StechecClientEntry* clientep)
  : data_(data),
    client_(client),
    api_(api),
    differ_(differ),
    client_entry_(clientep)
{
  HANDLE_WITH(STECHEC_PKT, CRules, this, msgStechecPkt, 0);
  HANDLE_WITH(MSG_LIST_TEAM, CRules, this, msgListTeam, GS_WAIT);
  HANDLE_WITH(MSG_BEFORE_GAME, CRules, this, msgBeforeGame, GS_WAIT);
  HANDLE_WITH(MSG_INIT_GAME, CRules, this, msgInitGame, GS_INITGAME);
  HANDLE_WITH(MSG_BEFORE_TURN, CRules, this, msgBeforeTurn, 0);
  HANDLE_WITH(MSG_AFTER_TURN, CRules, this, msgAfterTurn, GS_AFTERTURN);
  HANDLE_WITH(MSG_AFTER_GAME, CRules, this, msgAfterGame, 0);

  // We won't receive any, but it must know it.
  HANDLE_WITH(CUSTOM_EVENT, CRules, this, msgFoo, 0);

  api_->setClientRules(this);
  client_entry_->flushPacketList();

  limiter_.setConfigLimits(*cfg);
  limiter_.setSignalHandler(this);
}

CRules::~CRules()
{
  delete client_entry_;
  delete differ_;
  delete api_;
  delete client_;
  delete data_;
}

// Hmmm, bad, isn't it ? Try to inform the server...
void CRules::sigSegv()
{
  MsgChampionError pkt_err;
  stringToPacket(pkt_err.reason,
                 "Champion made a so called 'Segmentation Fault'. Plonk.", 64);
  sendPacket(pkt_err);
}

void CRules::sigBus()
{
  MsgChampionError pkt_err;
  stringToPacket(pkt_err.reason,
                 "Champion made a boo boo (bus error). Plonk.", 64);
  sendPacket(pkt_err);
}

void CRules::sigAlrm()
{
  MsgChampionError pkt_err;
  stringToPacket(pkt_err.reason,
                 "Timeout ! Champion had taken too much time.", 64);
  sendPacket(pkt_err);
}

const char* CRules::tokenToString(unsigned token) const
{
  if (token >= sizeof (prologin_token_str) / sizeof (const char *))
    return "(overflow)";
  return prologin_token_str[token];
}

void CRules::msgStechecPkt(const StechecPkt* m)
{
  if (getState() == GS_END)
    return;
  
  StechecPkt pkt(*m);
  if (getState() == GS_AFTERTURN)
    {
      LOG3("Client apply diff: %1", pkt);
      differ_->ApplyDiff(&pkt);
      onEvent(m);
    }
  else
    client_entry_->addPacket(pkt);
}

bool CRules::afterHook(int res, const char* hook_name)
{
  LOG5("Client hook `%1' has been called.", hook_name);
  if (res)
    {
      LOG1("Client hook `%1' failed.", hook_name);
      MsgChampionError pkt_err;
      stringToPacket(pkt_err.reason, "Client hook error.", 64);
      sendPacket(pkt_err);

      setState(GS_END);
      return false;
    }
  client_entry_->flushPacketList();
  return true;
}

void CRules::msgListTeam(const MsgListTeam* m)
{
  data_->team_[m->client_id] = m->team_id;
}
   
// Called before the champion is loaded.
void CRules::msgBeforeGame(const MsgBeforeGame* m)
{
  data_->nb_player_ = m->nb_player;
  data_->nb_team_ = getTeamNumber();
  if (getCoachId() < UID_VIEWER_BASE)
    data_->id_ = getCoachId();

  setState(GS_BEFOREGAME);
  int r = client_entry_->beforeGame();
  if (!afterHook(r, "beforeGame"))
    return;

  sendPacket(MsgInitGame());
  setState(GS_INITGAME);

  // After this function, champion will be loaded, and init_game()
  // called. Put limits.
  limiter_.setTimeLimit();
  limiter_.setSignalCatch();
}

// Called after champion has finished his init_game() function.
void CRules::msgInitGame(const MsgInitGame*)
{
  limiter_.unsetTimeLimit();
  limiter_.unsetSignalCatch();

  int r = client_entry_->initGame();
  if (!afterHook(r, "initGame"))
    return;

  sendPacket(MsgBeforeTurn());
  setState(GS_BEFORETURN);
}

void CRules::msgBeforeTurn(const MsgBeforeTurn*)
{
  if (getState() != GS_BEFORETURN && getState() != GS_AFTERTURN)
    return;
  
  data_->current_turn_++;

  // Only to display correctly the turns for prolo2009
  int t = data_->getCurrentTurn();
  int nb_virtual_turns = 2 * getTeamNumber() + 1; 
  int m = (t - 1) % nb_virtual_turns;  
  int phase;
  if (m < getTeamNumber()) phase = 1;
  if (m == getTeamNumber()) phase = 2;
  if (m > getTeamNumber()) phase = 3;
  LOG2("============= Turn %1, phase %2 =============",
       1 + (t - 1) / nb_virtual_turns, phase);

  // Valid until prolo2008
  //  LOG2("================== Turn %1 ==================",
  //       data_->getCurrentTurn());

  int r = client_entry_->beforeNewTurn();
  if (!afterHook(r, "beforeNewTurn"))
    return;


  if (getCoachId() >= UID_VIEWER_BASE)
    setState(GS_AFTERTURN);
  else
    {
      // Ready to play. Limit it.
      limiter_.setTimeLimit();
      limiter_.setSignalCatch();
      setState(GS_PLAYTURN);
    }
}

// FIXME: bug: all messages generated by server afterTurn will
//  go right into the differ.
void CRules::msgAfterTurn(const MsgAfterTurn*)
{
  int r = client_entry_->afterNewTurn();
  if (!afterHook(r, "afterNewTurn"))
    return;

  // Send an end-of-turn event.
  onEvent(51);

  setState(GS_BEFORETURN);
  sendPacket(MsgBeforeTurn());
}

// Can be called everytime, at the end of the game, or when the server
// has failed an entry point.
// Be prepared to handle it correctly.
void CRules::msgAfterGame(const MsgAfterGame*)
{
  if (getState() == GS_END)
    return;
  
  if (getState() != GS_WAIT)
    {
      int r = client_entry_->afterGame();
      if (!afterHook(r, "afterGame"))
        return;
    }
  setState(GS_END);
}


void CRules::msgFoo(const InternalEventPkt*)
{
  assert(false);
}


// Champion requested an end-of-turn. Send the action list
// to the server.
void CRules::endTurn()
{
  if (getState() != GS_PLAYTURN)
    {
      WARN("Not playing... can't end the turn right now.");
      return;
    }

  // Temporary disable signal and time limiter.
  limiter_.unsetTimeLimit();
  limiter_.unsetSignalCatch();

  CommandList& cl = api_->getCommitList();
  CommandList::const_iterator it;
  for (it = cl.begin(); it != cl.end(); ++it)
    sendPacket(*it);
  cl.clear();

  setState(GS_AFTERTURN);
  sendPacket(MsgAfterTurn());
}
