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

#ifndef SRULES_HH_
# define SRULES_HH_

# include "BaseSRules.hh"
# include "Field.hh"
# include "SBall.hh"
# include "SWeather.hh"
# include "STeam.hh"
# include "SPlayer.hh"
# include "SActionHandler.hh"

typedef Field<SPlayer> SField;
class STeamMsg;
class SPlayerMsg;

/*!
** @brief Rules implementation for the server.
** @ingroup rules
**
** It symbolizes the main referee, who manages
** game state, teams, turns, turnover and score.
**
** You will have only one instance of this class at runtime,
** handled by the stechec server.
*/
class SRules : public BaseSRules
{
public:
  SRules();
  virtual ~SRules();
  
  //! @brief Send a MSG_ILLEGAL packet. Deprecated.
  void sendIllegal(int token, int from) const;
  //! @brief Send a MSG_ILLEGAL packet with error code.
  void sendIllegal(int token, int from, enum eError error) const;
  
  SField* getField();
  SBall* getBall();
  STeam* getTeam(int team_id);
  Dice* getDice();
  SActionHandler* getActionHandler();
 
  //! @brief Return the current playing team id.
  //! @return -1 if no one play, otherwise 0 or 1.
  int getCurrentTeamId() const;

  //! @brief Return the current playing team.
  //! @return NULL if no one play.
  STeam* getCurrentTeam();

  //! @brief Return the current opposing playing team id.
  //! @return -1 if no one play, otherwise 0 or 1.
  int getCurrentOpponentTeamId() const;

  //! @brief Return the current opposing playing team.
  //! @return NULL if no one play.
  STeam* getCurrentOpponentTeam();

  int getOpponentTeamId(int team_id) const;
  int getOpponentTeamId(STeam* team) const;
  STeam* getOpponentTeam(int team_id);
  STeam* getOpponentTeam(STeam* team);

  //! @brief Called when game is starting.
  virtual void serverStartup();

  //! @brief Called periodically from server.
  //! Check if turn time is not elapsed, otherwise
  //! go on next turn.
  virtual void serverProcess();

  //! @brief The ball is in game, receiving team can play
  void finishKickoff();

  //! @brief Go on next turn.
  void nextTurn();

  //! @brief Announce a turnover.
  void turnover(enum eTurnOverMotive motive);
  //! @brief Gets the motive of the turnover, or TOM_NONE.
  enum eTurnOverMotive getTurnoverMotive();

  //! @brief Keeps the scoring player in mind and annonces a turnover.
  void touchdooown(SPlayer* p);
  //! @brief Launches kick-off, if needed, after a touchdooown.
  void afterTouchdooown();

  virtual void serialize(std::ostream& os) const;
  virtual void unserialize(std::istream& is);

private:

  const char* tokenToString(unsigned token) const;

  //! @brief Initialize rules and launch game.
  //! called once in a game.
  void initGame();
  //! @brief Called before a half.
  void initHalf();
  //! @brief Called before a kickoff.
  void initKickoff();
  
  bool checkGameEnd();

  void msgInitGame(const MsgInitGame* m);
  void msgDrawKicker(const MsgDrawKicker* m);
  void msgInitKickoff(const MsgInitKickoff* m);
  void msgPlayTurn(const MsgEndTurn* m);
  void msgForwardChat(const MsgChat* m);
  void msgMoveTurnMarker(const MsgMoveTurnMarker* m);
  void msgIllegalProcedure(const MsgIllegalProc* m);

  Timer     timer_;

  //! @brief Current turn number, in the range of [1, NB_TURNS], or 0 (half-time kick-off).
  int       cur_turn_;
  int       cur_half_;
  enum eTurnOverMotive turnover_;
  STeam*    team_[2];
  SWeather* weather_;
  SBall*    ball_;
  SField*   field_;
  Dice*     dice_;
  STeamMsg* team_msg_;
  SPlayerMsg* player_msg_;
  SActionHandler* action_handler_;
  //! @brief Coach who chooses to kick or receive the ball.
  //!   Coach who receives the ball at the beginning of the half-time.
  int       coach_begin_;
  //! @brief Coach who receives the ball.
  int       coach_receiver_;
  int       scoring_team_;
};

# include "SRules.hxx"

#endif /* !SRULES_HH_ */
