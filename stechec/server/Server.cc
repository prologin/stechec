/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006, 2007 Prologin
*/

#include <signal.h>

#include "tools.hh"
#include "datatfs/TcpCx.hh"
#include "Client.hh"
#include "Server.hh"

BEGIN_NS(server);


Server* Server::inst = NULL;

Server::Server(const xml::XMLConfig& cfg)
  : cfg_(cfg),
    cl_pool_(1500),
    server_shutdown_(0),
    server_shutdown_reset_(2)
{
  // Singleton.
  assert(inst == NULL);
  inst = this;
  pthread_mutex_init(&lock_, NULL);

  // Catch ctl-c, to properly shutdown the server.
  struct sigaction act;
  int netbsdsuck;
  netbsdsuck = sigemptyset(&act.sa_mask);
  act.sa_handler = &Server::wantShutdown;
  act.sa_flags = 0;
  sigaction(SIGINT, &act, NULL);

}

Server::~Server()
{
  inst = NULL;
  pthread_mutex_destroy(&lock_);
}


// Ctl-C was pressed, shutdown it after all games were finished.
void Server::wantShutdown(int)
{
  if (inst->server_shutdown_ == 0 || inst->server_shutdown_reset_.isTimeElapsed())
    {
      LOG1("Shutdown requested. Will stop when everybody will be disconnected.");
      inst->server_shutdown_ = 1;
      inst->server_shutdown_reset_.restart();
    }
  else if (inst->server_shutdown_ == 1)
    {
      LOG1("Ok, next press on Ctl-C will kill the server");
      inst->server_shutdown_ = 2;
    }
  else
    {
      WARN("Seems you really want to exit... Killing everything.");
      exit(1);
    }
}



bool Server::checkServerState(Cx* cx)
{
  if (server_shutdown_)
    {
      LOG2("Connection from %1 has been rejected. Server is shutdowning.", *cx);
      CxDeny denial;
      stringToPacket(denial.reason, "Server is shutdowning", 64);
      cx->send(denial);
      return false;
    }
  return true;
}

// Check if host is allowed on our system (ip in blacklist, etc...).
// If it's bad, the connection is closed right now.
bool Server::checkRemoteTCP(TcpCx* cx)
{
  bool  bad = false;

  // FIXME: do that, check for bad guys.

  if (bad)
    {
      LOG3("Connection from %1 has been rejected.", *cx);
      CxDeny denial;
      stringToPacket(denial.reason, "I don't like you.", 64);
      cx->send(denial);
      return false;
    }

  LOG4("Connection from '%1` has been accepted.", *cx);
  Packet acpt(CX_ACCEPT);
  cx->send(acpt);
  return true;
}

bool Server::checkRemoteVersion(Cx* cx, const CxInit& pkt)
{
  if (pkt.binary_version != STECHEC_BINARY_VERSION)
    {
      std::ostringstream os;
      os << "Binary version mismatch: server is `" << STECHEC_BINARY_VERSION;
      os << "', client is `" << pkt.binary_version << "'";
      LOG3("Connection from %1 has been rejected, reason:", *cx);
      LOG3(" - %1", os.str());
      CxDeny pkt_deny;
      stringToPacket(pkt_deny.reason, os.str(), 64);
      cx->send(pkt_deny);
      return false;
    }
  return true;
}



// Send to the client the list of active games.
bool Server::serveGameList(Client* cl, Packet*)
{
  if (!cl->init_pkt_received_)
    {
      LOG3("%1: init packet not received", *cl->cx_);
      return false;
    }

  // FIXME: send him the list of games currently hosted
  LOG5("A client is requesting the list of games.");
  Packet pkt_list(CX_LIST);
  cl->send(&pkt_list);
  return true;
}

bool Server::serveJoinGame(Client* cl, Packet* pkt)
{
  CxJoin* pkt_join = static_cast<CxJoin*>(pkt);
  std::string client_rules_name = packetToString(pkt_join->rules_name);

  if (!cl->init_pkt_received_)
    {
      LOG3("%1: init packet not received", *cl->cx_);
      return false;
    }

  // Find corresponding rules, else load it
  Rules *r;
  RulesList::iterator rit = rules_.find(client_rules_name);
  if (rit == rules_.end())
    {
      try {
        r = new Rules;
        r->lib_.open(packetToString(pkt_join->server_lib));
        r->desc_ = (const struct RuleDescription*)
          (r->lib_.getSymbol("rules_description"));
        r->create_fun_ = (Rules::create_rules_t)
          (r->lib_.getSymbol("load_server_rules"));
      } catch (const LibraryError& e) {
        LOG2("Can't load library: %1", e.what());
        return false;
      }
      rules_.insert(std::make_pair(client_rules_name, r));
    }
  else
    r = rit->second;

  // Check client rules version against server rules version
  if (pkt_join->rules_major != r->desc_->major ||
      pkt_join->rules_minor != r->desc_->minor)
    {
      std::ostringstream os;
      os << "Version mismatch: server's is `" << r->desc_->major << "."
	 << r->desc_->minor << "'"
	 << ", client is `" << pkt_join->rules_major << "."
         << pkt_join->rules_minor << "'";
      LOG3("Joining game %1 from %2 has been rejected, reason:", *cl->cx_);
      LOG3(" - %1", os.str());
      CxDeny pkt_deny;
      stringToPacket(pkt_deny.reason, os.str(), 64);
      cl->send(&pkt_deny);
      return false;
    }

  
  GameIter it = games_.find(pkt_join->game_uid);
  if (it == games_.end())
    {
      // Check if we can create a game
      if (!is_persistent_ && !games_.empty())
	{
	  LOG3("Cannot create a new game. Server is not persistent.");
	  return false;
	}

      // This is a new game. Create and start it
      GameHosting* gh = new GameHosting(pkt_join->game_uid, cfg_,
                                        r->create_fun_(&cfg_));
      it = games_.insert(std::make_pair(pkt_join->game_uid, gh)).first;
      pthread_t th;
      pthread_create(&th, NULL, &GameHosting::startThread, it->second);
      LOG5("Thread started.");
    }

  // Add this client to the {newly created|already existing} game
  cl->game_joined_ = true;
  cl->league_id_ = pkt_join->client_extid;
  cl->is_coach_ = pkt_join->is_coach ? true : false;
  cl_pool_.removeElt(cl->cx_);
  cl_.erase(cl->cx_);
  it->second->addClient(cl);
  return true;
}


bool Server::serveInitPacket(Client* cl, Packet* pkt)
{
  const CxInit& init_pkt = static_cast<CxInit&>(*pkt);

  if (cl->init_pkt_received_)
    {
      LOG3("%1: init packet already received", *cl->cx_);
      return false;
    }

  if (checkRemoteVersion(cl->cx_, init_pkt))
    {
      cl->init_pkt_received_ = true;
      return true;
    }
  return false;
}

void Server::receivePacket(Cx* cx)
{
  ClientList::iterator it;
  Client* cl;
  Packet* pkt;
  bool ret;

  if ((it = cl_.find(cx)) == cl_.end())
    {
      // BUG: each connection should map a cl_ entry.
      ERR("BUG: cl_ out of sync");
      cl_pool_.removeElt(cx);
      delete cx;
      return;
    }
  cl = it->second;

  // Dispatch packet.
  try {
    pkt = cx->receive();
    switch (pkt->token)
      {
      case CX_INIT:
        ret = serveInitPacket(cl, pkt);
        break;

      case CX_JOIN:
        ret = serveJoinGame(cl, pkt);
        break;

      case CX_QUERYLIST:
        ret = serveGameList(cl, pkt);
        break;

      case CX_ABORT:
	LOG4("Client %1 sent CX_ABORT, close its connection", *cx);
	ret = false;
        break;

      default:
	LOG2("Unknown message (%1) from waiting client '%2': kill it",
             pkt->token, *cx);
	ret = false;
        break;
      }

  } catch (const NetError& e) {
    LOG2("Connection error on waiting client (from %1): %2", *cx, e.what());
    ret = false;
  }

  if (!ret)
    {
      cl_pool_.removeElt(cx);
      cl_.erase(cx);
      delete cl;
    }
}

void Server::serveNewConnection(TcpCx* cxl)
{
  TcpCx* cx;

  LOG4("New connection detected, handling it.");
  cx = cxl->accept();

  if (checkServerState(cx) &&
      checkRemoteTCP(cx))
    {
      cl_pool_.addElt(cx);
      cl_.insert(std::make_pair(cx, new Client(cx)));
    }
}

// Check for games that are finished, to join thread and
// free memory.
// Return true if a game was cleaned.
bool    Server::cleanFinishedGame()
{
  bool  ret_value = false;

  for (GameIter it = games_.begin(); it != games_.end(); )
    {
      enum eGameState state = it->second->getState();
      if (state == eFinished || state == eCrashed)
        {
          LOG5("Clean a thread, used to belong to game uid %1.", it->first);
          pthread_join(it->second->getThreadId(), NULL);
          delete it->second;
          games_.erase(it++);
          LOG6("Thread cleaned.");
          ret_value = true;
        }
      else
        ++it;
    }
  return ret_value;
}

// Handle listening socket, and all clients that hasn't join a
// game yet.
void        Server::run()
{
  TcpCx		listen_socket;
  Timer		wait_timeout(0);
  int           port;

  is_persistent_ = cfg_.getAttr<bool>("server", "options", "persistent");
  if (!is_persistent_)
    wait_timeout.setAllowedTime(cfg_.getAttr<int>("server", "options", "wait_timeout"));
  if (wait_timeout.getAllowedTime() > 0)
    {
      LOG3("Will wait for `%1' second that a game begin before exiting",
	   wait_timeout.getAllowedTime());
      wait_timeout.start();
    }

  port = cfg_.getAttr<int>("server", "listen", "port");
  LOG2("Listening on port %1", port);
  listen_socket.listen(port);

  cl_pool_.addElt(&listen_socket);

  while (!server_shutdown_ || !games_.empty() || cl_pool_.size() > 1)
    {
      CxPool<Cx>::ConstEltIter it;
      const CxPool<Cx>::EltList& ready_list(cl_pool_.poll());

      for (it = ready_list.begin(); it != ready_list.end(); ++it)
        {
          Cx* cx = it->second;
          Client* cl;

          switch (it->first)
            {
            case E_FD_CONNECTION_PENDING:
              serveNewConnection(&listen_socket);
              break;

            case E_FD_CONNECTION_CLOSED:
              cl_pool_.removeElt(cx);
              cl = cl_[cx];
              cl_.erase(cx);
              delete cl;
              break;

            case E_FD_READ_READY:
              receivePacket(cx);
              break;
            }
        }

      if (cleanFinishedGame() && !is_persistent_)
        break;
      if (wait_timeout.isTimeElapsed() && games_.empty())
	break;
    }
  LOG1("Server has finished its work. Exiting.");
}

END_NS(server);
