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

#ifndef CLIENTAPP_H_
# define CLIENTAPP_H_

# include "misc/Conf.hh"
# include "ClientCx.hh"
# include "RulesLoader.hh"

class BaseCRules;

/*!
** @brief A generic client entry point (immediately after main).
** @ingroup client
**
** Regroup commonly used code.
*/
class ClientApp
{
public:
  //! @brief ClientApp constructor.
  //! @param argc argc from main().
  //! @param argv argv from main().
  //! @param cfg_def_file User configuration file to load, $HOME will be appended.
  //! @param cfg_def_loc Package data directory, where to find default configuration file.
  ClientApp(int argc, char** argv, const std::string& cfg_def_file, const std::string& cfg_def_loc);
  virtual ~ClientApp();

  //! @brief Enter in the main outer client loop.
  int runApp();

  //! @brief Load a champion as an external dynamic library ad run it.
  int runChampion();

private:
  Log log_client_; // FIXME: its destructor must be called _last_.

protected:
  virtual void showHelp(const char* prgname, const struct ConfCmdLineOpt* cmd_opt);
  virtual void showVersion();
  virtual int showMenu();
  virtual int onPlay(bool replay) = 0;

  ConfFile cfg_file_;   ///< configuration file
  ConfSection* cfg_;    ///< [client] conf section
  BaseCRules* rules_;
  ClientCx ccx_;
  RulesLoader rules_loader_;

private:
  virtual void parseConfig();

  int argc_;
  char** argv_;

  const char* config_file_;     ///< Optional configuration file to load.
  int client_gid_;              ///< Client game id, as stored in meta-data.
};

#endif /* !CLIENTAPP_H_ */
