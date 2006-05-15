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


#include "file.hh"

FileCx::FileCx()
  : mode_(0)
{
}

FileCx::~FileCx()
{
  if (f_.is_open())
    f_.close();
}

void FileCx::open(const std::string& filename, int mode)
{
  mode_ = mode;
  switch (mode)
    {
    case CX_RO:
      f_.open(filename.c_str(), std::ios::in | std::ios::binary);
      if (!f_.is_open())
        throw FileIOError(filename.c_str());
      break;

    case CX_WO:
      f_.open(filename.c_str(), std::ios::out | std::ios::binary);
      if (!f_.is_open())
        throw FileIOError(filename.c_str());
      break;

    default:
      WARN("FileCx doesn't not support this mode");
      mode_ = 0;
    }
}


void    FileCx::send(const Packet* pkt)
{
  if (!(mode_ & CX_WO))
    return;

  int*          dst_data;
  const int*    src_data;
  unsigned      size = pkt->data_size;

  // write pkt header
  char          header[HEADER_SIZE] = {
    'V', 'O', 'D', 'K', 'A', 'p', 0, 0, 0, 0
  };
  *((int *)&header[6]) = htonl(size);
  f_.write(header, HEADER_SIZE);
  
  assert(size < PACKET_MAX_SIZE);
  dst_data = reinterpret_cast<int*>(buff_send_);
  src_data = reinterpret_cast<const int*>(pkt);

  for (unsigned i = 0; i < size / sizeof(int); i++)
    dst_data[i] = htonl(src_data[i]);

  f_.write(buff_send_, size);
}

Packet* FileCx::receive()
{
  if (!(mode_ & CX_RO))
    return NULL;

  int           tok_readed;
  unsigned      data_size = 0;
  char          header_buf[HEADER_SIZE];
  int*          data;
  char          method;

  // fetch header (magic and packet size).
  if (!f_.read(header_buf, HEADER_SIZE) || !f_.good())
    {
      throw FileIOError("Can't fetch packet header");
    }
  tok_readed = sscanf(header_buf, "VODKA%c", &method);
  if (tok_readed < 1)
    throw NetError("Bad header");
  data_size = ntohl(*(int *)&header_buf[6]);

  // read packet
  if (!f_.read(buff_recv_, data_size))
    throw FileIOError("EOF reached before end of packet");
  if (f_.bad() || f_.fail())
    throw FileIOError("Error in stream while reading packet.");

  // convert to host format
  data = (int*) buff_recv_;
  assert(data_size == ntohl(data[2]));
  for (unsigned i = 0; i < data_size / sizeof(int); i++)
    data[i] = ntohl(data[i]);

  return reinterpret_cast<Packet*>(data);
}

bool    FileCx::poll(int)
{
  if (!(mode_ & CX_RO))
    return false;

  return !f_.eof();
}
