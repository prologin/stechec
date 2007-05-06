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

#ifndef SRULES_HH_
# define SRULES_HH_

/*!
** @ingroup stechec
** @{
*/

# include "BaseSRules.hh"
# include "Rules.hh"
# include "StechecGameData.hh"

// Forward decl.
class StechecGameData;
class StechecServer;
class StechecServerResolver;
class StechecServerEntry;

/*!
** This class is filled when a coach is disconnected, then
** used to ouput statistics.
*/
class CoachErrorCustom
{
public:
  int           fail_turn_;
  std::string   fail_reason_;
};

/*!
** Stechec BaseSRules.
*/
class SRules : public BaseSRules
{
public:
  SRules(StechecGameData* data, StechecServer* server,
	 StechecServerResolver* resolver, StechecServerEntry* serverep);
  virtual ~SRules();

private:
  // not yet used.
  virtual void serialize(std::ostream&) const {}
  virtual void unserialize(std::istream&) {}

  virtual const char* tokenToString(unsigned token) const;

  virtual void addPlayer(int client_id, int league_id);

  virtual void serverStartup();
  virtual void serverProcess();

  virtual bool coachKilled(int coach_id, CoachErrorCustom*& cec);
  virtual void outputStat(int coach_id, ClientStatistic& coach_stat);

  bool afterHook(int res, const char* hook_name);
  bool waitAllClient(int client_id);
  bool checkTeamFilling();
    
  void msgStechecPkt(const StechecPkt* m);
  void msgInitGame(const MsgInitGame* m);
  void msgBeforeTurn(const MsgBeforeTurn* m);
  void msgAfterTurn(const MsgAfterTurn* m);
  void msgChampionError(const MsgChampionError* m);

  int                    wait_nb_;			///< Number of ready player.
  bool                   wait_ok_;
  int                    wait_tab_[MAX_PLAYER];		///< Uid list of ready player.
  CoachErrorCustom*      coach_error_[MAX_PLAYER];	///< Array of received player error.
  int                    coach_killed_nb_;        ///< Number of coach killed.
  
  // Correspondance between league_id and team_id.
  struct LeagueTab
  {
    int league_id;
    int count;
  } league_tab_[MAX_TEAM];
  int league_tab_size_;
  
  CommandList command_list_;

  StechecGameData*       data_;
  StechecServer*         server_;
  StechecServerResolver* resolver_;
  StechecServerEntry*    server_entry_;
};

//! @}

#endif // !SRULES_HH_
