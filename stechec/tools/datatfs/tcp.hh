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

#ifndef TCP_HH_
# define TCP_HH_

# include "tools.hh"
# include "cx.hh"

/*!
** @ingroup tools_net
** @{
*/

/*! \brief Implements TCP networking.
**
** This class implement TCP/IP data connections. The Nagle algorithm
** is turned off, to speed up little but frequent transfers.
**
** \code
** TcpCx* cx = new TcpCx;
** cx->connect("localhost", 4242);
** cx->send(Packet(CX_INIT, 1));
** cx->sendFile("someFile.txt");
** Packet p;
** cx->receive(p);
** delete cx; // close connection
** \endcode
**
** \author victor
** \date 01/04/2005
*/
class TcpCx : public Cx
{
public:
  TcpCx();
  virtual ~TcpCx();

  //! \brief Open a TCP connection to an host, to a specified port. For client.
  //! \param host Host to connect to. Could be an IP adresse or an
  //! host name resolvable by the operating system.
  //! \param port Port to connect to (1-65535).
  //! \exception NetError Thrown when could not join host.
  void connect(const char* host, int port);

  //! \brief Prepare to accepts connection. For server.
  //! \param port Will accept connection on this port.
  //! \note Currently, binds on IN_ADDR_ANY.
  //! This function doesn't block.
  //! \exception NetError Thrown on some random OS error.
  void listenAt(int port);

  //! \brief Accept any connection from the outside. For server.
  //! \return A newly instance of TcpCx, connected with a remote host.
  //! \exception NetError Thrown on some random OS error.
  TcpCx* accept();

  virtual bool poll(int timeout = 0);

protected:
  virtual void print(std::ostream& os) const;
  
private:
  TcpCx(int fd);

  void setTimeout();
  void setSocketOptions();
  void checkError(int ret, const char* funcName);

  int remote_port_;
  int remote_ip_;
};

// !ingroup tools_net
//! @}

#endif /* !TCP_HH_ */
