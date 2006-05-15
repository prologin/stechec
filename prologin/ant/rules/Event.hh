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

class AntEvent : public Event
{
protected:
  virtual void positionAnthill(int, int, int) {}
  virtual void mapContent(const Position&, int) {}
  virtual void newUnit(int) {}
  virtual void antMove(int, int, const Position&) {}
  virtual void antGetFood(int, int) {}
  virtual void antLetFood(int, int) {}

private:
  virtual void evStechecPkt(const StechecPkt& pkt);

  virtual void endOfTurn() {}
};

inline void AntEvent::evStechecPkt(const StechecPkt& pkt)
{
  switch (pkt.type)
    {
    case POSITION_ANTHILL:
      positionAnthill(pkt.arg[0], pkt.arg[1], pkt.arg[2]);
      break;

    case MAP_CONTENT:
      mapContent(Position(pkt.arg[0], pkt.arg[1]), pkt.arg[2]);
      break;
      
    case ANT_NEW_UNIT:
      newUnit(pkt.client_id);
      break;
      
    case ANT_MOVE:
      antMove(pkt.client_id, pkt.arg[0],
              Position(pkt.arg[1], pkt.arg[2]));
      break;
      
    case ANT_GET_FOOD:
      antGetFood(pkt.client_id, pkt.arg[0]);
      break;
      
    case ANT_LET_FOOD:
      antLetFood(pkt.client_id, pkt.arg[0]);
      break;
    }
}

#endif /* !EVENT_HH_ */

