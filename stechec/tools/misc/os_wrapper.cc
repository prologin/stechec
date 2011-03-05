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

# include "os_wrapper.hh"

#ifdef WIN32

void initialize_socket()
{
  static bool init_ok = false;
  if (!init_ok)
    {
      WSADATA info;
      if (WSAStartup(MAKEWORD(2, 0), &info))
        throw NetError("Could not start WSA");
      init_ok = true;
    }
}

int poll(struct pollfd* pollfds, int nfds, int timeout)
{
  TIMEVAL tval;
  tval.tv_sec  = timeout / 1000;
  tval.tv_usec = (timeout % 1000) * 1000;
  
  fd_set fds;
  FD_ZERO(&fds);
  
  int max_fd = 0;
  for (int i = 0; i < nfds; i++)
  {
    FD_SET(pollfds[i].fd, &fds);
    pollfds[i].revents = 0;
    if (pollfds[i].fd > max_fd)
      max_fd = pollfds[i].fd;  
  }
  int r;
  if ((r = select(max_fd + 1, &fds, (fd_set*) 0, (fd_set*) 0, &tval)) <= 0)
    return r;
  for (int i = 0; i < nfds; i++)
    {
      if (FD_ISSET(pollfds[i].fd, &fds))
        pollfds[i].revents = POLLIN;
    }
  return r;
}

#else

void initialize_socket()
{
}

#endif // !WIN32
