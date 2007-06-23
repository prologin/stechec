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

#ifndef SERVER_HH_
# define SERVER_HH_

# include <map>
# include <string>

# include "tools.hh"
# include "xml/xml_config.hh"
# include "datatfs/CxPool.hh"
# include "GameHosting.hh"

//! @brief Maximum number of coachs + spectators connected on server.
//! @ingroup server
# define MAX_CONNECTION        250

class Cx;
class TcpCx;
class Client;

BEGIN_NS(server);

/*!
** @brief Control multiple game, receive new clients, handle
** spectators...
** @ingroup server
*/
class Server
{
public:
  Server(const xml::XMLConfig& cfg);
  ~Server();

  //! @brief Run the server, never ending function...
  void          run();

private:
  bool          cleanFinishedGame();
  static void   wantShutdown(int signal);

  // These functions check client before accepting it. Return false if
  // it should be discarded.
  bool          checkServerState(Cx* cx);
  bool          checkRemoteTCP(TcpCx* cx);
  bool          checkRemoteVersion(Cx* cx, const CxInit& pkt);

  bool	        serveGameList(Client* cl, Packet* pkt);
  bool	        serveJoinGame(Client* cl, Packet* pkt);
  bool          serveInitPacket(Client* cl, Packet* pkt);
  void          receivePacket(Cx* cx);
  void          serveNewConnection(TcpCx* cxl);

  static Server*                  inst;

  const xml::XMLConfig&           cfg_;

  // Rules loaded on server
  struct Rules
  {
    typedef BaseSRules*           (*create_rules_t)(const xml::XMLConfig*);
    const struct RuleDescription* desc_;
    Library                       lib_;
    create_rules_t                create_fun_;
  };
  typedef std::map<std::string, Rules*> RulesList;
  RulesList                     rules_;

  // Games running on the server
  typedef std::map<int, GameHosting*> GameList;
  typedef GameList::iterator    GameIter;
  GameList                      games_;

  // Clients connected, waiting to enter on a game
  typedef std::map<Cx*, Client*> ClientList;
  ClientList                    cl_;
  CxPool<Cx>                    cl_pool_;

  int                           server_shutdown_;
  Timer				server_shutdown_reset_;
  bool                          is_persistent_;

  pthread_mutex_t		lock_;
};

END_NS(server);

#endif /* !SERVER_HH_ */
