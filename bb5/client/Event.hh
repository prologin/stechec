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

#ifndef EVENT_HH_
# define EVENT_HH_

# include "EventProcess.hh"

enum {
  eKickOff
};

/*!
** Base GUI events class.
**
** Derive from this class and virtual methods will be called...
** when time will come.
*/
class Event
{
public:
  Event() {}
  virtual ~Event() {}

  virtual void evIllegal() {}
  virtual void evNewTurn() {}
  virtual void evEndGame() {}
  virtual void evKickOff() {}
  virtual void evMoveTurnMarker() {}
  virtual void evTimeExceeded() {}
  virtual void evChat(const std::string& msg);
  virtual void evBallPos(const Point& pos);
  virtual void evPlayerPos(int team_id, int player_id, const Point& pos);
  virtual void evPlayerMove(int team_id, int player_id, const Point& pos);
  virtual void evPlayerKnocked(int team_id, int player_id);
};

inline void Event::evChat(const std::string&) {}
inline void Event::evBallPos(const Point&) {}
inline void Event::evPlayerPos(int, int, const Point&) {}
inline void Event::evPlayerMove(int, int, const Point&) {}
inline void Event::evPlayerKnocked(int, int) {}

#endif /* !EVENT_HH_ */
