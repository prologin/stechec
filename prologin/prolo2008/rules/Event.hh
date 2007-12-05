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

private:
  virtual void evStechecPkt(const StechecPkt& pkt);
};


inline void GlobulusEvent::evStechecPkt(const StechecPkt& pkt)
{
  switch (pkt.type)
    {
      default:	// Nothing.
	break;
    }
}

#endif /* !EVENT_HH_ */
