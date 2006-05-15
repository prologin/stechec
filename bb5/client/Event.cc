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

#include "Player.hh"
#include "Constants.hh"
#include "CRules.hh"
#include "Event.hh"

template <>
void EventProcess::dispatch(const CustomEvent& ie) const
{
  switch (ie.client_id)
    {
    case eKickOff:
      ev_->evKickOff();
      break;
    }
}

template <>
void EventProcess::dispatch(const MsgIllegal&) const
{
  ev_->evIllegal();
}

template <>
void EventProcess::dispatch(const MsgNewTurn&) const
{
  ev_->evNewTurn();
}

template <>
void EventProcess::dispatch(const MsgEndGame&) const
{
  ev_->evEndGame();
}

template <>
void EventProcess::dispatch(const ActMoveTurnMarker&) const
{
  ev_->evMoveTurnMarker();
}

template <>
void EventProcess::dispatch(const MsgTimeExceeded&) const
{
  ev_->evTimeExceeded();
}

template <>
void EventProcess::dispatch(const MsgChat& pkt) const
{
  ev_->evChat(packetToString(pkt.msg));
}

template <>
void EventProcess::dispatch(const ActMove& pkt) const
{
  // FIXME: may send all movements to UI, to have a nice animation.
  Point pos;
  pos.y = pkt.moves[pkt.nb_move - 1].row;
  pos.x = pkt.moves[pkt.nb_move - 1].col;
  ev_->evPlayerMove(pkt.client_id, pkt.player_id - 1, pos);
}

template <>
void EventProcess::dispatch(const MsgPlayerKnocked& pkt) const
{
  ev_->evPlayerKnocked(pkt.client_id, pkt.player_id);
}
