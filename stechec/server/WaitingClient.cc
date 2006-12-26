/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#include "datatfs/tcp.hh"
#include "Server.hh"
#include "WaitingClient.hh"

BEGIN_NS(server);

/*
** WaitingClient
*/

WaitingClient::WaitingClient(Cx* cx, Server* srv)
  : Client(cx),
    srv_(srv)
{
}

WaitingClient::~WaitingClient()
{
}

bool WaitingClient::recvReady()
{
  Packet* pkt;

  try {
    pkt = cx_->receive();

    if (pkt->token == CX_QUERYLIST)
      {
	srv_->serveGameList(cx_);
	// This is the only case we keep it in the list of client.
	return false;
      }
    else if (pkt->token == CX_JOIN)
      {
	srv_->serveJoinGame(cx_, pkt);
      }
    else if (pkt->token == CX_ABORT)
      {
	LOG4("Client %1 sent CX_ABORT. Close its connection.", *cx_);
	delete cx_;
      }
    else
      {
	LOG2("Unknown message (%1) from waiting client '%2'. Kill it.", pkt->token, *cx_);
	delete cx_; // "/me takes aim: HEADSHOT !"
      }
  } catch (const NetError& e) {
    LOG2("Network error on waiting client (from %1): %2", *cx_, e.what());
    delete cx_;
  }

  // No matter what, remove this waiting client from list.
  return true;
}


/*
** TcpCxListener
*/

TcpCxListener::TcpCxListener(TcpCx* cx, Server* srv)
  : Client(cx),
    tcx_(cx),
    srv_(srv)
{
}

TcpCxListener::~TcpCxListener()
{
}

bool TcpCxListener::recvReady()
{
  LOG4("New connection detected, handling it.");
  TcpCx* client_cx = tcx_->accept();
  try {
    Packet* pkt = client_cx->receive();
    const CxInit& init_pkt = static_cast<CxInit&>(*pkt);
    if (srv_->checkServerState(client_cx)
	&& srv_->checkRemoteVersion(client_cx, init_pkt)
	&& srv_->checkRemoteTCP(client_cx, init_pkt))
      srv_->addClient(client_cx);
  } catch (const NetError& e) {
    LOG2("Network error on connection (from %1): %2", *client_cx, e.what());
    delete client_cx;
  }
  return false;
}

END_NS(server);
