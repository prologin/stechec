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

#ifndef STECHECEVENT_HH_
# define STECHECEVENT_HH_

# include "EventProcess.hh"
# include "Rules.hh"

// should be called StechecEvent, but BaseRules require it to be called Event.
class Event
{
public:
  Event();
  virtual ~Event();

  virtual void evStechecPkt(const StechecPkt& pkt) = 0;
  virtual void endOfTurn() = 0;

  
};

inline Event::Event()
{
}

inline Event::~Event()
{
}

#endif /* !STECHECEVENT_HH_ */
