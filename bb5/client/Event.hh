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

#ifndef EVENT_HH_
# define EVENT_HH_

# include "EventProcess.hh"
# include "Dice.hh"

/*!
** @brief Custom event identifier, for use as BaseCRules::onEvent(int) parameter.
** @ingroup tbt_client_api
*/
enum eCustomEvent {
  eInitGame,
};

/*!
** @brief Base UI events class.
** @ingroup tbt_client_api
**
** Derive from this class and virtual methods will be called...
** when time will come.
*/
class Event
{
public:
  Event() {}
  virtual ~Event() {}

  //! @brief An illegal token (sent by a client) has been spotted by the server.
  virtual void evIllegal(int team_id, int was_token);
  //! @brief A chat message has been received.
  virtual void evChat(const std::string& msg);

  //! @brief Both coaches and arbiter are ready.
  virtual void evInitGame() {}
  //! @brief Game ends, according to the rules.
  virtual void evEndGame() {}
  
  //! @brief A new half begins.
  virtual void evHalf(int half);
  //! @brief Coach is drawn to choose to either kick or receive the ball.
  virtual void evDrawKicker(int team_id, bool is_a_question);
  //! @brief A team is asked to enter into the field.
  virtual void evPlaceTeam(int team_id);
  //! @brief A team is asked to kick the ball off.
  virtual void evKickOff(int team_id);
  //! @brief A coach is asked to give the ball to one of his player.
  virtual void evGiveBall(int team_id, int player_id);
  //! @brief A new turn begins.
  virtual void evNewTurn(int team_id, int cur_half, int cur_turn);
  //! @brief Current coach moves his turn marker.
  //! @attention FIXME: NEVER CALLED FOR NOW
  virtual void evMoveTurnMarker() {}
  //! @brief A turnover happens.
  virtual void evTurnOver(enum eTurnOverMotive motive);
  //! @brief A touchdooown is scored.
  virtual void evTouchdooown(int team_id, int player_id);

  //! @brief Ball position changed.
  virtual void evBallPos(const Point& pos);

  //! @brief A player is created (at the very beginning of the game).
  virtual void evPlayerCreate(int team_id, int player_id);
  //! @brief A player status changed.
  virtual void evPlayerStatus(int team_id, int player_id, enum eStatus status);
  //! @brief A player position changed.
  virtual void evPlayerPos(int team_id, int player_id, const Point& pos);
  //! @brief A player moves of one square.
  virtual void evPlayerMove(int team_id, int player_id, const Point& pos);
  //! @brief A player is knocked down to the ground.
  virtual void evPlayerKnocked(int team_id, int player_id);
  //! @brief A KO'd player tries to wake up, during a break time.
  virtual void evPlayerKO(int team_id, int player_id, int dice);

  //! @brief A player declared an action.
  virtual void evDeclare(int team_id, int player_id, enum eDeclaredAction action);
  //! @brief Dice roll result associated to a player, often agility roll.
  virtual void evResult(int team_id, int player_id, enum eRoll action_type, int result,
                        int modifier, int required, bool reroll, enum eSkill skill);
  //! @brief A player blocks an opponent.
  virtual void evBlock(int team_id, int player_id, int opponent_player_id);
  //! @brief Block dices roll result.
  virtual void evBlockResult(int team_id, int player_id, int opponent_player_id,
                             int nb_dice, enum eBlockDiceFace result[3],
                             int strongest_team_id, bool reroll);
  //! @brief A player pushes another, due to a block.
  virtual void evBlockPush(const Position& pos, int nb_choice, const Position choices[]);
  //! @brief A player is asked to follow or not the player he pushed.
  virtual void evFollow();

  //! @brief A coach decided to use (or not) a team reroll.
  virtual void evReroll(int team_id, bool reroll);
  //! @brief A player used one of his skill.
  virtual void evSkill(int team_id, int player_id, enum eSkill skill, int choice);
};

inline void Event::evIllegal(int, int) {}
inline void Event::evChat(const std::string&) {}

inline void Event::evHalf(int) {}
inline void Event::evDrawKicker(int, bool) {}
inline void Event::evPlaceTeam(int) {}
inline void Event::evKickOff(int) {}
inline void Event::evGiveBall(int, int) {}
inline void Event::evNewTurn(int, int, int) {}
inline void Event::evTurnOver(enum eTurnOverMotive) {}
inline void Event::evTouchdooown(int, int) {}

inline void Event::evBallPos(const Point&) {}

inline void Event::evPlayerCreate(int, int) {}
inline void Event::evPlayerStatus(int, int, enum eStatus) {}
inline void Event::evPlayerPos(int, int, const Point&) {}
inline void Event::evPlayerMove(int, int, const Point&) {}
inline void Event::evPlayerKnocked(int, int) {}
inline void Event::evPlayerKO(int, int, int) {}

inline void Event::evDeclare(int, int, enum eDeclaredAction) {}
inline void Event::evResult(int, int, enum eRoll, int, int, int, bool, enum eSkill) {}
inline void Event::evBlock(int, int, int) {}
inline void Event::evBlockResult(int, int, int, int, enum eBlockDiceFace[3], int, bool) {}
inline void Event::evBlockPush(const Position&, int, const Position[]) {}
inline void Event::evFollow() {}

inline void Event::evReroll(int, bool) {}
inline void Event::evSkill(int, int, enum eSkill, int) {}

#endif /* !EVENT_HH_ */
