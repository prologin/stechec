/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2007 Prologin
*/

#ifndef EVENT_HH_
# define EVENT_HH_

# include "StechecEvent.hh"

class GlobulusEvent : public Event
{
protected:
  virtual void endOfTurn() = 0;

  // Custom events
  virtual void moveLeucocyte(int team_id, int unit_id, const Position& to);
  virtual void newLeucocyte(int team_id, int unit_id, const Position& at);
  virtual void moveVirus(int virus_id, const Position& to);
  virtual void newVirus(int virus_id, const Position& at, int type);
  virtual void dieVirus(int virus_id);
  virtual void newBacteria(int bacteria_id, const Position& at);
  virtual void dieBacteria(int bacteria_id);
  virtual void newCell(int cell_id, const Position& at);
  virtual void caseUpdate(int x, int y, int food, int antibody);
  virtual void cellUpdate(int cell_id, int type);

private:
  virtual void evStechecPkt(const StechecPkt& pkt);
};


inline void GlobulusEvent::evStechecPkt(const StechecPkt& pkt)
{
  return; // FIXME: deactivated for the moment

  switch (pkt.type)
    {
      case MOVE_LEUCO:
	moveLeucocyte(pkt.client_id, pkt.arg[0], Position(pkt.arg[1], pkt.arg[2]));
	break;
      case NEW_LEUCO:
	moveLeucocyte(pkt.arg[0], pkt.arg[1], Position(pkt.arg[3], pkt.arg[2]));
	break;
      case NEW_VIRUS:
	newVirus(0 /* FIXME */, Position(pkt.arg[0], pkt.arg[1]), pkt.arg[2]);
	break;
      case NEW_CELL:
	newCell(0 /* FIXME */, Position(pkt.arg[0], pkt.arg[1]));
	break;
      default:	// Nothing.
	break;
    }
}

inline void GlobulusEvent::moveLeucocyte(int team_id, int unit_id, const Position& to) {}
inline void GlobulusEvent::newLeucocyte(int team_id, int unit_id, const Position& at) {}
inline void GlobulusEvent::moveVirus(int virus_id, const Position& to) {}
inline void GlobulusEvent::newVirus(int virus_id, const Position& at, int type) {}
inline void GlobulusEvent::dieVirus(int virus_id) {}
inline void GlobulusEvent::newBacteria(int bacteria_id, const Position& at) {}
inline void GlobulusEvent::dieBacteria(int bacteria_id) {}
inline void GlobulusEvent::newCell(int cell_id, const Position& at) {}
inline void GlobulusEvent::caseUpdate(int x, int y, int food, int antibody) {}
inline void GlobulusEvent::cellUpdate(int cell_id, int type) {}

#endif /* !EVENT_HH_ */
