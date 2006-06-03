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

#include "Packet.hh"

void stringToPacket(int* dst, const std::string& src, unsigned max_size)
{
  unsigned i;
  unsigned src_size = src.length() > max_size - 1 ? max_size - 1 : src.length();

  memset(dst, 0, max_size);
  for (i = 0; i < src_size; i++)
    dst[i / 4] |= src[i] << ((i & 3) << 3);
}

std::string packetToString(const int* src)
{
  std::string        dst;
  int                i;
  char               dec;

  for (i = 0; src[i / 4] & (0xFF << (dec = ((i & 3) << 3))); i++)
    dst += (src[i / 4] & (0xFF << dec)) >> dec;
  return dst;
}
