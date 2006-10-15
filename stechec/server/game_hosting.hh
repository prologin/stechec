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

#ifndef GAME_HOSTING_HH_
# define GAME_HOSTING_HH_

# include "tools.hh"
# include "datatfs/netpoll.hh"
# include "datatfs/file.hh"
# include "xml/xml_config.hh"
# include "client.hh"
# include "BaseSRules.hh"
# include "PacketSender.hh"

/*!
** @addtogroup server
** @{
*/

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

  //! @brief Check is the game is finished, so memory can be freed.
  bool isFinished() const;

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
  bool processOne(Client* cl, std::string& remove_reason);
  bool process();
  void run(Log& log);

  typedef std::vector<Client*>                ClientList;
  typedef ClientList::iterator                ClientIter;

  const xml::XMLConfig& cfg_;             ///< Server configuration.
  FileCx                log_;             ///< File to log game to.

  int                   game_uid_;        ///< Game uid.
  BaseSRules*           rules_;           ///< Server rules.
  ClientList            client_list_;     ///< List of active/dead clients.
  ClientList            coach_list_;      ///< List of coachs. To output statistics.
  NetPoll<Client*>      client_poll_;     ///< Facility to poll all clients/dead clients at once.
  int                   nb_coach_;        ///< Number of coach connected.
  int                   nb_waited_coach_; ///< Number of waited coach, before starting game.
  int                   viewer_base_uid_; ///< Current uid to assign to new viewers.
  int                   nb_viewer_;       ///< Number of viewers currently connected.
  int                   nb_waited_viewer_;///< Number of waited viewer, before starting game.
  bool                  started_;         ///< Is the game started ?
  bool                  game_finished_;   ///< Is the game finished ?
  bool			thread_finished_; ///< Can the thread be cleaned ?
  pthread_t             self_;
};

//! @}

#endif /* !GAME_HOSTING_HH_ */
