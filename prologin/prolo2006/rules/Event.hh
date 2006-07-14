/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
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
  virtual void moveGoodman(int team_id, int unit_id, const Position& pos);
  virtual void changeDate(int date);

private:
  virtual void evStechecPkt(const StechecPkt& pkt);
};


inline void OutAnEvent::evStechecPkt(const StechecPkt& pkt)
{
  switch (pkt.type)
    {
    case MOVE_GOODMAN:
      moveGoodman(pkt.client_id, pkt.arg[0], Position(pkt.arg[1], pkt.arg[2]));
      break;

    case CHANGE_DATE:
      changeDate(pkt.arg[0]); // FIXME: don't know format yet.
      break;
    }
}

inline void OutAnEvent::moveGoodman(int, int, const Position&) {}
inline void OutAnEvent::changeDate(int) {}

#endif /* !EVENT_HH_ */
