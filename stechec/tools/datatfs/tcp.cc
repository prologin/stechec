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

#include "tcp.hh"

TcpCx::TcpCx()
  : remote_port_(0),
    remote_ip_(0)
{
  initialize_socket();
}

TcpCx::TcpCx(int fd)
  : remote_port_(0),
    remote_ip_(0)
{
  this->fd = fd;
}

TcpCx::~TcpCx()
{
}

void                TcpCx::checkError(int ret, const char* funcName)
{
  if (ret < 0)
    { 
      sprintf(msgerr, "%s: %s", funcName, strerror(errno));
      throw NetError(msgerr);
    }
}

void                TcpCx::setTimeout()
{
  struct timeval        to = {DEFAULT_NETCX_TO, 0};
  int                        ret;

  ret = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*) &to, sizeof(struct timeval));
  checkError(ret, "setsockopt(SO_RCVTIMEO)");
  ret = setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const char*) &to, sizeof(struct timeval));
  checkError(ret, "setsockopt(SO_SNDTIMEO)");
}

/* turn off Nagle's algorithm to speed up transfert */
void                TcpCx::setSocketOptions()
{
  int                        tcpnodelay_flag = 1;
  int                        ret;
  
  ret = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *) &tcpnodelay_flag, sizeof(int));
  checkError(ret, "setsockopt(TCP_NODELAY)");

  // BSD specific ? If we can do that, do it.
#ifdef SO_NOSIGPIPE
  ret = setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, (char *) &tcpnodelay_flag, sizeof(int));
  checkError(ret, "setsockopt(SO_NOSIGPIPE)");
#endif
}

// Connect to a host
void                TcpCx::connect(const char* host, int port)
{
  socklen_t                addrlen;
  struct sockaddr_in        data_peer;
  struct sockaddr_in        sin;
  struct hostent        *hp;
  int                        ret;
  
  if ((hp = gethostbyname(host)) == NULL) 
    {
      sprintf(msgerr, "%s: host not found.", host);
      throw NetError(msgerr);
    }
  memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length); 
  sin.sin_family = hp->h_addrtype;
  sin.sin_port = htons(port);

  if (fd >= 0)
    closesocket(fd);

  fd = socket(hp->h_addrtype, SOCK_STREAM, 0);
  checkError(fd, "socket");

  setTimeout();
  setSocketOptions();

  ret = ::connect(fd, (struct sockaddr *)&sin, sizeof (sin));
  checkError(ret, "connect");

  addrlen = sizeof (struct sockaddr_in);
  ret = getpeername(fd, (struct sockaddr *) &data_peer, &addrlen);
  checkError(ret, "getpeername");
  remote_port_ = data_peer.sin_port;
  remote_ip_ = data_peer.sin_addr.s_addr;
}

void                TcpCx::listenAt(int port)
{
  struct sockaddr_in        sin;
  int                        value = 1;
  int                        ret;

  assert(fd == -1);

  fd = socket(AF_INET, SOCK_STREAM, 0);
  checkError(fd, "socket");

  ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*) &value, sizeof(value));
  checkError(ret, "setsockopt(SO_REUSEADDR)");
  
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
  ret = bind(fd, (struct sockaddr *)&sin, sizeof (sin));
  checkError(ret, "bind");

  ret = listen(fd, 35);
  checkError(ret, "listen");
}

TcpCx*                TcpCx::accept()
{
  socklen_t                addrlen;
  struct sockaddr_in        data_addr;
  struct sockaddr_in        data_peer;
  int                        newfd;
  int                        ret;

  addrlen = sizeof (struct sockaddr_in);
  newfd = ::accept(fd, (struct sockaddr *) &data_addr, &addrlen);
  checkError(newfd, "accept");
  
  addrlen = sizeof (struct sockaddr_in);
  ret = getpeername(newfd, (struct sockaddr *) &data_peer, &addrlen);
  checkError(ret, "getpeername");
  
  TcpCx* cx = new TcpCx(newfd);
  try {
    cx->remote_port_ = data_peer.sin_port;
    cx->remote_ip_ = data_peer.sin_addr.s_addr;
    cx->setTimeout();
    cx->setSocketOptions();
  } catch (const NetError&) {
    delete cx;
    throw;
  }
  return cx;
}

bool        TcpCx::poll(int timeout)
{
  struct pollfd pf[1] = { { fd, POLLIN, 0 } };

  int ret = ::poll(pf, 1, timeout);
  if (ret < 0 && errno != EINTR)
    throw NetError("poll");
  if (ret <= 0)
    return false;
  return true;
}

void        TcpCx::print(std::ostream& os) const
{
  for (int i = 3; i >= 0; i--)
    {
      os << ((remote_ip_ & (0xFF << (i << 3))) >> (i << 3));
      if (i != 0)
        os << ".";
    }
  os << ":" << remote_port_;
}
