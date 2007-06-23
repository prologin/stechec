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

#ifndef CXPOOL_HH_
# define CXPOOL_HH_

# include <vector>
# include <map>
# include <utility>

# if HAVE_EPOLL
#  include <sys/epoll.h>
# else
#  include <sys/select.h>
# endif

//! @ingroup tools_net
enum ECxPollFdState {
  E_FD_READ_READY,
  E_FD_CONNECTION_PENDING,
  E_FD_CONNECTION_CLOSED
};

//! @ingroup tools_net
template <typename T>
class CxPool
{
public:
  typedef typename std::vector<std::pair<enum ECxPollFdState, T*> > EltList;
  typedef typename EltList::iterator ElIter;
  typedef typename EltList::const_iterator ConstEltIter;

  CxPool(int timeout);
  virtual ~CxPool();

  //! @brief If set, allow unlocking only during poll syscall.
  void setLock(pthread_mutex_t* lock);
  
  const EltList& poll();
  void addElt(T* elt);
  void removeElt(T* elt);
  int size() const;
  void flush();

protected:
  typedef typename std::map<int, T*> InternalEltList;
  typedef typename InternalEltList::iterator InternalEltIter;

  InternalEltList	elt_list_;

private:
  int			timeout_;
  pthread_mutex_t*	lock_;
  EltList		ready_list_;

# if HAVE_EPOLL
  int			epoll_fd_;
# else
  struct pollfd*        ev_;
  int			ev_size_;
# endif
};

# include "CxPool.hxx"

#endif /* !CXPOOL_HH_ */
