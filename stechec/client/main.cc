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

#include "tools.hh"

#ifdef HAVE_SDL
# include <SDL.h>
#endif // !HAVE_SDL

#include "start_arbiter.hh"
#include "client_cx.hh"
#include "rules_loader.hh"
#include "champion_loader.hh"

struct CmdLineOption
{
  CmdLineOption()
    : config_file(""),
      client_gid(1)
  {}

  char* config_file;  ///< Optionnal configuration file to load.
  int   client_gid;   ///< Client game id, as stored in meta-data.
};

// Very basic command line manager. We don't need anything more powerful.
static void parse_option(int argc, char** argv, CmdLineOption& opt)
{
  if (argc >= 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")))
    {
      std::cout << "usage: " << argv[0] << " [client_id] [config-file]\n";
      exit(0);
    }
  if (argc >= 2 && (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v")))
    {
      std::cout << "Stechec Generic client v" PACKAGE_VERSION << "\n";
      std::cout << "Copyright (C) 2005, 2006 Prologin.\n";
      exit(0);
    }

  if (argc >= 2)
    {
      char* endptr;
      int client_gid = strtol(argv[1], &endptr, 10);
      if (*endptr == 0)
        {
          opt.client_gid = client_gid;
          if (argc >= 3)
            opt.config_file = argv[2];
        }
      else
        opt.config_file = argv[1];
    }
}

// Parse xml configuration file.
static void parse_config(const CmdLineOption& opt, xml::XMLConfig& cfg)
{
  try {
    cfg.parse(opt.config_file);
  } catch (const xml::XMLError& e) {
    ERR("Sorry, I can't go further without a working configuration file...");
    exit(3);
  }
}

// Set some basic settings based on XML config file.
static void set_opt(const CmdLineOption& opt, xml::XMLConfig& cfg, Log& log)
{
  cfg.switchClientSection(opt.client_gid);
  log.setVerboseLevel(cfg.getAttr<int>("debug", "verbose"));
  log.setPrintLoc(cfg.getAttr<bool>("debug", "printloc"));
  cfg.switchSection("client");
}

int main(int argc, char** argv)
{
  Log log_client(5);
  xml::XMLConfig cfg;
  CmdLineOption opt;
  int ret_value = 1;

  parse_option(argc, argv, opt);
  parse_config(opt, cfg);
  set_opt(opt, cfg, log_client);

  // If the champion/UI is the first to use printf, it will segv :/
  // so 'initialize' it.
  fprintf(stderr, "%s", "");
  fprintf(stdout, "%s", "");
  srand(time(NULL)); // Asked by some candidats.

  // Optionally start a thread for the arbiter, if needed.
  start_arbiter(cfg);

  // FIXME: doesn't work right now, let it false.
//   cfg.switchClientSection();
//   bool replay_log = cfg.getAttr<bool>("mode", "replay");
//   cfg.switchSection("client");
  bool replay_log = false;

  try {
    BaseCRules* r;
    RulesLoader rl;

    // Load rules.
    r = rl.loadRules(cfg);

    // Connect to the arbiter now, if it was asked. Thus, errors while
    // loading champion could be reported to the server.
    ClientCx ccx(r, opt.client_gid);
    cfg.switchSection("client");
    if (cfg.getAttr<bool>("connect", "connect_on_startup") && !replay_log)
      {
        if (!ccx.connect(cfg))
          goto end;

        // Call a hook in rules, before UI is loaded.
        if (!rl.initRules(&ccx))
          {
            LOG3("Sorry, rules initialization failed, cannot go further.");
            goto end;
          }
      }

    // Log replay. Open the file. Other things should remain the same.
    if (replay_log)
      {
        cfg.switchClientSection();
        ccx.openLog(cfg.getAttr<std::string>("mode", "file"));
        cfg.switchSection("client");

        // Call a hook in rules, before UI is loaded.
        if (!rl.initRules(&ccx))
          {
            LOG3("Sorry, rules initialization failed, cannot go further.");
            goto end;
          }
      }

    // Load the UI/Champion.
    ChampionLoader cl;
    cfg.switchClientSection(opt.client_gid);
    cl.loadLibrary(argc, argv, cfg);

    // Give the hand to the UI/Champion.
    ret_value = cl.run(cfg, r, &ccx);

  } catch (const NetError& e) {
    ERR("Network: %1", e);
    ret_value = 51;
    goto end;
  } catch (const FileIOError& e) {
    ERR("File IO: %1", e);
    ret_value = 51;
    goto end;
  } catch (const LibraryError&) {
    ret_value = 52;
    goto end;
  } catch (const xml::XMLError&) {
    ret_value = 53;
    goto end;
  } catch (...) {
    ERR("Catching unknown exception, rethrowing...");
    clean_arbiter();
    throw;
  }

 end:
  clean_arbiter();

  if (ret_value == 0)
    LOG1("Have a nice day !");

  return ret_value;
}
