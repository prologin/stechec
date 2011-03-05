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

#include "BaseRules.hh"
#include "EventProcess.hh"

EventProcess::EventProcess()
  : ev_(NULL)
{
}

void EventProcess::setEventHandler(Event* ev)
{
  ev_ = ev;
}

bool EventProcess::hasEventHandler() const
{
  return ev_ != NULL;
}


// create symbols for some message that should exists in order
// to link the binary with all symbols, but not used
// by the UI.
template <>
void EventProcess::dispatch(const MsgSync&) const
{
  assert(false);
}

template <>
void EventProcess::dispatch(const ClientUid&) const
{
  assert(false);
}
