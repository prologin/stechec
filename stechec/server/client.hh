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

# include "tools.hh"
class Cx;

/*!
** @addtogroup server
** @{
*/

BEGIN_NS(server);


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
  Client(Cx* cx);
  virtual ~Client();

  //! @brief Get the file descriptor associated with the connection.
  int           getFd() const;

  //! @brief Fetch a packet from the network for this client.
  //! @note This function may block, so be sure to poll before.
  //! @return A packet, or NULL if this client is dead.
  //! @throw NetError Thrown on any kind of network error.
  Packet*       getPacket();

  //! @brief Called by NetPool when fd received something.
  virtual bool	recvReady() = 0;

protected:
  Cx*		cx_;            ///< Connection to the server.
  bool		close_pending_;	///< We had errors on this socket, to be closed ASAP.
};

END_NS(server);

/*
** Implementation.
*/

# include "datatfs/cx.hh"

BEGIN_NS(server);

inline Client::Client(Cx* cx)
  : cx_(cx),
    close_pending_(false)
{
}

inline Client::~Client()
{
}

inline int Client::getFd() const
{
  return cx_ == NULL || close_pending_ ? -1 : cx_->getFd();
}

inline Packet* Client::getPacket()
{
  return cx_->receive();
}


//! @}

END_NS(server);

#endif /* !CLIENT_HH_ */
