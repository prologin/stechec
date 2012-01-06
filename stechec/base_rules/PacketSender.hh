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
** `COPYING' file in the root directory.
**
** The TBT Team consists of people listed in the `AUTHORS' file.
*/

#ifndef PACKETSENDER_HH_
# define PACKETSENDER_HH_

# include "tools.hh"

/*!
** @brief Must be derived by somebody already owning BaseRules,
**    in order to really send packet.
**
** @author victor
** @date 04/03/06
*/
class PacketSender
{
public:
  PacketSender();
  virtual ~PacketSender();

  void setTeamId(int team_id);

  virtual void sendPacket(const Packet& p) = 0;

  virtual void gameFinished();
  
protected:
  int team_id_;
};

inline PacketSender::PacketSender()
  : team_id_(-1)
{
}

inline PacketSender::~PacketSender()
{
}

inline void PacketSender::setTeamId(int team_id)
{
  team_id_ = team_id;
}

inline void PacketSender::gameFinished()
{
}

#endif /* !PACKETSENDER_HH_ */
