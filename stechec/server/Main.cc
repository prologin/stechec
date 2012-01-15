/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006, 2007 Prologin
*/

#include <signal.h>
#include <memory>

#include "misc/Conf.hh"
#include "Server.hh"

typedef std::unique_ptr<server::Server> server_ptr;
static server_ptr server_;

static void get_config(int argc, char** argv, ConfFile& cfg_file);
static void interrupt_handler(int);

int main(int argc, char** argv)
{
  Log main_log(3);
  ConfFile cfg_file(argc, argv);
  ConfSection* cfg_server;

  try {
    get_config(argc, argv, cfg_file);
    cfg_server = cfg_file.getSection("server");
  } catch (const ConfException& e) {
    ERR("Error loading configuration: %1", e.what());
    return 69;
  }

  try {
    main_log.setVerboseLevel(cfg_server->getValue<int>("server_verbose"));
    main_log.setPrintLoc(cfg_server->getValue<bool>("server_verbose_location"));
    server_ = server_ptr(new server::Server(cfg_file, cfg_server));

    // Catch ctl-c, to properly shutdown the server.
    struct sigaction act;
    if (sigemptyset(&act.sa_mask) == -1)
      WARN("Could not empty sigaction mask");
    act.sa_handler = &interrupt_handler;
    act.sa_flags = 0;
    if (sigaction(SIGINT, &act, NULL) == -1)
      WARN("Could not place SIGINT handler");

    server_->run();
  } catch (const NetError& e) {
    ERR("Uncatched network error: %1", e);
    return 51;
  } catch (const ConfException& e) {
    ERR("%1", e);
    return 52;
  } catch (const LibraryError&) {
    return 53;
  } catch (...) {
    ERR("Uncatched error");
    return 54;
  }

  return 0;
}

// Get configuration
static void get_config(int argc, char** argv,
                       ConfFile& cfg_file)
{
  ConfSection::RegList def;
  ConfSection* cfg;

  // Set default values (lower priority)
  def["listen_port"] = "25150";
  def["nb_spectator"] = "0";
  def["persistent"] = "false";
  def["wait_timeout"] = "45";
  def["log"] = "";
  def["verbose"] = "2";
  def["verbose_location"] = "false";
  def["server_verbose"] = "2";
  def["server_verbose_location"] = "false";
  def["start_game_timeout"] = "30";
  def["config"] = "";

  // Set command line options (higher priority)
  static const struct ConfCmdLineOpt opt[] = {
    { 'h', "help", 0, "show this help" },
    { 'v', "version", 0, "show version" },
    { 'c', "config", 1, "use this configuration file" },
    { 'l', "listen_port", 1, "listen_port" },
    { 's', "nb_spectator", 1, "nb_spectator" },
    { 'p', "persistent", 0, "can accept multiple games" },
    { 'w', "wait_timeout", 1, "wait_timeout" },
    { 'o', "log", 1, "log" },
    { 'd', "verbose", 1, "verbose" },
    { 'e', "verbose_location", 1, "verbose_location" },
    { 'f', "server_verbose", 1, "server_verbose" },
    { 'g', "start_game_timeout", 1, "start_game_timeout" },
    { 0, 0, 0, 0 }
  };

  // Parse and handle basic command line options
  cfg = cfg_file.parseCmdLine("server", opt);
  
  if (cfg->exist("help") && cfg->getValue<bool>("help"))
    {
      std::cout << "usage: " << argv[0] << " [options]\n";
      std::cout << "options:\n";
      cfg_file.printHelpOption(opt);

      // also print specific rules command line, if provided
      if (argc > 2)
        {
          try {
            Library l(argv[2]);
            const struct ConfCmdLineOpt* opt;
            opt = (struct ConfCmdLineOpt*)(l.getSymbol("rules_cmd_opt"));
            std::cout << argv[2] << " options:\n";
            cfg_file.printHelpOption(opt);
          } catch (const LibraryError&) {
          }
        }

      exit(0);
    }  

  if (cfg->exist("version") && cfg->getValue<bool>("version"))
    {
      std::cout << "Stechec Server v" PACKAGE_VERSION << std::endl;
      std::cout << "Copyright (C) 2006, 2007 Prologin." << std::endl;
      exit(0);
    }

  // Parse config file, if any
  if (cfg->exist("config") && cfg->getValue<std::string>("config") != "")
    cfg_file.parse(cfg->getValue<std::string>("config"));

  // Add default entries
  cfg_file.setDefaultEntries("server", def);
}

static void interrupt_handler(int)
{
  server_->shutdown();
}
