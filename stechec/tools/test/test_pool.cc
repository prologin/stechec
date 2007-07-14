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
#include "datatfs/CxPool.hh"

#define NB_THREAD 50

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void client_run()
{
  TcpCx cx;

  cx.connect("localhost", 23111);
  if (!cx.poll(2000)) {
    ERR("connection failed");
    return;
  }
  LOG1("connected");

  for (int i = 0; i < 5; i++)
    {
      if (rand() & 0x9000)
        {
          LOG2("abort cx");
          return;
        }
      cx.send(Packet(-1));
    }
  LOG1("5 packets sended");

  sleep(1);
  if (!cx.poll())
    LOG1("nothing to read?");
  cx.receive();
}

void* client(void*)
{
  Log l(8);
  try {
    client_run();
  } catch (const NetError& e) {
    ERR(e.what());
  }

  return NULL;
}

void server()
{
  TcpCx cxl;
  TcpCx *cx[NB_THREAD];
  CxPool<Cx> pool(2000);
  int cx_nb = 0;

  cxl.listen(23111);
  pool.setLock(&lock);
  
#if 0
  LOG1("listening, now poll for ready connection");
  for (int i = 0; i < NB_THREAD; i++)
    {
      if (!cxl.poll(5000))
	{
	  ERR("no client after 5 seconds");
	  return;
	}
      cx[i] = cxl.accept();
      pool.addElt(cx[i]);
      LOG1("accepted: %1", i);
    }
#else
  LOG1("listening, accepting connections/data");
  pool.addElt(&cxl);
#endif

  
  for (int i = 0; i < NB_THREAD * 5; )
    {
      const CxPool<Cx>::EltList& ready_list(pool.poll());
      CxPool<Cx>::ConstEltIter it;
      LOG1("ready list: %1", ready_list.size());
      for (it = ready_list.begin(); it != ready_list.end(); ++it)
	{
	  int type = it->first;
	  Cx* cl = it->second;
          if (type == E_FD_CONNECTION_PENDING)
            {
              cx[cx_nb] = cxl.accept();
              pool.addElt(cx[cx_nb]);
              cx_nb++;
            }
          else if (type == E_FD_CONNECTION_CLOSED)
            {
              LOG1("XXX connection closed\n");
              pool.removeElt(cl);
              delete cl;
            }
          else
            {
              Packet* p = cl->receive();
              cl->send(*p);
              printf(".");
              i++;
            }
	}
    }
  
  LOG1(" done.");
}


int main(int argc, char **argv)
{
  Log l(8);
  pthread_t th[NB_THREAD];

  if (argc < 2 || (strcmp(argv[1], "client") && strcmp(argv[1], "server")))
    {
      ERR("usage: %1 <server|client>", argv[0]);
      return 0;
    }

  try {
    if (!strcmp(argv[1], "client"))
      {
	void* status;
	for (int i = 0; i < NB_THREAD; i++)
	  pthread_create(&th[i], NULL, client, NULL);
	for (int i = 0; i < NB_THREAD; i++)
	  pthread_join(th[i], (void **)&status);
      }
    if (!strcmp(argv[1], "server"))
      server();
  } catch (const NetError& e) {
    ERR(e.what());
  }
  
  return 0;
}
