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

#include <stdarg.h>
#include <cassert>

inline NetError::NetError(const std::string& msg)
  : Exception(msg)
{
  // FIXME: use strerror_r
  if (errno != 0)
    msg_ = msg_ + ": " + strerror(errno);
}

inline FileIOError::FileIOError(const std::string& msg)
  : Exception(msg)
{
  // FIXME: use strerror_r
  if (errno != 0)
    msg_ = msg_ + ": " + strerror(errno);
}


inline Cx::Cx()
  : fd(-1), method(CX_RW)
{
}

inline Cx::~Cx()
{
  if (fd >= 0)
    closesocket(fd);
}

inline int   Cx::getFd() const
{
  return fd;
}

inline void  Cx::print(std::ostream&) const
{
}

inline std::ostream& operator<< (std::ostream& os, const Cx& cx)
{
  cx.print(os);
  return os;
}

inline int Min(int a, int b)
{
  return a > b ? b : a;
}
