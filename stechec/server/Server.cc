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

#include <signal.h>
#include "WaitingClient.hh"
#include "Server.hh"

BEGIN_NS(server);


Server* Server::inst = NULL;

Server::Server(const xml::XMLConfig& cfg)
  : cfg_(cfg),
    wcl_poll_(waiting_clients_, 500),
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
  
  // Load shared library to get the rules.
  rules_.open(cfg.getData<std::string>("server", "rules"));
  create_rules_fun_ = (create_rules_t)(rules_.getSymbol("load_server_rules"));
  desc_ = (const struct RuleDescription*)(rules_.getSymbol("rules_description"));
}

Server::~Server()
{
  inst = NULL;
  pthread_mutex_destroy(&lock_);
}

void Server::addClient(Cx* client_cx)
{
  waiting_clients_.push_back(new WaitingClient(client_cx, this));
}

// Ctl-C was pressed, shutdown it after all games were finished.
void   Server::wantShutdown(int)
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

bool    Server::checkServerState(Cx* cx)
{
  if (server_shutdown_)
    {
      LOG2("Connection from %1 has been rejected. Server is shutdowning.", *cx);
      CxDeny denial;
      stringToPacket(denial.reason, "Server is shutdowning", 64);
      cx->send(&denial);
      delete cx;
      return false;
    }
  return true;
}

bool    Server::checkRemoteVersion(Cx* cx, const CxInit& pkt)
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
      cx->send(&pkt_deny);
      delete cx;
      return false;
    }
  return true;
}

bool Server::checkRemoteModuleDesc(Cx* cx, const CxJoin& pkt, int game_uid)
{
  std::string client_rules_name = packetToString(pkt.rules_name);
  if (client_rules_name != desc_->name)
    {
      std::ostringstream os;
      os << "Rules mismatch: server's is `" << desc_->name << "'";
      os << ", client is `" << client_rules_name << "'";
      LOG3("Joining game %1 from %2 has been rejected, reason:", *cx);
      LOG3(" - %1", os.str());
      CxDeny pkt_deny;
      stringToPacket(pkt_deny.reason, os.str(), 64);
      cx->send(&pkt_deny);
      delete cx;
      return false;
    }
  if (pkt.rules_major != desc_->major || pkt.rules_minor != desc_->minor)
    {
      std::ostringstream os;
      os << "Version mismatch: server's is `" << desc_->major << "."
	 << desc_->minor << "'"
	 << ", client is `" << pkt.rules_major << "." << pkt.rules_minor << "'";
      LOG3("Joining game %1 from %2 has been rejected, reason:", *cx);
      LOG3(" - %1", os.str());
      CxDeny pkt_deny;
      stringToPacket(pkt_deny.reason, os.str(), 64);
      cx->send(&pkt_deny);
      delete cx;
      return false;
    }
  return true;
}

// Check if host is allowed on our system (ip in blacklist, etc...).
// If it's bad, the connection is closed right now.
bool    Server::checkRemoteTCP(TcpCx* cx, const Packet&)
{
  bool  bad = false;

  // FIXME: do that, check for bad guys.

  if (bad)
    {
      LOG3("Connection from %1 has been rejected.", *cx);
      CxDeny denial;
      stringToPacket(denial.reason, "I don't like you.", 64);
      cx->send(&denial);
      delete cx;
      return false;
    }

  LOG4("Connection from '%1` has been accepted.", *cx);
  Packet acpt(CX_ACCEPT);
  cx->send(&acpt);
  return true;
}

// Send to the client the list of active games.
void	Server::serveGameList(Cx* cx)
{
  // FIXME: send him the list of games currently hosted
  LOG5("A client is requesting the list of games.");
  Packet pkt_list(CX_LIST);
  cx->send(&pkt_list);
}

void	Server::serveJoinGame(Cx* cx, Packet* pkt)
{
  CxJoin* pkt_join = static_cast<CxJoin*>(pkt);
  int game_uid = pkt_join->game_uid;

  if (!checkRemoteModuleDesc(cx, *pkt_join, game_uid))
    return;
  
  GameIter it = games_.find(game_uid);
  if (it == games_.end())
    {
      if (!is_persistent_ && !games_.empty())
	{
	  LOG3("Cannot create a new game. Server is not persistent.");
	  delete cx;
	  return;
	}
      // This is a new game. Create and start it.
      GameHosting* gh = new GameHosting(game_uid, cfg_,
					create_rules_fun_(&cfg_));
      it = games_.insert(std::make_pair(game_uid, gh)).first;
      pthread_t th;
      pthread_create(&th, NULL, &GameHosting::startThread, it->second);
      LOG5("Thread started.");
    }
  it->second->addClient(cx,
			pkt_join->client_extid,
			pkt_join->is_coach ? true : false);
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
  TcpCxListener listener(&listen_socket, this);
  Timer		wait_timeout(0);
  
  is_persistent_ = cfg_.getAttr<bool>("server", "options", "persistent");
  if (!is_persistent_)
    wait_timeout.setAllowedTime(cfg_.getAttr<int>("server", "options", "wait_timeout"));
  
  LOG2("Listening on port %1", cfg_.getAttr<int>("server", "listen", "port"));
  if (wait_timeout.getAllowedTime() > 0)
    {
      LOG2("Will wait for `%1' second that a game begin before exiting",
	   wait_timeout.getAllowedTime());
      wait_timeout.start();
    }
  listen_socket.listenAt(cfg_.getAttr<int>("server", "listen", "port"));
  waiting_clients_.reserve(1024); // should be max cx;
  waiting_clients_.push_back(&listener);

  while (!server_shutdown_ || !games_.empty() || waiting_clients_.size() > 1)
    {
      wcl_poll_.poll();
      if (cleanFinishedGame() && !is_persistent_)
        break;
      if (wait_timeout.isTimeElapsed() && games_.empty())
	break;
    }
  LOG1("Server has finished its work. Exiting.");
}

END_NS(server);
