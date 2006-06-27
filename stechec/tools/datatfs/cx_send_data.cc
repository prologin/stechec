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

#include <fcntl.h>
#include <sys/stat.h>

#include "tools.hh"
#include "cx.hh"

int                Cx::finalizeSndTransfert(int cntr, int cntw, int snd_bytes)
{
  if (cntw == -1 && errno == EAGAIN)
    {
      errno = 0;
      THROW(NetError, "Transfer timeout (" << DEFAULT_NETCX_TO
            << " seconds, " << snd_bytes << " bytes transferred)");
    }

  if (cntw <= 0)
    THROW(NetError, "Write error");

  if (cntr < 0)
    THROW(FileIOError, "File input error");

  return 0;
}

int                Cx::sendHeader(unsigned size)
{
  char            data_size[HEADER_SIZE] = {0};
  unsigned        snd_bytes = 0;
  int             cnt = -1;

  sprintf(data_size, "VODKAx");

  int *idata_size = (int *)&data_size[6];
  *idata_size = htonl(size);
  
  while (snd_bytes < HEADER_SIZE)
    {
      cnt = ::send(fd, data_size, HEADER_SIZE - snd_bytes, MSG_NOSIGNAL);
      if (cnt <= 0)
        finalizeSndTransfert(0, cnt, snd_bytes);
      snd_bytes += cnt;
    }

  if (snd_bytes != HEADER_SIZE)
    THROW(NetError, "Can't send header");
  
  return 0;
}

/*
** copy content of filefd (opened file) to netfd
*/
void                Cx::sendFile(const char* filename)
{
  if (method == CX_RO)
    return;
  
#ifndef WIN32

  int                cnt = -1, cntw = 1;
  int                snd_bytes = 0;
  char                buf[BUFF_SIZE];
  struct stat        st;
  off_t                file_size;
  int                netfd = fd;
  int                filefd = 0;

  filefd = open(filename, O_RDONLY);
  if (!filefd)
    THROW(FileIOError, "Error while opening `" << filename << "'");
  
  if (fstat(filefd, &st) < 0)
    {
      close(filefd);
      THROW(FileIOError, "Error on `" << filename << "'");
    }
  file_size = st.st_size;

  try {
    sendHeader(file_size);
  } catch (const NetError& e) {
    close(filefd);
    throw;
  }

  while ((cnt = read(filefd, buf, BUFF_SIZE)) > 0
         && (cntw = write(netfd, buf, cnt)) == cnt)
    {
      snd_bytes += cnt;
    }
  close(filefd);
  finalizeSndTransfert(cnt, cntw, snd_bytes);

#endif
}

/*
** copy content of data to netfd
*/
void                Cx::send(const char *data, unsigned size)
{
  if (method == CX_RO)
    return;
  
  unsigned      snd_bytes = 0;
  int           snd_size = Min(size, BUFF_SIZE);
  int           cnt = 1;

  sendHeader(size);

  while (snd_bytes < size && (cnt = ::send(fd, data + snd_bytes,
                                           snd_size,
                                           MSG_NOSIGNAL))
         == snd_size)
    {
      snd_bytes += snd_size;
      snd_size = Min(size - snd_bytes, BUFF_SIZE);
    }
  finalizeSndTransfert(0, cnt, snd_bytes);
}

// Send a Packet.
void                Cx::send(const Packet* pkt)
{
  if (method == CX_RO)
    return;

  int*           dst_data;
  const int*     src_data;
  unsigned       size = pkt->data_size;

  assert(size < PACKET_MAX_SIZE);
  dst_data = reinterpret_cast<int*>(buff_send_);
  src_data = reinterpret_cast<const int*>(pkt);

  for (unsigned i = 0; i < size / sizeof(int); i++)
    dst_data[i] = htonl(src_data[i]);

  send(buff_send_, size);
}
