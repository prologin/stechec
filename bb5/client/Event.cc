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
    case eInitGame:
      ev_->evInitGame();
      break;
    case eKickOff:
      ev_->evKickOff();
      break;
    case eOurTurn:
      ev_->evNewTurn(true);
      break;
    case eTheirTurn:
      ev_->evNewTurn(false);
      break;
    }
}

template <>
void EventProcess::dispatch(const MsgSync&) const
{
  ev_->evSync();
}

template <>
void EventProcess::dispatch(const MsgIllegal& m) const
{
  ev_->evIllegal(m.was_token);
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
void EventProcess::dispatch(const MsgBallPos& pkt) const
{
  ev_->evBallPos(Point(pkt.col, pkt.row));
}

template <>
void EventProcess::dispatch(const MsgPlayerPos& pkt) const
{
  ev_->evPlayerPos(pkt.client_id, pkt.player_id, Point(pkt.col, pkt.row));
}

template <>
void EventProcess::dispatch(const ActMove& pkt) const
{
  // FIXME: may send all movements to UI, to have a nice animation.
  Point pos;
  pos.y = pkt.moves[pkt.nb_move - 1].row;
  pos.x = pkt.moves[pkt.nb_move - 1].col;
  ev_->evPlayerMove(pkt.client_id, pkt.player_id, pos);
}

template <>
void EventProcess::dispatch(const MsgPlayerKnocked& pkt) const
{
  ev_->evPlayerKnocked(pkt.client_id, pkt.player_id);
}

template <>
void EventProcess::dispatch(const MsgPlayerStatus& pkt) const
{
  ev_->evPlayerStatus(pkt.client_id, (enum eStatus)pkt.player_id);
}

template <>
void EventProcess::dispatch(const MsgPlayerKO& pkt) const
{
  ev_->evPlayerKO(pkt.client_id, pkt.player_id, pkt.dice);
}

template <>
void EventProcess::dispatch(const MsgInitHalf& pkt) const
{
  ev_->evHalf(pkt.cur_half);
}

template <>
void EventProcess::dispatch(const MsgGiveBall&) const
{
  ev_->evGiveBall();
}

template <>
void EventProcess::dispatch(const MsgResult& pkt) const
{
  ev_->evResult(pkt.client_id, pkt.player_id, (enum eRoll)pkt.roll_type,
                  pkt.result, pkt.modifier, pkt.required, pkt.reroll);
}

template <>
void EventProcess::dispatch(const MsgBlockResult& pkt) const
{
  enum eBlockDiceFace results[3];
  for (int i = 0; i < pkt.nb_dice; i++)
    {
      results[i] = (enum eBlockDiceFace) pkt.results[i];
    }
  ev_->evBlockResult(pkt.client_id, pkt.player_id, pkt.opponent_id, pkt.nb_dice,
		     results, pkt.choose_team_id, pkt.reroll);
}

template <>
void EventProcess::dispatch(const MsgFollow&) const
{
  ev_->evFollow();
}

template <>
void EventProcess::dispatch(const ActBlockPush& pkt) const
{
  Position pos(pkt.target_row, pkt.target_col);
  Position choices[3];
  for (int i = 0; i < pkt.nb_choice; i++)
    {
      choices[i] = Position(pkt.choice[i].row, pkt.choice[i].col);
    }
  ev_->evBlockPush(pos, pkt.nb_choice, choices);
}
