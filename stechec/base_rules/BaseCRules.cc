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

# include "BaseCRules.hh"

BaseCRules::BaseCRules()
  : team_id_(-1)
{
  HANDLE_WITH(CLIENT_UID, BaseCRules, this, msgCatchUid, GS_WAIT);
  HANDLE_WITH(GAME_FINISHED, BaseCRules, this, msgCatchGameFinished, GS_ALL | GS_END);
}

BaseCRules::~BaseCRules()
{
}

void    BaseCRules::onEvent(int event)
{
  Packet pkt(CUSTOM_EVENT, event);
  onEvent(&pkt);
}

void    BaseCRules::onEvent(const Packet* pkt)
{
  if (pkt_hdl_[pkt->token].empty())
    {
      WARN("Paquet `" << pkt->token << "' is not registered.");
      return;
    }

  // Send this event to the dispatcher, if there is one (eg: stechec champions
  // do not have one).
  if (evp_.hasEventHandler())
    pkt_hdl_[pkt->token].begin()->second->dispatchUIEvent(&evp_, *pkt);
}

void    BaseCRules::msgCatchUid(const ClientUid* m)
{
  LOG3("[" << m->client_id << "] <= Look! This is my new shining uid!");
  team_id_ = m->client_id;
  setTeamNumber(m->nb_team);
  assert(packet_sender_ != NULL);
  packet_sender_->setTeamId(team_id_);
}

void    BaseCRules::msgCatchGameFinished(const GameFinished*)
{
  setState(GS_END);
  packet_sender_->gameFinished();
}
