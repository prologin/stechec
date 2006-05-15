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
#include "cx.hh"

template <typename T>
NetPoll<T>::NetPoll(ElementList& elt_list, int to)
  : elt_list_(elt_list),
    timeout_(to),
    pifi_(NULL),
    pifi_size_(0)
{
}

template <typename T>
NetPoll<T>::~NetPoll()
{
  delete[] pifi_;
}

template <typename T>
int NetPoll<T>::poll()
{
  const int elt_size = elt_list_.size();
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
    }

  int nb_ready = ::poll(pifi_, elt_size, timeout_);
  if (nb_ready < 0 && errno != EINTR)
    throw NetError("Poll");
  if (nb_ready <= 0)
    return 0;
  
  // Look for fd that are ready, put them on the beginning of the list.
  int nb_seen;
  for (i = 0, nb_seen = 0; i < elt_size && nb_seen < nb_ready; ++i)
    {
      if (pifi_[i].revents & POLLIN)
        {
          T tmp = elt_list_[i];
          elt_list_[i] = elt_list_[nb_seen];
          elt_list_[nb_seen] = tmp;
          nb_seen++;
        }
    }
  assert(nb_seen == nb_ready);
  return nb_seen;
}
