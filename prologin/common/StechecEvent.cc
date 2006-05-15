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

#include "StechecEvent.hh"
#include "CRules.hh"

template <>
void EventProcess::dispatch(const StechecPkt& pkt) const
{
  ev_->evStechecPkt(pkt);
}

template <>
void EventProcess::dispatch(const InternalEventPkt& pkt) const
{
  if (pkt.client_id == 51)
    ev_->endOfTurn();
  else
    assert(false);
}

/*
** These ones could be used... but should not.
*/

template <>
void EventProcess::dispatch(const MsgAfterTurn&) const
{
  assert(false);
}

template <>
void EventProcess::dispatch(const MsgInitGame&) const
{
  assert(false);
}

template <>
void EventProcess::dispatch(const MsgBeforeGame&) const
{
  assert(false);
}

template <>
void EventProcess::dispatch(const MsgChampionError&) const
{
  assert(false);
}

template <>
void EventProcess::dispatch(const MsgAfterGame&) const
{
  assert(false);
}

template <>
void EventProcess::dispatch(const MsgBeforeTurn&) const
{
  assert(false);
}
