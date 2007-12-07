/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#include "tools.hh"
#include "start_arbiter.hh"

#if 0

static pthread_t th_server;
static bool thread_started = false;

// Set some basic settings based on XML config file.
static void set_opt(xml::XMLConfig& cfg, Log& log)
{
  log.setVerboseLevel(cfg.getAttr<int>("server", "debug", "verbose"));
  log.setPrintLoc(cfg.getAttr<bool>("server", "debug", "printloc"));
}

void* run_arbiter(void* data)
{
  xml::XMLConfig* cfg = static_cast<xml::XMLConfig*>(data);
  Log log_server(2);

  set_opt(*cfg, log_server);

  // FIXME: create arbiter_base.
//   GameMaster gm;
//   gm.launchStandalone();
//   while (!gm.isGameFinished())
//     gm.process();

  return NULL;
}

// Start arbiter thread.
bool start_arbiter(xml::XMLConfig& cfg)
{
  assert(thread_started == false);
  if (cfg.getAttr<std::string>("client", "connect", "val") == "standalone")
    {
      LOG2("Creating server thread");
      pthread_create(&th_server, NULL, run_arbiter, static_cast<void*>(&cfg));
      thread_started = true;
      return true;
    }
  return false;
}

// Clean arbiter thread.
void clean_arbiter()
{
  if (thread_started)
    {
      pthread_join(th_server, NULL);
      thread_started = false;
    }
}

#else

bool start_arbiter(const ConfSection& cfg)
{
  return false;
}

void clean_arbiter()
{
}

#endif
