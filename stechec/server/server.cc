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

#include <signal.h>
#include "server.hh"

#define SERVER_BINARY_VERSION 1

Server* Server::inst = NULL;

Server::Server(const xml::XMLConfig& cfg)
  : cfg_(cfg),
    wcl_poll_(waiting_clients_, 3000),
    server_shutdown_(false)
{
  LOG1(PACKAGE_NAME << " server v" PACKAGE_VERSION << " initializing...");

  // Singleton.
  assert(inst == NULL);
  inst = this;
  
  // Catch ctl-c, to properly shutdown the server.
  struct sigaction act;
  int netbsdsuck;
  netbsdsuck = sigemptyset(&act.sa_mask);
  act.sa_handler = &Server::wantShutdown;
  act.sa_flags = 0;
  sigaction(SIGINT, &act, NULL);
  
  // Load shared library to get the rules.
  try {
    rules_name_ = cfg.getData<std::string>("rules");
  } catch (xml::XMLError) {
    LOG1("Note: previous error is harmless.");
    cfg.switchSection("game");
    rules_name_ = cfg.getData<std::string>("rules");
    cfg.switchSection("server");
  }
  rules_.open(rules_name_);
  create_rules_fun_ = (create_rules_t)(rules_.getSymbol("load_server_rules"));
}

Server::~Server()
{
  inst = NULL;
}

// Ctl-C was pressed, shutdown it after all games were finished.
void   Server::wantShutdown(int)
{
  LOG1("Shutdown requested. Will stop when everybody will be disconnected.");
  inst->server_shutdown_ = true;
}

bool    Server::checkServerState(Cx* cx)
{
  if (server_shutdown_)
    {
      LOG2("Connection from " << *cx << " has been rejected. Server is shutdowning.");
      Packet denial(CX_DENY);
      cx->send(&denial);
      delete cx;
      return false;
    }
  return true;
}

bool    Server::checkRemoteVersion(Cx* cx, const CxInit& pkt)
{
  if (pkt.binary_version != SERVER_BINARY_VERSION)
    {
      std::ostringstream os;
      os << "Binary version mismatch: server is `" << SERVER_BINARY_VERSION;
      os << "', client is `" << pkt.binary_version << "'";
      LOG3("Connection from " << *cx << " has been rejected, reason:");
      LOG3(" - " << os.str());
      CxDeny pkt_deny;
      stringToPacket(pkt_deny.reason, os.str(), 64);
      cx->send(&pkt_deny);
      delete cx;
      return false;
    }

  std::string client_rules_name = packetToString(pkt.rules_name);
  if (client_rules_name.substr(0, 4) != rules_name_.substr(0, 4))
    {
      std::ostringstream os;
      os << "Rules mismatch: server's is `" << rules_name_ << "'";
      os << ", client is `" << client_rules_name << "'";
      LOG3("Connection from " << *cx << " has been rejected, reason:");
      LOG3(" - " << os.str());
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
      LOG3("Connection from " << *cx << " has been rejected.");
      CxDeny denial;
      stringToPacket(denial.reason, "I don't like you.", 64);
      cx->send(&denial);
      delete cx;
      return false;
    }

  LOG4("Connection from '" << *cx << "' has been accepted.");
  Packet acpt(CX_ACCEPT);
  cx->send(&acpt);
  return true;
}

// Return true if it should be removed from waiting_clients_
bool    Server::serveClient(Cx* cx)
{
  Packet* pkt;

  pkt = cx->receive();
  if (pkt->token == CX_QUERYLIST)
    {
      // FIXME: send him the list of games currently hosted
      LOG5("A client is requesting the list of games.");
      Packet pkt_list(CX_LIST);
      cx->send(&pkt_list);
      return false;
    }
  else if (pkt->token == CX_JOIN)
    {
      CxJoin* pkt_join = static_cast<CxJoin*>(pkt);
      int game_uid = pkt_join->game_uid;
      
      GameIter it = games_.find(game_uid);
      if (it == games_.end())
        {
          if (!is_persistent_ && !games_.empty())
            {
              LOG3("Cannot create a new game. Server is not persistent.");
              delete cx;
            }
          else
            {
              // This is a new game. Create and start it.
              GameHosting* gh = new GameHosting(game_uid, cfg_,
                                            create_rules_fun_(&cfg_));
              it = games_.insert(std::make_pair(game_uid, gh)).first;
              pthread_t th;
              pthread_create(&th, NULL, &GameHosting::startThread, it->second);
              LOG5("Thread started.");
            }
        }
      it->second->addClient(cx,
                            pkt_join->client_extid,
                            pkt_join->is_coach ? true : false);
    }
  else if (pkt->token == CX_ABORT)
    {
      LOG4("Client " << *cx << " sent CX_ABORT. Close its connection.");
      delete cx;
    }
  else
    {
      LOG2("Unknown message (" << pkt->token << ") from waiting client '"
           << *cx << "'. Kill it.");
      delete cx; // "/me takes aim: HEADSHOT !"
    }
  return true;
}

// Check for games that are finished, to join thread and
// free memory.
// Return true if a game was cleaned.
bool    Server::cleanFinishedGame()
{
  bool  ret_value = false;

  for (GameIter it = games_.begin(); it != games_.end(); )
    {
      if (it->second->isFinished())
        {
          LOG5("Clean a thread, used to belong to game uid " << it->first << ".");
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
  TcpCx          listen_socket;

  is_persistent_ = cfg_.getAttr<bool>("options", "persistent");
  
  LOG2("Listening on port " << cfg_.getAttr<int>("listen", "port"));
  listen_socket.listenAt(cfg_.getAttr<int>("listen", "port"));
  waiting_clients_.reserve(1024); // should be max cx;
  waiting_clients_.push_back(&listen_socket);

  while (!server_shutdown_ || !games_.empty() || waiting_clients_.size() > 1)
    {
      int nb_ready = wcl_poll_.poll();

      for (int i = 0; i < nb_ready; i++)
        if (waiting_clients_[i] == &listen_socket)
          {
            LOG4("New connection detected, handling it.");
            TcpCx* client_cx = listen_socket.accept();
            try {
              Packet* pkt = client_cx->receive();
              const CxInit& init_pkt = static_cast<CxInit&>(*pkt);
              if (checkServerState(client_cx)
                  && checkRemoteVersion(client_cx, init_pkt)
                  && checkRemoteTCP(client_cx, init_pkt))
                waiting_clients_.push_back(client_cx);
            } catch (const NetError& e) {
              LOG2("Network error on connection (from " << *client_cx
                   << "): " << e);
              delete client_cx; // should not be pushed on waiting_clients_.
            }
          }
        else
          {
            bool remove_from_wc = true;
            Cx* client_cx = waiting_clients_[i];
            try {
              remove_from_wc = serveClient(client_cx);
            } catch (const NetError& e) {
              LOG2("Network error on waiting client (from " << *client_cx
                   << "): " << e);
              delete client_cx;
            }
            if (remove_from_wc)
              {
                // be soft with our vector.
                const int wc_size = waiting_clients_.size();
                waiting_clients_[i] = waiting_clients_[wc_size - 1];
                waiting_clients_.erase(waiting_clients_.end() - 1);
                if (nb_ready >= wc_size)
                  nb_ready--;
              }
          }
      if (cleanFinishedGame() && !is_persistent_)
        break;
    }
  LOG1("Server has finished its work. Exiting.");
}
