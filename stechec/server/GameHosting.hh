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

#ifndef GAME_HOSTING_HH_
# define GAME_HOSTING_HH_

# include "tools.hh"
# include "datatfs/CxPool.hh"
# include "datatfs/FileCx.hh"
# include "xml/xml_config.hh"
# include "BaseSRules.hh"
# include "PacketSender.hh"

BEGIN_NS(server);

class Client;

//! @brief Status of a single server game thread.
//! @ingroup server
enum eGameState {
  eStarting = 0,    ///< Starting thread.
  eWaiting,     ///< Wait for players.
  ePlaying,     ///< Game playing.
  eFinishing,   ///< Game finished. Wait for last clients to exit.
  eFinished,    ///< Thread can be cleaned.
  eCrashed,     ///< Thread can be cleaned, but it was termined anormally.
};

/*! @brief Represents a game, with up to two coachs and as many
** viewers as they could be.
** @ingroup server
**
** Each game is identified by a uid, passed to the ctor.
**
** Instance of this class are under Server control, each of them
** run in a separate thread.
*/
class GameHosting : public PacketSender
{
public:
  GameHosting(int game_uid, const xml::XMLConfig& cfg, BaseSRules* rules);
  ~GameHosting();

  //! @brief Add a client to this game.
  //! @param cl Client data.
  void addClient(Client* cl);

  //! @brief Get client current status.
  enum eGameState getState() const;

  //! @brief Get the thread_t structure of the thread running this game.
  //! Mostly useful for joining with the main thread.
  pthread_t getThreadId() const;
  
  //! @brief Send a packet to all clients.
  //!   Automatically called from the Arbiter.
  virtual void sendPacket(const Packet& p);

  //! @brief Start the thread. Call run(), and exit.
  //! @param gh_inst A GameHosting instance, corresponding to the game.
  static void* startThread(void* gh_inst);

private:
  void outputStatistics();
  void run(Log& log);

  void spectatorReadiness(Client* cl);
  void servePlaying(Client* cl);
  void killClient(Client *cl, const std::string& msg);
  void waitCoaches();
  void playGame();
  
  typedef std::vector<Client*>      ClientList;
  typedef std::map<Cx*, Client*>    ClientMapList;

  const xml::XMLConfig& cfg_;             ///< Server configuration.
  FileCx                log_;             ///< File to log game to.

  CxPool<Cx>            cl_pool_;         ///< Facility to poll all clients at once.
  ClientList            cl_;              ///< Active client list.
  ClientMapList         cl_map_;          ///< Active client list (map version).
  ClientList            coach_;           ///< Coach list (alive and died), for statistics.

  BaseSRules*           rules_;           ///< Server rules.
  int                   game_uid_;        ///< Game uid.
  int                   nb_coach_;        ///< Number of coach connected.
  int                   nb_waited_coach_; ///< Number of waited coach, before starting game.
  int                   nb_team_;         ///< Number of playing teams.
  int                   viewer_base_uid_; ///< Current uid to assign to new viewers.
  int                   nb_viewer_;       ///< Number of viewers currently connected.
  int                   nb_waited_viewer_;///< Number of waited viewer, before starting game.
  enum eGameState	state_;		  ///< Game status.

  pthread_t             self_;
  pthread_mutex_t	lock_;
};

END_NS(server);

#endif /* !GAME_HOSTING_HH_ */
