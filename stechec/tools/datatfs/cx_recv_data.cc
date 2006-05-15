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
#include "tools.hh"
#include "cx.hh"

int        Cx::finalizeRcvTransfert(int cnt, int snd_bytes)
{
  if (errno == EAGAIN)
    {
      sprintf(msgerr, "Transfer timeout (%d seconds, %d bytes transferred)",
               DEFAULT_NETCX_TO, snd_bytes);
      throw NetError(msgerr);
    }

  if (cnt < 0)
    {
      sprintf(msgerr, "Input error: %s", strerror(errno));
      throw NetError(msgerr);
    }

  if (cnt == 0)
    throw NetError("Connection was unexpertly closed on other side.");

  return 0;
}

/*
** return the size of incoming file
*/
int             Cx::recvHeader(char *method)
{
  unsigned      recv_bytes = 0;
  int           cnt = -1;
  int           tok_readed;
  char          data_size_buf[HEADER_SIZE];
  unsigned      data_size = 0;
  
  while (recv_bytes < HEADER_SIZE)
    {
      cnt = recv(fd, data_size_buf + recv_bytes, HEADER_SIZE - recv_bytes, 0);
      if (cnt <= 0)
        finalizeRcvTransfert(cnt, recv_bytes);
      recv_bytes += cnt;
    }

  tok_readed = sscanf(data_size_buf, "VODKA%c", method);
  
  if (recv_bytes < HEADER_SIZE || tok_readed < 1)
    throw NetError("Bad header");

  data_size = ntohl(*(int *)&data_size_buf[6]);
  return data_size;
}

/*
** copy content of netfd to filefd
*/
void                Cx::receiveFile(const char *filedest)
{
  if (method == CX_WO)
    return;

#ifndef WIN32

  int                cnt = 1;
  int                recv_bytes = 0;
  char                buf[BUFF_SIZE];
  int                data_size;
  char                method = 0;
  int                netfd = fd;
  int                filefd = 0;

  filefd = open(filedest, O_CREAT | O_WRONLY, 0644);
  if (filefd < 0)
    throw FileIOError(filedest);

  try {
    data_size = recvHeader(&method);
  } catch (NetError e) {
    close(filefd);
    throw;
  }

  while (recv_bytes < data_size
         && (cnt = read(netfd, buf,
                        Min(data_size - recv_bytes, BUFF_SIZE))) > 0
         && write(filefd, buf, cnt) == cnt)
    {
      recv_bytes += cnt;
    }
  close(filefd);
  finalizeRcvTransfert(cnt, recv_bytes);

#endif
}

/*
** copy content of netfd to (allocated) data
*/
void*                Cx::receive(unsigned* size)
{
  if (method == CX_WO)
    return NULL;
  
  int                cnt = 1;
  int                 recv_bytes = 0;
  int                data_size;
  char                method = 0;
  int                netfd = fd;

  data_size = recvHeader(&method);
  *size = data_size;
  assert(data_size < PACKET_MAX_SIZE);
  buff_recv_[data_size] = 0;

  while (recv_bytes < data_size
         && (cnt = recv(netfd, buff_recv_ + recv_bytes,
                        Min(data_size - recv_bytes, BUFF_SIZE), 0)) > 0)
    {
      recv_bytes += cnt;
    }

  finalizeRcvTransfert(cnt, recv_bytes);
  return buff_recv_;
}

/*
** Receive a packet.
*/
Packet*         Cx::receive()
{
  if (method == CX_WO)
    return NULL;

  unsigned      size;
  int*          data;

  data = (int*) receive(&size);
  assert(size == ntohl(data[2]));

  for (unsigned i = 0; i < size / sizeof(int); i++)
    data[i] = ntohl(data[i]);

  return reinterpret_cast<Packet*>(data);
}
