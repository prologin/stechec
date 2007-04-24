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

inline int      BaseCRules::getCoachId() const
{
  return coach_id_;
}

inline int      BaseCRules::getLeagueId() const
{
  return league_id_;
}

inline bool	BaseCRules::isBusy() const
{
  return busy_count_ > 0;
}

inline void	BaseCRules::sendPacket(const Packet& p) const
{
  assert(packet_sender_ != NULL);
  packet_sender_->sendPacket(p);
  busy_count_++;
}

inline void     BaseCRules::setEventHandler(Event* evp)
{
  evp_.setEventHandler(evp);
}
