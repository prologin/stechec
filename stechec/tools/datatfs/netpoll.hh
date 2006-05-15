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

#ifndef NETPOOL_HH_
# define NETPOOL_HH_

# include <vector>

/*!
** @ingroup tools_net
** @{
*/

/*! @brief Wrapper used to poll a whole list of connections.
**
** A list of any object (this is the parametred type) could be given
** to NetPoll. This object \c must implement the method getFd(), that
** give a file descriptor.
**
** @author victor
** @date 30/01/2006
** @see Cx
*/
template <typename T>
class NetPoll
{
public:
  typedef typename std::vector<T> ElementList;
  typedef typename ElementList::iterator ElementIter;

  //! @brief Construct a NetPoll object.
  //! @param elt_list A list of element to which this NetPoll is bound,
  //!    whose lenght and content can vary on the time.
  //! @param to Poll timeout, in milliseconds (-1 to wait indefinitely).
  NetPoll(ElementList& elt_list, int to = 0);
  //! @brief NetPoll destructor.
  ~NetPoll();

  //! @brief Wait that one or more file descriptor given
  //! @return The number of file descriptors ready for reading, or 0 if
  //!       timeout elapsed before. elt_list_ is reordered, so that elements
  //!       that are ready to receive are placed first.
  //! @note This function is _not_ thread-safe ! Take care to not modify
  //!       elt_list_ while in this function.
  //! @throw NetError Thrown on any network error.
  int poll();

private:
  ElementList&                elt_list_;
  int                        timeout_;
  struct pollfd*        pifi_;
  int                     pifi_size_;
};

// !ingroup tools_net
//! @}

# include "netpoll.hxx"

#endif /* !NETPOOL_HH_ */
