/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#ifndef EVENT_HH_
# define EVENT_HH_

# include "StechecEvent.hh"

class OutAnEvent : public Event
{
protected:
  virtual void endOfTurn() = 0;
  virtual void goodmanMove(int team_id, int unit_id, const Position& to);
  virtual void goodmanChangeState(int team_id, int unit_id, int new_state);
  virtual void goodmanLostDelorean(int team_id, int unit_id);
  virtual void goodmanGetDelorean(int team_id, int unit_id);
  virtual void goodmanLostAlmanach(int team_id, int unit_id);
  virtual void goodmanGetAlmanach(int team_id, int unit_id);
  virtual void deloreanMove(const Position& pos);
  virtual void almanachMove(const Position& pos);
  virtual void changeDate(int date);

private:
  virtual void evStechecPkt(const StechecPkt& pkt);
};


inline void OutAnEvent::evStechecPkt(const StechecPkt& pkt)
{
  switch (pkt.type)
    {
    case MOVE_GOODMAN:
      goodmanMove(pkt.client_id, pkt.arg[0], Position(pkt.arg[1], pkt.arg[2]));
      break;

    case GOODMAN_CHANGE_STATE:
      goodmanChangeState(pkt.client_id, pkt.arg[0], pkt.arg[1]);
      break;

    case GOODMAN_DELOREAN:
      if (pkt.arg[1] == 0)
        goodmanLostDelorean(pkt.client_id, pkt.arg[0]);
      else
        goodmanGetDelorean(pkt.client_id, pkt.arg[0]);
      break;

    case GOODMAN_ALMANACH:
      if (pkt.arg[1] == 0)
        goodmanLostAlmanach(pkt.client_id, pkt.arg[0]);
      else
        goodmanGetAlmanach(pkt.client_id, pkt.arg[0]);
      break;
      
    case DELOREAN_MOVE:
      deloreanMove(Point(pkt.arg[1], pkt.arg[0]));
      break;

    case ALMANACH_MOVE:
      almanachMove(Point(pkt.arg[1], pkt.arg[0]));
      break;
  
    case CHANGE_DATE:
      changeDate(pkt.arg[0]);
      break;
    }
}

inline void OutAnEvent::goodmanMove(int, int, const Position&) {}
inline void OutAnEvent::goodmanChangeState(int, int, int) {}
inline void OutAnEvent::goodmanLostDelorean(int, int) {}
inline void OutAnEvent::goodmanGetDelorean(int, int) {}
inline void OutAnEvent::goodmanLostAlmanach(int, int) {}
inline void OutAnEvent::goodmanGetAlmanach(int, int) {}
inline void OutAnEvent::deloreanMove(const Position&) {}
inline void OutAnEvent::almanachMove(const Position&) {}
inline void OutAnEvent::changeDate(int) {}

#endif /* !EVENT_HH_ */
