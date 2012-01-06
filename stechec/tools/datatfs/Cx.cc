/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2007 Prologin
*/

#include <cassert>
#include <cerrno>
#include <arpa/inet.h>
#include <cstring>

#include "misc/os_wrapper.hh"
#include "Cx.hh"

NetError::NetError(const std::string& msg)
  : Exception(msg)
{
}

NetSysError::NetSysError(const std::string& msg)
  : NetError(msg)
{
  // FIXME: use strerror_r
  if (errno != 0)
    msg_ = msg_ + ": " + strerror(errno);
}


Cx::Cx()
  : fd_(-1),
    state_(CX_CLOSED),
    batch_send_(false),
    buff_send_tail_(0),
    buff_recv_head_(0),
    buff_recv_tail_(0)
{
  last_error_[0] = 0;
}

Cx::~Cx()
{
  if (fd_ >= 0)
    closesocket(fd_);
}

const std::string Cx::getLastError() const
{
  if (last_error_[0] == 0)
    return "No error";
  return last_error_;
}


void Cx::begin()
{
  batch_send_ = true;
}

void Cx::commit()
{
  sendData(buff_send_, buff_send_tail_);
  buff_send_tail_ = 0;
  batch_send_ = false;
}

void Cx::send(const Packet& pkt)
{
  unsigned char*	dst_data;
  int*			dst_data_int;
  const int*		src_data;
  unsigned		size = pkt.data_size;

  if (!(state_ & CX_WRITE))
    return;

  // Too big.
  assert(size < PACKET_SIZE_MAX);
  // Packet size _have to_ be multiple of 4.
  assert((size & 3) == 0);

  dst_data = buff_send_ + buff_send_tail_;
  dst_data_int = reinterpret_cast<int*>(dst_data);
  src_data = reinterpret_cast<const int*>(&pkt);
  for (unsigned i = 0; i < size / sizeof (int); i++)
    dst_data_int[i] = htonl(src_data[i]);

  if (!batch_send_)
    {
      sendData(buff_send_, size);
    }
  else
    {
      buff_send_tail_ += size;
      if (buff_send_tail_ >= BUF_SEND_SIZE - PACKET_SIZE_MAX)
	{
	  // buffer almost full, time to send
	  sendData(buff_send_, buff_send_tail_);
	  buff_send_tail_ = 0;
	}
    }
}

Packet* Cx::receive()
{
  unsigned int* data_int;
  unsigned data_size;
  unsigned pkt_size = 0;

  if (!(state_ & CX_READ))
    throw NetError("Read is not available on this connection");

  data_int = reinterpret_cast<unsigned int*>(buff_recv_ + buff_recv_head_);
  data_size = buff_recv_tail_ - buff_recv_head_;
  if (data_size >= sizeof (Packet))
    {
      pkt_size = ntohl(data_int[2]);
      if (pkt_size & 3)
	{
	  // Received packet must be multiple of 4.
	  throw NetError("All received packet size must be multiple of 4");
	}
    }
  if (pkt_size == 0 || pkt_size > data_size)
    {
      // You _have to_ call poll() before, and wait that this latter
      // return true.
      throw NetError("Not enough data collected, can't read");
    }

  for (unsigned i = 0; i < pkt_size / sizeof(int); i++)
    data_int[i] = ntohl(data_int[i]);
  buff_recv_head_ += pkt_size;
  return reinterpret_cast<Packet*>(data_int);
}

// Arrange read buffer, before fetching data
// Called from poll(), or from CxPool.
// Return true if a full packet can be read, else false.
bool Cx::arrangeReadBuffer()
{
  unsigned int* data_int;
  unsigned data_size;

  // Check if at least one packet is waiting in the buffer
  data_int = reinterpret_cast<unsigned int*>(buff_recv_ + buff_recv_head_);
  data_size = buff_recv_tail_ - buff_recv_head_;
  if (data_size >= sizeof (Packet) && ntohl(data_int[2]) <= data_size)
    return true;

  // Move data to the beginning, if we went past the middle of the buffer
  if (buff_recv_head_ > BUF_RECV_SIZE - PACKET_SIZE_MAX)
    {
      memmove(buff_recv_, buff_recv_ + buff_recv_head_, data_size);
      buff_recv_head_ = 0;
      buff_recv_tail_ = data_size;
    }
  if (data_size == 0)
    {
      buff_recv_head_ = 0;
      buff_recv_tail_ = 0;
    }
  return false;
}

bool Cx::poll(int timeout)
{
  bool single_shot = false;

  assert(timeout >= 0);
  if (timeout == 0)
    single_shot = true;

  // If connecting or listening, don't play with read buffer,
  // only call internal poll.
  if (state_ & (CX_CONNECTING | CX_LISTENING))
    {
      bool ret;

      ret = pollInternal(&timeout);
      if (ret && (state_ & CX_CONNECTING))
	state_ = CX_RW; // Mark as connected
      return ret;
    }

  if (!(state_ & CX_READ))
    {
      // Please connect to some host before
      throw NetError("Can't call poll() when reading is not allowed");
    }

  while (timeout > 0 || single_shot)
    {
      if (arrangeReadBuffer())
        return true;

      if (pollInternal(&timeout))
        buff_recv_tail_ += recvData(true);

      if (single_shot)
        return arrangeReadBuffer();
    }
  return false;
}
