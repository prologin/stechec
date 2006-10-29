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

#ifndef WAITINGCLIENT_HH_
# define WAITINGCLIENT_HH_

# include "client.hh"
class TcpCx;

BEGIN_NS(server);

class Server;

/*!
** @brief Represent a client waiting to enter a game.
*/
class WaitingClient : public Client
{
public:
  WaitingClient(Cx* cx, Server* srv);
  virtual ~WaitingClient();

  virtual bool recvReady();

protected:
  Server* srv_;
};


/*!
** @brief Listener socket in TCP.
*/
class TcpCxListener : public Client
{
public:
  TcpCxListener(TcpCx* cx, Server* srv);
  virtual ~TcpCxListener();
  
  virtual bool recvReady();

private:
  TcpCx* tcx_;
  Server* srv_;
};


END_NS(server);

#endif /* !WAITINGCLIENT_HH_ */
