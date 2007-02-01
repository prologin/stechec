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

BaseRules::BaseRules()
  : state_(GS_WAIT),
    team_number_(0),
    sync_(false),
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

void    BaseRules::setTeamNumber(int value)
{
  team_number_ = value;
}

void    BaseRules::handlePacket(const Packet* p)
{
  assert(p->token < MAX_TOKEN);
  if (pkt_hdl_[p->token].empty())
    {
      // This token has no associated object. This is an error.
      WARN("handlePacket: unknown packet token = %1", p->token);
    }
  else
    {
      bool handled = false;
      PktHList::iterator it;
      for (it = pkt_hdl_[p->token].begin(); it != pkt_hdl_[p->token].end(); ++it)
        if (it->first & state_)
          {
            it->second->handle(p);
	    handled = true;
          }
      if (!handled)
	WARN("handlePacket: msg '%1` doesn't have handler for state %2", pkt_hdl_[p->token].begin()->second->getCstStr(), state_);
    }
  if (sync_)
    sendPacket(MsgSync());
}

int        BaseRules::getState() const
{
  return state_;
}

void        BaseRules::setState(int new_state)
{
  state_ = new_state;
}

std::string BaseRules::getPacketStr(int token) const
{
  if (pkt_hdl_[token].empty())
    {
      std::ostringstream os;
      os << "`" << token << "'";
      return os.rdbuf()->str();
    }
  return std::string(pkt_hdl_[token].begin()->second->getCstStr());
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

void	    BaseRules::setSync(bool enable)
{
  sync_ = enable;
}
