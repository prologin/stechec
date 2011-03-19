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

#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <cerrno>

#include <cstdio>
#include <cstring>

#include "misc/os_wrapper.hh"
#include "misc/ExceptionDefine.hh"
#include "TcpCx.hh"

TcpCx::TcpCx()
  : remote_port_(0),
    remote_ip_(0)
{
#ifdef WIN32
  static bool init_ok = false;
  if (!init_ok)
    {
      WSADATA info;
      if (WSAStartup(MAKEWORD(2, 0), &info))
        throw NetError("Could not start WSA");
      init_ok = true;
    }
#endif
}

TcpCx::TcpCx(int fd)
  : remote_port_(0),
    remote_ip_(0)
{
  fd_ = fd;
}

TcpCx::~TcpCx()
{

}

void TcpCx::setSocketOptions()
{
  int tcpnodelay_flag = 1;

  // turn off Nagle's algorithm to speed up transfert
  if (setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY,
		 (char *) &tcpnodelay_flag, sizeof(int)))
    throw NetSysError("setsockopt(TCP_NODELAY)");

  // set non-blocking if possible
  if (fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL, NULL) | O_NONBLOCK) < 0)
    throw NetSysError("fnctl(NONBLOCK)");

  // ignore sigpipe (BSD specific? not always implemented).
#ifdef SO_NOSIGPIPE
  if (setsockopt(fd_, SOL_SOCKET, SO_NOSIGPIPE,
		 (char *) &tcpnodelay_flag, sizeof(int)))
    throw NetSysError("setsockopt(SO_NOSIGPIPE)");
#endif
}

void TcpCx::connect(const char* host, int port)
{
  struct sockaddr_in    sin;
  struct hostent*       hp;

  if ((hp = gethostbyname(host)) == NULL)
    throw NetError(std::string(host) + ": host not found.");

  memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);
  sin.sin_family = hp->h_addrtype;
  sin.sin_port = htons(port);

  if (fd_ >= 0)
    closesocket(fd_);

  if ((fd_ = socket(hp->h_addrtype, SOCK_STREAM, 0)) < 0)
    throw NetSysError("socket");

  setSocketOptions();

  if (::connect(fd_, (struct sockaddr *)&sin, sizeof (sin)))
    {
      if (errno != EINPROGRESS)
	throw NetSysError("connect");
    }

  // Socket is non-blocking, connection may be not made now
  state_ = CX_CONNECTING;
}

void TcpCx::listen(int port)
{
  struct sockaddr_in sin;
  int value = 1;

  if (fd_ >= 0)
    closesocket(fd_);

  if ((fd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    throw NetSysError("socket");

  if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, (const char*)&value, sizeof (value)))
    throw NetSysError("setsockopt(SO_REUSEADDR)");

  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
  if (bind(fd_, (struct sockaddr *)&sin, sizeof (sin)))
    throw NetSysError("bind");

  if (::listen(fd_, 50))
    throw NetSysError("listen");

  state_ = CX_LISTENING;
}

TcpCx* TcpCx::accept()
{
  struct sockaddr_in data_addr;
  struct sockaddr_in data_peer;
  socklen_t addrlen = sizeof (struct sockaddr_in);
  int newfd;

  if ((newfd = ::accept(fd_, (struct sockaddr *) &data_addr, &addrlen)) < 0)
    throw NetSysError("accept");

  addrlen = sizeof (struct sockaddr_in);
  if (getpeername(newfd, (struct sockaddr *) &data_peer, &addrlen))
    throw NetSysError("getpeername");

  TcpCx* cx = new TcpCx(newfd);
  cx->remote_port_ = data_peer.sin_port;
  cx->remote_ip_ = data_peer.sin_addr.s_addr;
  try {
    cx->setSocketOptions();
  } catch (const NetSysError&) {
    delete cx;
    throw;
  }

  cx->state_ = CX_RW;
  return cx;
}

bool TcpCx::pollInternal(int* timeout)
{
  struct timeval tv;
  fd_set fdset;
  int ret;

  tv.tv_sec = *timeout / 1000;
  tv.tv_usec = (*timeout % 1000) * 1000;
  FD_ZERO(&fdset);
  FD_SET(fd_, &fdset);

  if (state_ == CX_CONNECTING)
    {
      ret = select(fd_ + 1, NULL, &fdset, NULL, &tv);
      if (ret > 0)
        {
          socklen_t addrlen;
          struct sockaddr_in data_peer;
          int val;

          // check if connection was successful
          addrlen = sizeof (val);
          if (getsockopt(fd_, SOL_SOCKET, SO_ERROR, (char *)&val, &addrlen))
            throw NetSysError("getsockopt");
          if (val != 0)
            THROW(NetError, "connect: " << strerror(val));

          addrlen = sizeof (struct sockaddr_in);
          if (getpeername(fd_, (struct sockaddr *) &data_peer, &addrlen))
            throw NetSysError("getpeername");
          remote_port_ = data_peer.sin_port;
          remote_ip_ = data_peer.sin_addr.s_addr;
        }
    }
  else
    ret = select(fd_ + 1, &fdset, NULL, NULL, &tv);
  if (ret < 0 && errno != EINTR)
    throw NetError("poll");

  // FIXME: may check exceptions ?

  *timeout = tv.tv_sec * 1000 + tv.tv_usec / 1000;
  if (ret <= 0)
    return false;
  return true;
}

int TcpCx::recvData(bool use_exception)
{
  int recv_size = BUF_RECV_SIZE - buff_recv_tail_;
  int cnt;

  cnt = ::recv(fd_, buff_recv_ + buff_recv_tail_, recv_size, MSG_DONTWAIT);
  if (cnt == -1 && errno == EINTR)
    return 0;
  if (cnt == 0)
    {
      state_ = CX_CLOSED;
      strcpy(last_error_, "Connection closed on other side");
      if (use_exception)
        throw NetError(last_error_);
      else
        return -1;
    }
  if (cnt == -1)
    {
      state_ = CX_CLOSED;
      snprintf(last_error_, 63, "recv: %s", strerror(errno));
      if (use_exception)
        throw NetSysError("recv");
      else
        return -1;
    }
  return cnt;
}

void TcpCx::sendData(unsigned char* data, unsigned size)
{
  struct timespec delay;
  int retry_count = 0;
  unsigned snd_bytes = 0;
  int snd_size;
  int cnt;

  while (snd_bytes < size)
    {
      snd_size = std::min(size - snd_bytes, 16384u);
      cnt = ::send(fd_, data + snd_bytes, snd_size, MSG_NOSIGNAL| MSG_DONTWAIT);

      if (cnt == -1)
        {
          if (errno == EINTR)
            continue;
          if (errno == EPIPE)
            {
              state_ &= ~CX_WRITE;
              return;
            }
          if (errno == EAGAIN)
            {
              delay.tv_sec = 0;
              delay.tv_nsec = 1000 * 1000;
              nanosleep(&delay, NULL);
              if (retry_count++ > 5)
                {
                  state_ = CX_CLOSED;
                  throw NetSysError("send: EAGAIN raised too much");
                }
              continue;
            }
        }

      snd_bytes += cnt;
    }
}

void TcpCx::print(std::ostream& os) const
{
  unsigned long int addr;

  addr = htonl(remote_ip_);
  os << ((addr & 0xff000000) >> 24) << "."
     << ((addr & 0x00ff0000) >> 16) << "."
     << ((addr & 0x0000ff00) >> 8) << "."
     << (addr & 0x000000ff);
  os << ":" << ntohs(remote_port_);
}
