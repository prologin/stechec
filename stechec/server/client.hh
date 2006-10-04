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

#ifndef CLIENT_HH_
# define CLIENT_HH_

# include "BaseSRules.hh"
# include "datatfs/cx.hh"

/*!
** @addtogroup server
** @{
*/

/*!
** @brief Clients statistics that can be retrieved from Client.
**
** Originated from a kludge: Generic server and arbiter have several
** different informations on each client. They are all grouped here (with
** the CoachErrorCustom rules class), then resend to rules at the end so it can
** write stats, calculate scores, ...
*/
class ClientStatistic
{
public:
  int                   ext_id_;
  std::string           fail_reason_;
  CoachErrorCustom*     custom_;
};

/*!
** @brief Represents an external connection on the server, coach or
** spectator, on the generic server side.
**
** This class only encapsulate a Cx object, adding some useful informations
** like the connection uid, and if it a coach or a viewer.
*/
class Client
{
public:
  //! @brief Create a Client object.
  //!
  //! Constructing this object means that client has already chosen the
  //! game, was accepted, and is ready to play. Tell him it's ok,
  //! and send him its uid.
  //!
  //! @param cx The associated Cx object.
  //! @param id The unique client connection id (See UID_VIEWER_BASE and UID_COACH_BASE).
  //! @param client_gid The client external game identifier (eg: for league).
  //! @param nb_team Number of coach for this game.
  Client(Cx* cx, int id, int client_gid, int nb_team);
  ~Client();

  //! @brief Get the file descriptor associated with the connection.
  int           getFd() const;

  //! @brief Get the client id.
  int           getId() const;
  
  //! @brief Fetch a packet from the network for this client.
  //! @note This function may block, so be sure to poll before.
  //! @return A packet, or NULL if this client is dead.
  //! @throw NetError Thrown on any kind of network error.
  Packet*       getPacket();

    //! @brief Check is this connection is associated with a real coach
  //!   or a viewer.
  bool          isCoach() const;

  //! @brief Return true if this client is ready.
  bool          isReady() const;

  //! @brief Set client readyness.
  void          setReady(bool value);

    //! @brief Check whether this client is dead, wait that it closes its
  //!  connection. Do not process any message from it, do not send it anything.
  bool		isDead() const;

  //! @brief Set client state to dead, either we don't want it anymore, or
  //!  we had a connection error with it.
  //! @param fail_msg A reason of why it dead. For statistics.
  //! @param kill_now true if we can close its socket now. false if we wait
  //!   that it'll do that cleany later.
  void          setDead(std::string fail_msg, bool kill_now = false);

    //! @brief Discard input. Only works on dead client
  //! @return true if it can be deleted.
  bool		discardInput();

  //! @brief Get the coach statistics.
  ClientStatistic& getClientStatistic();
  
  // Used jointly with stl algorithm, for batch-sending packets.
  class Send
  {
  public:
    Send(const Packet& p);
    void operator() (Client* cl);
  private:
    const Packet& p_;
  };
  
private:
  Cx*                cx_;         ///< Connection to the server.
  int                id_;         ///< Player uid.

  //! True if this client is ready (mostly for the next turn)
  //! (sync visio with game).
  bool               is_ready_;

  bool		     is_dead_;

  ClientStatistic    client_stat_; ///< Clients statistics.
};

# include "Client.hxx"

//! @}

#endif /* !CLIENT_HH_ */
