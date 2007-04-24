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

#ifndef GAMECLIENT_HH_
# define GAMECLIENT_HH_

# include "Client.hh"

class CoachErrorCustom;

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
  ClientStatistic(int id, int ext_id)
    : id_(id), ext_id_(ext_id), custom_(NULL)
  {}

  int			id_;
  int                   ext_id_;
  std::string           fail_reason_;
  CoachErrorCustom*     custom_;
};


BEGIN_NS(server);

class GameHosting;


class GameClient : public Client
{
public:
  
  //! @brief Create a GameClient object.
  //!
  //! Constructing this object means that client has already chosen the
  //! game, was accepted, and is ready to play. Tell him it's ok,
  //! and send him its uid.
  //!
  //! @param gh Pointer to the associated game.
  //! @param cx The associated Cx object.
  //! @param id The unique client connection id (See UID_VIEWER_BASE and UID_COACH_BASE).
  //! @param league_id The client external game identifier.
  //! @param nb_team Number of team for this game.
  //! @param nb_coach Number of coach for this game.
  GameClient(GameHosting* gh, Cx* cx, int id, int league_id, int nb_team, int nb_coach);
  virtual ~GameClient();

  //! @brief Get the client id.
  int           getId() const;

  //! @brief Get the client league id.
  int		getLeagueId() const;

  //! @brief Close client connection.
  //! @note Connection is immediately closed unless close_pending_ is true.
  //!  The latter is useful to keep connection receive buffer until next poll().
  //! @param fail_msg A reason of why it dead. For statistics.
  void          kill(std::string fail_msg);
  
  //! @brief Check is this connection is associated with a real coach
  //!   or a viewer.
  bool          isCoach() const;

  //! @brief Return true if this client is ready.
  bool          isReady() const;

  //! @brief Set client readyness.
  void          setReady(bool value);

  virtual bool	recvReady();
  
  //! @brief Get the coach statistics.
  ClientStatistic& getClientStatistic();

  // Used jointly with stl algorithm, for batch-sending packets.
  class Send
  {
  public:
    Send(const Packet& p);
    void operator() (GameClient* cl);
  private:
    const Packet& p_;
  };
  
private:
  //! @brief Discard input. Only works on dead client
  //! @return true if it can be deleted.
  bool discardInput();

  GameHosting*		gh_;
  int			id_;         ///< Player uid.
  int			league_id_;  ///< Player league id.

  //! True if this client is ready (mostly for the next turn)
  //! (sync visio with game).
  bool			is_ready_;

  ClientStatistic*	client_stat_; ///< Clients statistics.
};

END_NS(server);

#endif /* !GAMECLIENT_HH_ */
