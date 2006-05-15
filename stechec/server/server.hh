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

#ifndef SERVER_HH_
# define SERVER_HH_

# include "tools.hh"
# include "datatfs/tcp.hh"
# include "game_hosting.hh"

/*! @file src/server/server.hh
**
** @defgroup server Generic server application.
** @{
*/

//! @brief Maximum number of coachs + spectators connected on server.
# define MAX_CONNECTION        250

/*! @brief Class that handle spectators, multiple games...
**
** @author victor
** @date 15/01/2006
*/
class Server
{
public:
  Server(const xml::XMLConfig& cfg);
  ~Server();

  //! @brief Run the server, never ending function...
  void          run();

private:
  bool          checkServerState(Cx* cx);
  bool          checkRemoteVersion(Cx* cx, const CxInit& pkt);
  bool          checkRemoteTCP(TcpCx* cx, const Packet& pkt);
  bool          cleanFinishedGame();
  bool          serveClient(Cx* cx);
  static void   wantShutdown(int signal);

  static Server* inst;
  
  const xml::XMLConfig& cfg_;

  std::string   rules_name_;
  Library       rules_;
  typedef BaseSRules* (*create_rules_t)(const xml::XMLConfig*);
  create_rules_t create_rules_fun_;
  
  typedef std::map<int, GameHosting*> GameList;
  typedef GameList::iterator    GameIter;
  GameList                      games_;

  typedef std::vector<Cx*>      CxList;
  CxList                        waiting_clients_;
  NetPoll<Cx*>                  wcl_poll_;

  bool                          server_shutdown_;
  bool                          is_persistent_;
};

//! @}

#endif /* !SERVER_HH_ */
