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

#ifndef DIRECT_HH_
# define DIRECT_HH_

# include "cx.hh"

/*!
** @ingroup tools_net
** @{
*/

enum eCxWhoIAm {
  CXD_IS_SERVER,
  CXD_IS_COACH1,
  CXD_IS_COACH2
};

class DirectCx : public Cx
{
public:
  DirectCx(eCxWhoIAm w);
  virtual ~DirectCx();

  virtual void send(const Packet* pkt);
  virtual Packet* receive();
  virtual bool poll(int timeout = 0);
  
private:
  void lock(bool lock_server);
  void unlock(bool unlock_server);

  typedef std::queue<Packet*> PacketQueue;
  PacketQueue queue_;
  eCxWhoIAm w_;
  
  static DirectCx*        inst_client_[2];
  static DirectCx*        inst_server_;
};

// !ingroup tools_net
//! @}

#endif /* !DIRECT_HH_ */
