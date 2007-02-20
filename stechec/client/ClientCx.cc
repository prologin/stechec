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

#include "datatfs/file.hh"
#include "datatfs/direct.hh"
#include "datatfs/tcp.hh"
#include "ClientCx.hh"

ClientCx::ClientCx()
  : cx_(NULL),
    rules_(NULL),
    client_gid_(-1)
{
}

ClientCx::~ClientCx()
{
  delete cx_;
}

bool    ClientCx::connect(xml::XMLConfig& cfg)
{
  return connect(cfg.getAttr<std::string>("client", "connect", "host"),
		 cfg.getAttr<int>("client", "connect", "port"));
}

bool	ClientCx::join(xml::XMLConfig& cfg, const struct RuleDescription& desc)
{
  bool mode = cfg.getAttr<bool>("client", "mode", "spectator");
  int game_uid = cfg.getAttr<int>("client", "connect", "game_uid");
  return join(game_uid, mode, desc);
}

bool    ClientCx::openLog(const std::string& filename)
{
  FileCx* cx = new FileCx;
  try {
    cx->open(filename, CX_RO);
  } catch (const FileIOError& e) {
    ERR("%1",e);
    return false;
  }
  cx_ = cx;
  return true;
}


void    ClientCx::disconnect(bool send_abort)
{
  if (cx_ != NULL)
    {
      if (send_abort)
	{
	  LOG5("Send CX_ABORT to the server.");
	  Packet pkt(CX_ABORT, team_id_);
	  cx_->send(&pkt);
	}

      delete cx_;
      cx_ = NULL;
    }
}

bool    ClientCx::isConnected() const
{
  return cx_ != NULL;
}

bool    ClientCx::process(bool block)
{
  if (cx_ != NULL && cx_->poll(block ? 50 : 0))
    {
      Packet* p = cx_->receive();
      rules_->handlePacket(p);
      return true;
    }
  return false;
}

void	ClientCx::setRules(BaseCRules* rules)
{
  // Rules will go through us to send packets.
  rules_ = rules;
  rules_->setSendPacketObject(this);
}

void	ClientCx::setClientGameId(int client_gid)
{
  client_gid_ = client_gid;
}

void    ClientCx::setReady()
{
  if (cx_ == NULL)
    {
      WARN("You are not connected! connect first.");
      return;
    }
  LOG5("Send `CX_READY' (from client_id: %1)", team_id_);
  Packet pkt_ready(CX_READY, team_id_);
  cx_->send(&pkt_ready);
}




void ClientCx::sendPacket(const Packet& p)
{
  const_cast<Packet&>(p).client_id = team_id_;

  // FIXME: if spectator, filter all message except MSG_CHAT, since
  // they are not allowed by the server... see if we can do better.
  if (p.client_id >= UID_VIEWER_BASE)
    return;

  LOG5("Send packet `%1` (to client_id %2)", rules_->stringifyToken(p.token), p.client_id);
  assert(p.client_id >= UID_COACH_BASE);
  cx_->send(&p);
}

void ClientCx::gameFinished()
{
  LOG4("Game is finished. Closing connection.");
  delete cx_;
  cx_ = NULL;
}

// Connect to the server, in network mode.
bool ClientCx::connect(const std::string& host, int port)
{
  int nb_retry = 6;

  if (cx_ != NULL)
    {
      WARN("You are already connected! Disconnect first.");
      return false;
    }

  LOG3("Connecting to %1:%2", host, port);
  TcpCx* cx = new TcpCx;
  while (nb_retry--)
    try {
      cx->connect(host.c_str(), port);
      LOG2("Connected to %1", *cx);
      break;
    } catch (const NetError&) {
      LOG2("Error - Retrying in 3 seconds.");
      if (nb_retry > 0)
        sleep(3);
    }
  if (!nb_retry)
    {
      ERR("Connection failed.");
      delete cx;
      return false;
    }

  // Send init packet. (the first one !)
  CxInit pkt_init;
  pkt_init.binary_version = STECHEC_BINARY_VERSION;
  cx->send(&pkt_init);

  // Receive server answer.
  Packet* p = cx->receive();
  if (p->token == CX_DENY)
    {
      const CxDeny& pkt_deny = static_cast<CxDeny&>(*p);
      ERR("Server denied the connection. Reason below:");
      ERR(" - %1", packetToString(pkt_deny.reason));
      delete cx;
      return false;
    }
  if (p->token != CX_ACCEPT)
    {
      ERR("Not CX_ACCEPT nor CX_DENY. Must be a broken server. Get out of there!");
      delete cx;
      return false;
    }
  cx_ = cx;
  return true;
}

bool ClientCx::join(int game_uid, bool wanna_be_viewer, const struct RuleDescription& desc)
{
  LOG4("Join game `%1` as `%2`.", game_uid, (wanna_be_viewer ? "viewer" : "coach"));
  CxJoin pkt_join(CX_JOIN);
  stringToPacket(pkt_join.rules_name, desc.name, 32);
  pkt_join.rules_major = desc.major;
  pkt_join.rules_minor = desc.minor;
  pkt_join.is_coach = wanna_be_viewer ? 0 : 1;
  pkt_join.game_uid = game_uid;
  pkt_join.client_extid = client_gid_;
  cx_->send(&pkt_join);
  Packet* p = cx_->receive();
  if (p->token == CX_DENY)
    {
      const CxDeny& pkt_deny = static_cast<CxDeny&>(*p);
      ERR("Server denied the join. Reason below:");
      ERR(" - %1", packetToString(pkt_deny.reason));
      return false;
    }
  
  
  LOG3("Connection sequence completed.");
  return p->token == CX_ACCEPT;
}

// Connect to the arbiter, in standalone mode
void ClientCx::createDirectCx(int uid)
{
  LOG4("[%1] Create DirectCx", uid);
  cx_ = new DirectCx(uid == 1 ? CXD_IS_COACH1 : CXD_IS_COACH2);
}

bool ClientCx::syncArbiter()
{
  Packet hello(CX_INIT, 1);
  cx_->send(&hello);
  if (!cx_->poll(1000))
    return false;
  Packet* p = cx_->receive();
  LOG4("Sync ok.");
  return p->token == CX_ACCEPT;
}


//
// FIXME: exports all client_cx functions in this way, so C UI can do
//   everything with the connection.
//
extern "C" int client_cx_process(void* ptr)
{
  ClientCx* client_cx = reinterpret_cast<ClientCx*>(ptr);
  if (client_cx->process(true))
    return 1;
  return 0;
}
