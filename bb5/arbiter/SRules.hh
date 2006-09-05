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

#ifndef SRULES_HH_
# define SRULES_HH_

# include "BaseSRules.hh"
# include "Field.hh"
# include "SBall.hh"
# include "SWeather.hh"
# include "STeam.hh"
# include "SPlayer.hh"

typedef Field<SPlayer> SField;

/*! @brief Rules implementation for the server.
**
** @author victor
** @date 30/12/2005
*/
class SRules : public BaseSRules
{
public:
  SRules();
  virtual ~SRules();
  
  //! @brief Send a MSG_ILLEGAL packet.
  void sendIllegal(int token, int from) const;
  
  SField* getField();
  SBall* getBall();
  STeam* getTeam(int team_id);

  //! @brief Return the current playing team.
  //! @return -1 if no one play, otherwise 0 or 1.
  int getCurrentTeamId() const;

  //! @brief Return the current opposing playing team.
  //! @return -1 if no one play, otherwise 0 or 1.
  int getCurrentOpponentTeamId() const;

  //! @brief Called when game is starting.
  virtual void serverStartup();

  //! @brief Called periodically from server.
  //! Check if turn time is not elapsed, otherwise
  //! go on next turn.
  virtual void serverProcess();

  //! @brief Cause a turn over. Call it when, eg, an action failed.
  void turnOver();
  
	//! @brief Adapt score and launch the kickoff
	void touchdown();

  virtual void serialize(std::ostream& os) const;
  virtual void unserialize(std::istream& is);

  
private:

  //! @brief Initialize rules and launch game.
  //! called once in a game.
  void initGame();
	//!@ brief Called before a half.
  void initHalf();
  //!@ brief Called before a kickoff.
  void initKickoff();


  
  void msgInitGame(const MsgInitGame* m);
  void msgInitKickoff(const MsgInitKickoff* m);
  void msgPlayTurn(const MsgEndTurn* m);
  void msgForwardChat(const MsgChat* m);
  void msgMoveTurnMarker(const ActMoveTurnMarker* m);
  void msgIllegalProcedure(const ActIllegalProc* m);

  Timer     timer_;

  int       cur_turn_;
  int       cur_half_;
  STeam*    team_[2];
  SWeather* weather_;
  SBall*    ball_;
  SField*   field_;
  int       coach_begin_;
	int				coach_receiver_;	
};

# include "SRules.hxx"

#endif /* !SRULES_HH_ */
