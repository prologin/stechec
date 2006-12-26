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

#ifdef HAVE_SDL
# include <SDL.h>
#endif // !HAVE_SDL

#include "start_arbiter.hh"
#include "client_cx.hh"
#include "rules_loader.hh"
#include "champion_loader.hh"

ClientApp::ClientApp(int argc, char** argv)
  : argc_(argc),
    argv_(argv),
    log_client_(5),
    config_file_(NULL),
    client_gid_(1)
{

}

ClientApp::~ClientApp()
{
  clean_arbiter();
}

void ClientApp::showHelp(const char* prgname)
{
  std::cout << "usage: " << prgname << " [client_id] [config-file]\n";
}

void ClientApp::showVersion()
{
  std::cout << "Stechec Generic client v" PACKAGE_VERSION << "\n";
  std::cout << "Copyright (C) 2005, 2006 Prologin.\n";
}

// Very basic command line manager. We don't need anything more powerful.
void ClientApp::parseOption()
{
  if (argc_ >= 2 && (!strcmp(argv_[1], "--help") || !strcmp(argv_[1], "-h")))
    {
      showHelp(argv_[0]);
      exit(0);
    }
  if (argc_ >= 2 && (!strcmp(argv_[1], "--version") || !strcmp(argv_[1], "-v")))
    {
      showVersion();
      exit(0);
    }

  if (argc_ >= 2)
    {
      char* endptr;
      int client_gid = strtol(argv_[1], &endptr, 10);
      if (*endptr == 0)
        {
          client_gid_ = client_gid;
          if (argc_ >= 3)
            config_file_ = argv_[2];
        }
      else
        config_file_ = argv_[1];
    }
}

// Parse xml configuration file.
void ClientApp::parseConfig()
{
  try {
    cfg_.parse(config_file_);
  } catch (const xml::XMLError& e) {
    ERR("Sorry, I can't go further without a working configuration file...");
    exit(3);
  }
}

// Set some basic settings based on XML config file.
void ClientApp::setOpt()
{
  cfg_.switchClientSection(client_gid_);
  log_.setVerboseLevel(cfg_.getAttr<int>("client", "debug", "verbose"));
  log_.setPrintLoc(cfg_.getAttr<bool>("client", "debug", "printloc"));
}


int ClientApp::runChampion()
{
  // Load the UI/Champion.
  ChampionLoader cl;
  cl.loadLibrary(argc, argv, cfg);

  // Give the hand to the UI/Champion.
  return cl.run(cfg, r, &ccx);
}


// Main entry
int ClientApp::runApp()
{
  int ret_value = 0;

  parseOption();
  parseConfig();
  setOpt();

  // If the champion/UI is the first to use printf, it will segv :/
  // so 'initialize' it.
  fprintf(stderr, "%s", "");
  fprintf(stdout, "%s", "");
  srand(time(NULL));

  try {
    while (ret_value == 0)
      {
	if ((ret_value = showMenu()))
	  break;

	// FIXME: doesn't work right now, let it false.
	// bool replay_log = cfg.getAttr<bool>("client", "mode", "replay");
	bool replay_log = false;

	// Optionally start a thread for the arbiter, if needed.
	start_arbiter(cfg);

	// Load rules.
	BaseCRules* r;
	RulesLoader rl;
	r = rl.loadRules(cfg);

	// Connect to the arbiter now, if it was asked. Thus, errors while
	// loading champion could be reported to the server.
	ClientCx ccx(r, opt.client_gid);
	if (cfg.getAttr<bool>("client", "connect", "connect_on_startup") && !replay_log)
	  {
	    if (!ccx.connect(cfg))
	      return 1;
	  }

	if (replay_log)
	  {
	    // Log replay. Open the file, and play.
	    ccx.openLog(cfg.getAttr<std::string>("client", "mode", "file"));
	    ret_value = onPlay(true);
	  }
	else
	  {
	    // Normal game.
	    ret_value = onPlay(false);
	  }
	clean_arbiter();
      }
  } catch (const NetError& e) {
    ERR("Network: %1", e);
    return 50;
  } catch (const FileIOError& e) {
    ERR("File IO: %1", e);
    return 51;
  } catch (const LibraryError&) {
    return 52;
  } catch (const xml::XMLError& e) {
    ERR("%1", e.what());
    return 53;
  } catch (...) {
    ERR("Catching unknown exception, rethrowing...");
    throw;
  }

  if (ret_value < 0)
    ret_value = 0;
  LOG1("Have a nice day !");
  return ret_value;
}


#if 0
        // Call a hook in rules, before UI is loaded.
        if (!rl.initRules(&ccx))
          {
            LOG3("Sorry, rules initialization failed, cannot go further.");
            goto end;
          }
#endif
