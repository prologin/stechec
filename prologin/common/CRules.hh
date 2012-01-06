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

#ifndef CRULES_HH_
# define CRULES_HH_

# include "BaseCRules.hh"
# include "NutsBarrier.hh"
# include "Rules.hh"

// Mostly a big hack, to have onEvent(int) working.
DECLARE_EMPTY_PACKET(CUSTOM_EVENT, InternalEventPkt);

/*!
** @ingroup stechec
** @{
*/

// Forward decl.
class StechecGameData;
class StechecClient;
class StechecApi;
class StechecClientDiffer;
class StechecClientEntry;

class CRules : public BaseCRules, public SignalHandler
{
public:
  CRules(const ConfSection* cfg, StechecGameData* data, StechecClient* client,
         StechecApi* api, StechecClientDiffer* differ, StechecClientEntry* clientep);
  virtual ~CRules();

  //! @brief Get the limiter. Used to get remaining time values.
  const NutsBarrier& getLimiter() const;

  //! @brief Called by the api when a champion has finished its turn.
  void endTurn();

private:
  // not yet used.
  virtual void serialize(std::ostream&) const {}
  virtual void unserialize(std::istream&) {}

  virtual const char* tokenToString(unsigned token) const;

  virtual Api* getApi() { return reinterpret_cast<Api*>(api_); }

  virtual void sigSegv();
  virtual void sigBus();
  virtual void sigAlrm();

  bool afterHook(int res, const char* hook_name);

  void msgStechecPkt(const StechecPkt* m);
  void msgListTeam(const MsgListTeam* m);
  void msgBeforeGame(const MsgBeforeGame* m);
  void msgInitGame(const MsgInitGame* m);
  void msgBeforeTurn(const MsgBeforeTurn* m);
  void msgAfterTurn(const MsgAfterTurn* m);
  void msgAfterGame(const MsgAfterGame* m);
  void msgFoo(const InternalEventPkt* m);

  NutsBarrier           limiter_;

  StechecGameData*      data_;
  StechecClient*        client_;
  StechecApi*           api_;
  StechecClientDiffer*  differ_;
  StechecClientEntry*   client_entry_;
};

//! @}

inline const NutsBarrier& CRules::getLimiter() const
{
  return limiter_;
}

#endif // !CRULES_HH_
