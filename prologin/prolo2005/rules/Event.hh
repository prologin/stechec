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

class ZavieEvent : public Event
{
protected:
  virtual void newUnit(int player_id, int unit_id, const Position& pos);
  virtual void moveColeoptera(int player_id, int unit_id, const Position& pos);
  virtual void buildingFinished(int player_id, const Position& pos);

private:
  virtual void evStechecPkt(const StechecPkt& pkt);

  virtual void endOfTurn() {}
};


inline void ZavieEvent::evStechecPkt(const StechecPkt& pkt)
{
  switch (pkt.type)
    {
    case NEW_UNIT:
      newUnit(pkt.client_id, pkt.arg[0], Position(pkt.arg[1], pkt.arg[2]));
      break;

    case MOVE_COLEOPTERA:
      moveColeoptera(pkt.client_id, pkt.arg[0], Position(pkt.arg[1], pkt.arg[2]));
      break;

    case BUILDING_FINISHED:
      buildingFinished(pkt.client_id, Position(pkt.arg[0], pkt.arg[1]));
      break;
    }
}

inline void ZavieEvent::newUnit(int, int, const Position&) {}
inline void ZavieEvent::moveColeoptera(int, int, const Position&) {}
inline void ZavieEvent::buildingFinished(int, const Position&) {}


#endif /* !EVENT_HH_ */
