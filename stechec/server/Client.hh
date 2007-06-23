/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2007 Prologin
*/

#ifndef CLIENT_HH_
# define CLIENT_HH_

# include <string>

class Cx;
class CoachErrorCustom;

/*!
** @brief Clients statistics that can be retrieved from Client.
** @ingroup server
**
** Originated from a kludge: Generic server and arbiter have several
** different informations on each client. They are all grouped here (with
** the CoachErrorCustom rules class), then resend to rules at the end so it can
** write stats, calculate scores, ...
*/
class ClientStatistic
{
public:
  ClientStatistic()
    : id_(-1), ext_id_(-1), custom_(NULL)
  {}

  int			id_;
  int                   ext_id_;
  std::string           fail_reason_;
  CoachErrorCustom*     custom_;
};


BEGIN_NS(server);


/*!
** @brief Keep additional data about a client (statistics, status, ...)
** @ingroup server
*/
class Client
{
  friend class Server;
  friend class GameHosting;
  
public:
  Client(Cx* cx);
  virtual ~Client();

  int           getId() const;
  int           getLeagueId() const;
  bool          isCoach() const;
  void          setFailReason(const std::string& msg, int prio);

  void          begin();
  void          commit();
  void          send(const Packet* p);
    
private:
  Cx*           cx_;

  bool          init_pkt_received_;
  bool          game_joined_;

  int           id_;
  int           league_id_;
  bool          is_coach_;
  bool          is_ready_;              ///< Spectators

  //! @brief Highest failure msg set. Any attempt to set a failure
  //!  message with a priority less than that will be ignored.
  int           fail_priority_;              
  ClientStatistic st_;
};

END_NS(server);



/*
** Implementation.
*/

# include "datatfs/Cx.hh"

BEGIN_NS(server);

inline Client::Client(Cx* cx)
  : cx_(cx),
    init_pkt_received_(false),
    game_joined_(false),
    id_(-1),
    league_id_(-1),
    is_coach_(false),
    is_ready_(false),
    fail_priority_(-1)
{
}

inline Client::~Client()
{
  delete cx_;
}

inline int Client::getId() const
{
  return id_;
}

inline int Client::getLeagueId() const
{
  return league_id_;
}

inline bool Client::isCoach() const
{
  return is_coach_;
}

inline void Client::setFailReason(const std::string& msg, int prio)
{
  if (fail_priority_ < prio)
    {
      st_.fail_reason_ = msg;
      fail_priority_ = prio;
    }
}

inline void Client::begin()
{
  cx_->begin();
}

inline void Client::commit()
{
  try {
    cx_->commit();
  } catch (const NetError& e) {
    setFailReason(e.what(), 0);
  }
}

inline void Client::send(const Packet* p)
{
  try {
    cx_->send(*p);
  } catch (const NetError& e) {
    setFailReason(e.what(), 0);
  }
}

END_NS(server);

#endif /* !CLIENT_HH_ */
