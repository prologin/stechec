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

enum {
  eInitGame,
};

/*!
** Base UI events class.
**
** Derive from this class and virtual methods will be called...
** when time will come.
*/
class Event
{
public:
  Event() {}
  virtual ~Event() {}

  virtual void evIllegal(int team_id, int was_token);
  virtual void evInitGame() {}
  virtual void evNewTurn(int team_id, int cur_half, int cur_turn);
  virtual void evEndGame() {}
  virtual void evResult(int team_id, int player_id, enum eRoll action_type, int result,
                        int modifier, int required, bool reroll, enum eSkill skill);
  virtual void evBlock(int team_id, int player_id, int opponent_player_id);
  virtual void evBlockResult(int team_id, int player_id, int opponent_player_id,
                             int nb_dice, enum eBlockDiceFace result[3],
                             int strongest_team_id, bool reroll);
  virtual void evReroll(int team_id, bool reroll);
  virtual void evSkill(int team_id, int player_id, enum eSkill skill, int choice);
  virtual void evHalf(int half);
  virtual void evDrawKicker(int team_id, bool is_a_question);
  virtual void evPlaceTeam(int team_id);
  virtual void evKickOff(int team_id);
  virtual void evGiveBall(int team_id, int player_id);
  virtual void evMoveTurnMarker() {}
  virtual void evTurnOver(enum eTurnOverMotive motive);
  virtual void evTouchdooown(int team_id, int player_id);
  virtual void evChat(const std::string& msg);
  virtual void evBallPos(const Point& pos);
  virtual void evPlayerCreate(int team_id, int player_id);
  virtual void evPlayerPos(int team_id, int player_id, const Point& pos);
  virtual void evPlayerMove(int team_id, int player_id, const Point& pos);
  virtual void evPlayerKnocked(int team_id, int player_id);
  virtual void evPlayerStatus(int team_id, int player_id, enum eStatus status);
  virtual void evPlayerKO(int team_id, int player_id, int dice);
  virtual void evFollow();
  virtual void evDeclare(int team_id, int player_id, enum eDeclaredAction action);
  virtual void evBlockPush(const Position& pos, int nb_choice, const Position choices[]);
};

inline void Event::evIllegal(int, int) {}
inline void Event::evNewTurn(int, int, int) {}
inline void Event::evResult(int, int, enum eRoll, int, int, int, bool, enum eSkill) {}
inline void Event::evBlock(int, int, int) {}
inline void Event::evBlockResult(int, int, int, int, enum eBlockDiceFace[3], int, bool) {}
inline void Event::evReroll(int, bool) {}
inline void Event::evSkill(int, int, enum eSkill, int) {}
inline void Event::evHalf(int) {}
inline void Event::evDrawKicker(int, bool) {}
inline void Event::evPlaceTeam(int) {}
inline void Event::evKickOff(int) {}
inline void Event::evGiveBall(int, int) {}
inline void Event::evTurnOver(enum eTurnOverMotive) {}
inline void Event::evTouchdooown(int, int) {}
inline void Event::evChat(const std::string&) {}
inline void Event::evBallPos(const Point&) {}
inline void Event::evPlayerCreate(int, int) {}
inline void Event::evPlayerPos(int, int, const Point&) {}
inline void Event::evPlayerMove(int, int, const Point&) {}
inline void Event::evPlayerKnocked(int, int) {}
inline void Event::evPlayerStatus(int, int, enum eStatus) {}
inline void Event::evPlayerKO(int, int, int) {}
inline void Event::evFollow() {}
inline void Event::evDeclare(int, int, enum eDeclaredAction) {}
inline void Event::evBlockPush(const Position&, int, const Position[]) {}

#endif /* !EVENT_HH_ */
