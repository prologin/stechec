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

#include "server.hh"

// Very basic command line manager. We don't need anything more powerful.
static void parse_option(int argc, char** argv)
{
  if (argc >= 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")))
    {
      std::cout << "usage: " << argv[0] << " [config-file]" << std::endl;
      exit(0);
    }
  if (argc >= 2 && (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v")))
    {
      std::cout << "TBT Server v" PACKAGE_VERSION << std::endl;
      std::cout << "Copyright (C) 2006 The TBT Team." << std::endl;
      exit(0);
    }
}

// Parse xml configuration file.
static void parse_config(const char* opt_file, xml::XMLConfig& cfg)
{
  try {
    if (opt_file != NULL)
      cfg.parse(opt_file);
    else
      cfg.parse("");
  } catch (const xml::XMLError&) {
    ERR("Sorry, I can't go further without a working configuration file...");
    exit(3);
  }
}

// Set some basic settings based on XML config file.
static void set_opt(xml::XMLConfig& cfg, Log& log)
{
  log.setVerboseLevel(cfg.getAttr<int>("server", "server_debug", "verbose"));
  log.setPrintLoc(cfg.getAttr<bool>("server", "server_debug", "printloc"));
}

int     main(int argc, char** argv)
{
  Log   main_log(5);
  xml::XMLConfig cfg;

  parse_option(argc, argv);
  parse_config(argc >= 2 ? argv[1] : NULL, cfg);
  set_opt(cfg, main_log);

  try {
    Server s(cfg);
    s.run();
  } catch (const NetError& e) {
    ERR("Uncatched network error: %1", e);
    return 51;
  } catch (const xml::XMLError&) {
    return 52;
  } catch (const LibraryError&) {
    return 53;
  } catch (const FileIOError& e) {
    ERR("IOError: %1", e);
    return 54;
  }

  return 0;
}
