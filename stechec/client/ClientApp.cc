/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2006, 2007 Prologin
*/

// Required to trick main() function if a dynamic library will use SDL.
#ifdef HAVE_SDL
# include <SDL.h>
#endif // !HAVE_SDL

#include "tools.hh"
#include "datatfs/Cx.hh"
#include "ClientApp.hh"
#include "ChampionLoader.hh"


ClientApp::ClientApp(int argc, char** argv, const std::string&, const std::string&)
  : log_client_(5),
    cfg_file_(argc, argv),
    argc_(argc),
    argv_(argv),
    config_file_(0),
    client_gid_(1)
{
}

ClientApp::~ClientApp()
{}

void ClientApp::showHelp(const char* prgname,
                         const struct ConfCmdLineOpt* cmd_opt)
{
  std::cout << "usage: " << prgname << " [options]\n";
  std::cout << "options:\n";
  cfg_file_.printHelpOption(cmd_opt);
}

void ClientApp::showVersion()
{
  std::cout << "Stechec Generic client v" PACKAGE_VERSION << "\n";
  std::cout << "Copyright (C) 2005, 2006, 2007 Prologin.\n";
}

void ClientApp::parseConfig()
{
  ConfSection::RegList def;

  // Set default values (lower priority)
  def["server_host"] = "127.0.0.1";
  def["server_port"] = "25150";
  def["game_uid"] = "42";
  def["spectator"] = "false";
  def["rules"] = "";
  def["path"] = "";
  def["library"] = "";
  def["memory"] = "0";
  def["time"] = "0";
  def["time_reserve"] = "2500";
  def["valgrind"] = "false";
  def["gdb"] = "false";
  def["verbose"] = "3";
  def["verbose_location"] = "false";
  def["log"] = "";
  def["log_replay"] = "false";
  def["config"] = "";
  def["id"] = "1";

  // Used only run.sh, but may be left here. ignore it.
  def["stdin_redir"] = "";
  def["stdout_redir"] = "";

  // Set command line options (higher priority)
  static const struct ConfCmdLineOpt opt[] = {
    { 'h', "help", 0, "show this help" },
    { 'v', "version", 0, "show version" },
    { 'c', "config", 1, "use this configuration file" },
    { 'i', "id", 1, "client/config id" },
    { 'r', "rules", 1, "rules name" },
    { 's', "spectator", 0, "join game as a spectator" },
    { 'd', "server_host", 1, "server address" },
    { 'p', "server_port", 1, "server port" },
    { 'a', "path", 1, "libray path" },
    { 'l', "library", 1, "library name" },
    { 'g', "valgrind", 0, "use valgrind" },
    { 'b', "gdb", 0, "use gdb" },
    { 'e', "verbose", 1, "verbose level" },
    { 'o', "log", 1, "file to log name" },
    { 'm', "memory", 1, "max memory for the process" },
    { 't', "time", 1, "max time per turn" },
    { 'f', "time_reserve", 1, "time reserve" },
    { 0, 0, 0, 0 }
  };

  // Parse and handle basic command line options
  cfg_ = cfg_file_.parseCmdLine("client", opt);

  if (cfg_->exist("help") && cfg_->getValue<bool>("help"))
    {
      showHelp(argv_[0], opt);
      exit(0);
    }

  if (cfg_->exist("version") && cfg_->getValue<bool>("version"))
    {
      showVersion();
      exit(0);
    }

  client_gid_ = 1;
  if (cfg_->exist("id"))
    client_gid_ = cfg_->getValue<int>("id");

  // [client] section will refer to [client_<gid>]
  std::ostringstream os;
  os << "client_" << client_gid_;
  cfg_file_.addAlias(os.str(), "client");
  
  // Parse config file, if any
  if (cfg_->exist("config") && cfg_->getValue<std::string>("config") != "")
    cfg_file_.parse(cfg_->getValue<std::string>("config"));

  // Add default entries, superseded by command line opt.
  cfg_file_.setDefaultEntries("client", def);
  cfg_file_.parseCmdLine("client", opt);
  
  // Set some settings, that have to be set asap
  log_client_.setVerboseLevel(cfg_->getValue<int>("verbose"));
  log_client_.setPrintLoc(cfg_->getValue<bool>("verbose_location"));
}


int ClientApp::runChampion()
{
  // Load the UI/Champion.
  ChampionLoader cl;
  cl.loadLibrary(argc_, argv_, *cfg_);

  // Give the hand to the UI/Champion.
  return cl.run(&cfg_file_, rules_, &ccx_);
}

int ClientApp::showMenu()
{
  return 0;
}

// Main entry
int ClientApp::runApp()
{
  int ret_value = 0;

  try {
    parseConfig();
  } catch (const ConfException& e) {
    ERR("Error loading configuration: %1", e.what());
    return 69;
  }

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

	// Load rules.
	rules_ = rules_loader_.loadRules(&cfg_file_, cfg_);
	ccx_.setRules(rules_);
	ccx_.setClientGameId(client_gid_);

	// Try connecting, if not done yet.
	if (!ccx_.isConnected())
	  if (!ccx_.connect(*cfg_))
	    {
	      ret_value = 21;
	      break;
	    }

	// Join the game.
	if (!ccx_.join(*cfg_, rules_loader_.getModuleDesc()))
	  {
	    ret_value = 22;
	    break;
	  }

	if (replay_log)
	  {
	    // Log replay. Open the file, and play.
	    ccx_.openLog(cfg_->getValue<std::string>("log"));
	    ret_value = onPlay(true);
	  }
	else
	  {
	    // Normal game.
	    ret_value = onPlay(false);
	  }
	ccx_.disconnect(false);

      }
  } catch (const NetError& e) {
    ERR("Network: %1", e);
    return 50;
  } catch (const LibraryError&) {
    return 52;
  } catch (const Exception& e) {
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
