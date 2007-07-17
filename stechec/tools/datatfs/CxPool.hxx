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

#include <sys/select.h>
#include <sys/poll.h>
#include <cerrno>

#include "logger/log.hh"
#include "Cx.hh"
#include "CxPool.hh"

template <typename T>
CxPool<T>::CxPool(int timeout)
  : timeout_(timeout),
    lock_(NULL)
{
#if HAVE_EPOLL
  epoll_fd_ = epoll_create(10);
  if (epoll_fd_ < 0)
    throw NetError("Poll");
#else
  ev_size_ = 12;
  ev_ = new struct pollfd[ev_size_];
#endif
}

template <typename T>
CxPool<T>::~CxPool()
{
#if HAVE_EPOLL
  close(epoll_fd_);
#else
  delete[] ev_;
#endif
}

template <typename T>
void CxPool<T>::setLock(pthread_mutex_t* lock)
{
  lock_ = lock;
}


template <typename T>
void CxPool<T>::addElt(T* elt)
{
  int fd = elt->fd_;

  if (elt_list_.count(fd) > 0)
    WARN("elt with fd `%1' already set", fd);
  assert(fd >= 0);

  elt_list_[fd] = elt;

#if HAVE_EPOOL
  struct epoll_event ev;

  ev.events = EPOLLIN;
  ev.data.fd = fd;
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev))
    throw NetError("epoll_ctl");
#endif
}

// 'fd' must still be valid.
template <typename T>
void CxPool<T>::removeElt(T* elt)
{
  int fd = elt->fd_;

  if (elt_list_.count(fd) == 0)
    {
      WARN("trying to remove non-existing element (fd `%1')", fd);
      return;
    }
  elt_list_.erase(fd);

#if HAVE_EPOOL
  struct epoll_event ev;
  int fd = elt->getFd();

  ev.events = EPOLLIN;
  ev.data.fd = fd;
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, &ev))
    throw NetError("epoll_ctl");
#endif
}


template <typename T>
int CxPool<T>::size() const
{
  return elt_list_.size();
}

template <typename T>
void CxPool<T>::flush()
{
  elt_list_.clear();
}

template <typename T>
const typename CxPool<T>::EltList& CxPool<T>::poll()
{
  int nb_ready;

  ready_list_.clear();

#if HAVE_EPOOL
  struct epoll_event ev[32];

  pthread_mutex_unlock(lock_);
  nb_ready = epoll_wait(epoll_fd_, &ev, 32, timeout_);
  pthread_mutex_lock(lock_);

  if (nb_ready < 0 && errno != EINTR)
    throw NetError("epoll");
  if (nb_ready == 0)
    return;

  for (int i = 0; i < nb_ready; i++)
    {
      int fd = ev[i].data.fd;
      ready_list_.push_back(std::make_pair(1, elt_list_[fd]));
    }

#else

  int elt_size = elt_list_.size();
  InternalEltIter it;
  int i;

  // elt_list_ size may have changed outside.
  if (ev_size_ < elt_size)
    {
      ev_size_ = elt_size * 2;
      delete[] ev_;
      ev_ = new struct pollfd[ev_size_];
    }

  // Fill ev_ with file descriptors.
  for (it = elt_list_.begin(), i = 0; it != elt_list_.end(); ++it, i++)
    {
      T* cx = it->second;

      if ((cx->state_ & CX_READ) && cx->arrangeReadBuffer())
        {
          // There is already some data to read. Push it on ready list.
          ready_list_.push_back(std::make_pair(E_FD_READ_READY, cx));
        }
      else if (cx->state_ & (CX_READ | CX_LISTENING))
        {
          ev_[i].events = POLLIN;
          ev_[i].fd = cx->fd_;
          ev_[i].revents = 0;
        }
      else
        {
          // We only support read or listening sockets. Connecting and
          // closed sockets are not intended to enter in the pool.
          // Report it as closed, caller _has to_ remove it from list.
          ready_list_.push_back(std::make_pair(E_FD_CONNECTION_CLOSED, cx));
          elt_size--;
        }
    }

  // No need to wait if some elements are ready
  if (!ready_list_.empty())
    return ready_list_;

  if (lock_)
    pthread_mutex_unlock(lock_);
  nb_ready = ::poll(ev_, elt_size, timeout_);
  if (lock_)
    pthread_mutex_lock(lock_);

  if (nb_ready < 0 && errno != EINTR)
    throw NetSysError("poll");
  if (nb_ready == 0)
    return ready_list_;

  for (int i = 0; i < elt_size; ++i)
    if ((ev_[i].revents & POLLIN))
      {
        T* cx = elt_list_[ev_[i].fd];
        
        // Read data once from fd.
        if (cx->state_ & CX_READ)
          {
            cx->arrangeReadBuffer();
            int ret = cx->recvData(false);
            if (ret == -1)
              {
                ready_list_.push_back(std::make_pair(E_FD_CONNECTION_CLOSED,
                                                     elt_list_[ev_[i].fd]));
              }
            else
              {
                cx->buff_recv_tail_ += ret;
                if (cx->arrangeReadBuffer())
                  ready_list_.push_back(std::make_pair(E_FD_READ_READY,
                                                       elt_list_[ev_[i].fd]));
              }
          }
        else if (cx->state_ & CX_LISTENING)
          {
            ready_list_.push_back(std::make_pair(E_FD_CONNECTION_PENDING,
                                                 elt_list_[ev_[i].fd]));
          }
        else
          {
            // BUG: should not have socket closed for reading.
            WARN("bug spotted in CxPool::poll");
          }
      }

#endif

  return ready_list_;
}
