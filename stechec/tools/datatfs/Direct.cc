/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006, 2007 Prologin
*/

#include <time.h>
#include <cstring>
#include "tools.hh"
#include "Direct.hh"

DirectCx* DirectCx::inst_client_[2] = { NULL, NULL };
DirectCx* DirectCx::inst_server_ = NULL;

static pthread_mutex_t mutex_client;
static pthread_mutex_t mutex_server;

DirectCx::DirectCx(eCxWhoIAm w)
{
  w_ = w;
  switch (w_)
    {
    case CXD_IS_SERVER:
      inst_server_ = this;
      break;
    case CXD_IS_COACH1:
      inst_client_[0] = this;
      break;
    case CXD_IS_COACH2:
      inst_client_[1] = this;
      break;
    }
}

DirectCx::~DirectCx()
{
  switch (w_)
    {
    case CXD_IS_SERVER:
      inst_server_ = NULL;
      break;
    case CXD_IS_COACH1:
      inst_client_[0] = NULL;
      break;
    case CXD_IS_COACH2:
      inst_client_[1] = NULL;
      break;
    }
}

void DirectCx::lock(bool lock_server)
{
  if (lock_server)
    pthread_mutex_lock(&mutex_server);
  else
    pthread_mutex_lock(&mutex_client);
}

void DirectCx::unlock(bool unlock_server)
{
  if (unlock_server)
    pthread_mutex_unlock(&mutex_server);
  else
    pthread_mutex_unlock(&mutex_client);
}

void DirectCx::sendData(unsigned char* data, unsigned size)
{
//  struct timespec tm = { 0, 50 * 1000000 };
  int retry_count = 100;

  Packet* dup_pkt = reinterpret_cast<Packet*>(new char[size]);
  memcpy(dup_pkt, data, size);

  lock(w_ != CXD_IS_SERVER);
  if (w_ == CXD_IS_SERVER)
    {
      // hack: wait that both client are initialized, otherwise
      // it will crash.
      while (inst_client_[0] == NULL || inst_client_[1] == NULL)
        {
          //nanosleep(&tm, NULL);
          if (retry_count-- < 0)
            throw NetError("Client isn't reachable\n");
        }
//       if (pkt->client_id > 0)
//         inst_client_[pkt->client_id - 1]->queue_.push(dup_pkt);
//       else
//         {
//           inst_client_[0]->queue_.push(dup_pkt);
//           inst_client_[1]->queue_.push(dup_pkt);
//         }
    }
  else
    {
      while (inst_server_ == NULL)
        {
         // nanosleep(&tm, NULL);
          if (retry_count-- < 0)
            throw NetError("Server isn't reachable\n");
        }
//       inst_server_->queue_.push(dup_pkt);
    }
  unlock(w_ != CXD_IS_SERVER);
}

int DirectCx::recvData(bool)
{
  Packet* pkt;

  lock(w_ == CXD_IS_SERVER);
//   assert(!queue_.empty());
//   pkt = queue_.front();
//   queue_.pop();
  unlock(w_ == CXD_IS_SERVER);
  return 0;
}

bool DirectCx::pollInternal(int* timeout)
{
//  struct timespec tm = { 0, 30 * 1000000 };
  bool have_data;

  while (*timeout >= 0)
    {
      lock(w_ == CXD_IS_SERVER);
//       have_data = !queue_.empty();
      unlock(w_ == CXD_IS_SERVER);
      if (have_data)
        return true;
//      nanosleep(&tm, NULL);
      *timeout -= 30;
    }
  return false;
}
