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
** `COPYING' file in the root directory.
** 
** The TBT Team consists of people listed in the `AUTHORS' file.
*/

#include "datatfs/Cx.hh"
#include "BaseRules.hh"

BaseRules::BaseRules(int team_number)
  : state_(GS_WAIT),
    team_number_(team_number),
    packet_sender_(NULL)
{
}

BaseRules::~BaseRules()
{
  for (unsigned i = 0; i < MAX_TOKEN; i++)
    {
      PktHList::iterator it;
      for (it = pkt_hdl_[i].begin(); it != pkt_hdl_[i].end(); ++it)
        delete it->second;
    }
}

void    BaseRules::sendPacket(const Packet& p) const
{
  assert(packet_sender_ != NULL);
  packet_sender_->sendPacket(p);
}

void    BaseRules::handleWith(BasePacketHandler* bph, int when)
{
  pkt_hdl_[bph->getCstValue()].push_back(std::make_pair(when, bph));
}

int     BaseRules::getTeamNumber() const
{
  return team_number_;
}

void    BaseRules::handlePacket(const Packet* p)
{
  if (p->token >= MAX_TOKEN)
    {
      WARN("handlePacket: token not in allowed range: `%1'", p->token);
      return;
    }

  if (pkt_hdl_[p->token].empty())
    {
      // This token has no associated object. This is an error.
      WARN("handlePacket: unknown packet token: `%1'",
	   stringifyToken(p->token));
    }
  else
    {
      bool handled = false;
      PktHList::iterator it;
      for (it = pkt_hdl_[p->token].begin(); it != pkt_hdl_[p->token].end(); ++it)
        if (it->first == 0 || it->first == state_)
          {
            it->second->handle(p);
	    handled = true;
          }
      if (!handled)
	WARN("handlePacket: msg `%1' doesn't have handler for state `%2'",
	     stringifyToken(p->token), state_);
    }
}

int        BaseRules::getState() const
{
  return state_;
}

void        BaseRules::setState(int new_state)
{
  state_ = new_state;
}

const char* BaseRules::stringifyToken(unsigned token) const
{
  static const char *cx_token_str[] = {
    "CX_INIT",
    "CX_ACCEPT",
    "CX_DENY",
    "CX_QUERYLIST",
    "CX_LIST",
    "CX_JOIN",
    "CX_READY",
    "CX_ABORT",
  };

  static const char *base_token_str[] = {
    "MSG_SYNC",
    "CUSTOM_EVENT",
    "CLIENT_UID",
    "GAME_FINISHED",
  };

  if (token >= CX_TOKEN_START && token < CX_TOKEN_LAST)
    return cx_token_str[token - CX_TOKEN_START];
  if (token >= BASE_TOKEN_START && token < BASE_TOKEN_LAST)
    return base_token_str[token - BASE_TOKEN_START];

  // Ask upper level for stringified token
  return tokenToString(token);
}

const char* BaseRules::tokenToString(unsigned) const
{
  return "n/a";
}

void        BaseRules::serialize(std::ostream& os) const
{
  os << state_;
}

void        BaseRules::unserialize(std::istream& is)
{
  is >> state_;
}

void        BaseRules::setSendPacketObject(PacketSender* ps)
{
  packet_sender_ = ps;
}
