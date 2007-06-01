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

# include "BaseCRules.hh"

BaseCRules::BaseCRules()
  : coach_id_(-1),
    league_id_(-1),
    busy_count_(0)
{
  HANDLE_WITH(MSG_SYNC, BaseCRules, this, msgCatchSync, 0);
  HANDLE_WITH(CLIENT_UID, BaseCRules, this, msgCatchUid, GS_WAIT);
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
      WARN("Paquet `%1` is not registered.", pkt->token);
      return;
    }

  // Send this event to the dispatcher, if there is one (eg: stechec champions
  // do not have one).
  if (evp_.hasEventHandler())
    pkt_hdl_[pkt->token].begin()->second->dispatchUIEvent(&evp_, *pkt);
}

void	BaseCRules::msgCatchSync(const MsgSync* m)
{
  if (m->client_id == coach_id_ && --busy_count_ < 0)
    {
      WARN("busy count goes under 0");
      busy_count_ = 0;
    }
}


void    BaseCRules::msgCatchUid(const ClientUid* m)
{
  LOG3("[ %1:%2 ] <= Look! This is my new shining uid!",
       m->client_id, m->league_id);
  coach_id_ = m->client_id;
  league_id_ = m->league_id;
  setTeamNumber(m->nb_coach, m->nb_team);

  assert(packet_sender_ != NULL);
  packet_sender_->setTeamId(m->client_id);
}
