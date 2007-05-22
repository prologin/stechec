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

#ifndef CRULES_HH_
# define CRULES_HH_

# include "BaseCRules.hh"
# include "BaseApi.hh"
# include "Constants.hh"
# include "Weather.hh"
# include "CTeam.hh"
# include "CPlayer.hh"
# include "CBall.hh"
# include "CField.hh"

// Mostly a big hack, to have BaseCRules::onEvent(int) working.
DECLARE_EMPTY_PACKET(CUSTOM_EVENT, CustomEvent);

namespace xml { class XMLConfig; }
class Api;
class Weather;
class CTeamMsg;
class CPlayerMsg;

//! @brief Constants for switchTeam().
enum eSelTeam {
  TEAM1 = 0,    ///< Get information on team 1.
  TEAM2,        ///< Get information on team 2.
  US,           ///< Get information on our team.
  THEM          ///< Get information on the other them.
};

/*!
** @brief Rules implementation for the client.
*/
class CRules : public BaseCRules
{
  friend class Api;
  
public:
  CRules(const xml::XMLConfig& cfg);
  virtual ~CRules();

  CField* getField();

  // FIXME: see later.
  virtual void serialize(std::ostream& os) const;
  virtual void unserialize(std::istream& is);

  //! @brief Gets the api.
  virtual Api* getApi();

  void switchToTeamState(int state);
  void restoreGameState();

private:

  virtual const char* tokenToString(unsigned token) const;

  // Methods to catch server response.
  void msgIllegal(const MsgIllegal* m);
  void msgInitGame(const MsgInitGame* m);
  void msgInitHalf(const MsgInitHalf* m);
  void msgDrawKicker(const MsgDrawKicker* m);
  void msgInitKickoff(const MsgInitKickoff* m);
  void msgGiveBall(const MsgGiveBall* m);
  void msgPlayTurn(const MsgNewTurn* m);
  void msgEndGame(const MsgEndGame* m);
  void msgTimer(const MsgTimer* m);
  void msgTurnOver(const MsgTurnOver* m);
  void msgChatMessage(const MsgChat* m);
  void msgMoveTurnMarker(const MsgMoveTurnMarker* m);
  void msgCustomEvent(const CustomEvent* m);
  void msgResult(const MsgResult* m);
  void msgBlockResult(const MsgBlockResult* m);
  void msgBlockDice(const MsgBlockDice* m);
  void msgFollow(const MsgFollow* m);
  void msgBlockPush(const MsgBlockPush* m);

  const xml::XMLConfig& cfg_;
  Api*                  api_;
  Timer			timer_;

  int       saved_game_state_;
  int       cur_turn_;
  int       cur_half_;
  Weather*  weather_;
  CBall*    ball_;
  CField*   field_;
  CTeam*    our_team_;
  CTeam*    other_team_;
  CTeamMsg* team_msg_;
  CPlayerMsg* player_msg_;
};

# include "CRules.hxx"

#endif /* !CRULES_HH_ */
