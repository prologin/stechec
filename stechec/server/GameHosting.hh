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

#ifndef GAME_HOSTING_HH_
# define GAME_HOSTING_HH_

# include "tools.hh"
# include "datatfs/netpoll.hh"
# include "datatfs/file.hh"
# include "xml/xml_config.hh"
# include "BaseSRules.hh"
# include "PacketSender.hh"

BEGIN_NS(server);

class GameClient;

/*!
** @addtogroup server
** @{
*/

//! @brief Status of a single server game thread.
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
  //! @param cx The associated connection.
  //! @param client_extid The client game id, used to identify the client outside
  //!   the game (ie: league).
  //! @param wanna_be_coach True if the client wants to be a coach.
  void addClient(Cx* cx, int client_extid, bool wanna_be_coach);

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

  void clientDied(GameClient* cl);

  void spectatorReadiness(GameClient* cl);
  void servePlaying(GameClient* cl, Packet* pkt);

private:
  void outputStatistics();
  void run(Log& log);

  typedef std::vector<GameClient*>      GameClientList;
  typedef GameClientList::iterator      GameClientIter;
  typedef std::vector<ClientStatistic*>	ClientStatisticList;

  const xml::XMLConfig& cfg_;             ///< Server configuration.
  FileCx                log_;             ///< File to log game to.

  int                   game_uid_;        ///< Game uid.
  BaseSRules*           rules_;           ///< Server rules.
  GameClientList        client_list_;     ///< List of active/dead clients.
  NetPoll<GameClient*>  client_poll_;     ///< Facility to poll all clients/dead clients at once.
  ClientStatisticList   stats_list_;      ///< Stats of coachs. To output statistics after they exited.
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

//! @}

END_NS(server);

#endif /* !GAME_HOSTING_HH_ */
