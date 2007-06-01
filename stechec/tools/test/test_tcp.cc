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

#include "logger/log.hh"
#include "datatfs/TcpCx.hh"

void client()
{
  TcpCx cx;

  cx.connect("localhost", 23111);
  if (!cx.poll(2000)) {
    ERR("connection failed");
  }
  LOG1("connected");

  for (int i = 0; i < 5; i++)
    cx.send(Packet(-1));
  LOG1("5 packet sended");

  sleep(1);
  if (!cx.poll())
    LOG1("nothing to read?");
  cx.receive();
}

void server()
{
  TcpCx cxl;
  TcpCx* cx;

  cxl.listen(23111);

  LOG1("listening, now poll for ready connection");
  if (!cxl.poll(5000)) {
    ERR("no client after 5 seconds");
    return;
  }
  cx = cxl.accept();
  LOG1("accepted");

  LOG1("write 1 pkt");
  cx->send(Packet(-1));
  
  LOG1("read 5 pkts");
  for (int i = 0; i < 5; i++) {
    if (!cx->poll(500)) {
      ERR("read1 timeout");
      return;
    }
    Packet* p = cx->receive();
    printf(".");
  }
  LOG1(" done.");

}


int main(int argc, char **argv)
{
  Log l(8);

  if (argc < 2 || (strcmp(argv[1], "client") && strcmp(argv[1], "server")))
    {
      ERR("usage: %1 <server|client>", argv[0]);
      return 0;
    }

  try {
    if (!strcmp(argv[1], "client"))
      client();
    if (!strcmp(argv[1], "server"))
      server();
  } catch (const NetError& e) {
    ERR(e.what());
  }
  
  return 0;
}
