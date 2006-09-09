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

#ifndef CRULES_HH_
# define CRULES_HH_

# include "BaseCRules.hh"
# include "Weather.hh"
# include "CField.hh"
# include "CBall.hh"
# include "CTeam.hh"

//! @brief Constants for switchTeam().
enum eSelTeam {
  TEAM1 = 0,    ///< Get information on team 1.
  TEAM2,        ///< Get information on team 2.
  US,           ///< Get information on our team.
  THEM          ///< Get information on the other them.
};

// Mostly a big hack, to have BaseCRules::onEvent(int) working.
DECLARE_EMPTY_PACKET(CUSTOM_EVENT, CustomEvent);

class Api;

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

  //! @brief Get the api.
  virtual Api*  getApi();

private:

  // Methods to catch server response.
  void msgIllegal(const MsgIllegal* m);
  void msgInitGame(const MsgInitGame* m);
  void msgInitHalf(const MsgInitHalf* m);
  void msgInitKickoff(const MsgInitKickoff* m);
  void msgGiveBall(const MsgGiveBall* m);
  void msgPlayTurn(const MsgNewTurn* m);
  void msgEndGame(const MsgEndGame* m);
  void msgTimeExceeded(const MsgTimeExceeded* m);
  void msgChatMessage(const MsgChat* m);
  void msgMoveTurnMarker(const ActMoveTurnMarker* m);
  void msgCustomEvent(const CustomEvent* m);
	void msgResult(const MsgResult* m);
	void msgBlockResult(const MsgBlockResult* m);
	void msgBlockDice(const MsgBlockDice* m);
	void msgFollow(const MsgFollow* m);
	
	void actBlockPush(const ActBlockPush* m);

  const xml::XMLConfig& cfg_;
  Api*                  api_;

  int       cur_turn_;
  int       cur_half_;
  Weather*  weather_;
  CBall*    ball_;
  CField*   field_;
  CTeam*    our_team_;
  CTeam*    other_team_;
};

# include "CRules.hxx"

#endif /* !CRULES_HH_ */
