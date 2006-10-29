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

#include <cerrno>
#include "netpoll.hh"

template <typename T>
NetPoll<T>::NetPoll(ElementList& elt_list, int to)
  : elt_list_(elt_list),
    timeout_(to),
    pifi_size_(3),
    lock_set_(false)
{
  pifi_ = new struct pollfd[3];
}

template <typename T>
NetPoll<T>::~NetPoll()
{
  delete[] pifi_;
}

template <typename T>
void NetPoll<T>::setLock(pthread_mutex_t* lock)
{
  lock_ = lock;
  lock_set_ = true;
}

template <typename T>
void NetPoll<T>::poll()
{
  int elt_size = elt_list_.size();
  ElementIter it;
  int i;

  // elt_list_ size may have changed outside.
  if (pifi_size_ < elt_size)
    {
      pifi_size_ = elt_size * 2;
      delete[] pifi_;
      pifi_ = new struct pollfd[pifi_size_];
    }

  // Fill pifi_ with file descriptors.
  for (it = elt_list_.begin(), i = 0; i < elt_size; ++it, ++i)
    {
      pifi_[i].fd = (*it)->getFd();
      pifi_[i].events = POLLIN;
      pifi_[i].revents = 0;

      // Client may have been killed outside of this poll function.
      if (pifi_[i].fd == -1)
	{
	  delete elt_list_[i];
          elt_list_[i] = elt_list_[elt_size - 1];
	  elt_list_.erase(elt_list_.end() - 1);
	  return;
	}
    }

  if (lock_set_)
    pthread_mutex_unlock(lock_);
  int nb_ready = ::poll(pifi_, elt_size, timeout_);
  if (lock_set_)
    pthread_mutex_lock(lock_);

  if (nb_ready < 0 && errno != EINTR)
    throw NetError("Poll");
  if (nb_ready == 0)
    return;

  // Look for fd that are ready, put them on the beginning of the list.
  for (i = 0; i < elt_size; ++i)
    if ((pifi_[i].revents & POLLIN) && elt_list_[i]->recvReady())
      {
	// This client is removed from the list.
	delete elt_list_[i];
	elt_list_[i] = elt_list_[elt_size - 1];
	elt_list_.erase(elt_list_.end() - 1);
	pifi_[i] = pifi_[elt_size - 1];
	i--;
	elt_size--;
      }
}
