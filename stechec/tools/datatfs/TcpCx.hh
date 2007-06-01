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

#ifndef TCPCX_HH_
# define TCPCX_HH_

# include "Cx.hh"

/*! @brief Implements TCP networking.
**
** This class implement TCP/IP data connections. Nagle algorithm
** is turned off, to speed up little but frequent transfers.
**
** @code
** TcpCx* cx = new TcpCx;
** Packet* p;
**
** cx->connect("localhost", 4242);
** if (!cx->poll(5)) {
**   delete cx;// connection failed
**   return;
** }
** cx->send(Packet(CX_INIT, 1));
** if (!cx->poll(5)) {
**   delete cx; // can't receive anything
**   return;
** }
** p = cx->receive();
** delete cx; // close connection
** @endcode
*/
class TcpCx : public Cx
{
public:
  TcpCx();
  virtual ~TcpCx();

  //! @brief Open a TCP connection to an host, to a specified port. For client.
  //! @param host Host to connect to. Could be an IP adresse or an
  //!   host name resolvable by the operating system.
  //! @note This function doesn't block. Actually, the connection could not be
  //!   made when this function returns. Call poll to check when connected.
  //! @param port Port to connect to (1-65535).
  //! @exception NetError Thrown on OS error.
  void connect(const char* host, int port);

  //! @brief Prepare to accepts connection. For server.
  //! @param port Will accept connection on this port.
  //! @note Currently, binds on IN_ADDR_ANY.
  //!    This function doesn't block.
  //! @exception NetError Thrown on OS error.
  void listen(int port);

  //! @brief Accept any connection from the outside. For server.
  //! @return A newly instance of TcpCx, connected with a remote host.
  //! @note This function blocks. If you don't want that, use poll() before.
  //! @exception NetError Thrown on OS error.
  TcpCx* accept();

protected:
  virtual bool pollInternal(int* timeout);
  virtual int recvData(bool use_exception);
  virtual void sendData(unsigned char* data, unsigned size);
  virtual void print(std::ostream& os) const;
  
private:
  //! @brief Private constructor, to create object on 'accept'.
  TcpCx(int fd);

  void setSocketOptions();

  int remote_port_;
  int remote_ip_;
};

#endif /* !TCPCX_HH_ */
