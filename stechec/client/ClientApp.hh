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

#ifndef CLIENTAPP_H_
# define CLIENTAPP_H_

# include "xml/xml_config.hh"
# include "ClientCx.hh"
# include "RulesLoader.hh"

class BaseCRules;

/*!
** @brief A generic client entry point (immediately after main).
**
** Regroup commonly used code.
*/
class ClientApp
{
public:
  ClientApp(int argc, char** argv);
  virtual ~ClientApp();

  //! @brief Enter in the main outer client loop.
  int runApp();

  //! @brief Load a champion as an external dynamic library ad run it.
  int runChampion();

private:
  Log log_client_; // FIXME: its destructor must be called _last_.

protected:
  virtual void showHelp(const char* prgname);
  virtual void showVersion();
  virtual int showMenu();
  virtual int onPlay(bool replay) = 0;

  xml::XMLConfig cfg_;
  BaseCRules* rules_;
  ClientCx ccx_;
  RulesLoader rules_loader_;

private:
  void parseOption();
  void parseConfig();
  void setOpt();
  
  int argc_;
  char** argv_;

  char* config_file_; ///< Optional configuration file to load.
  int client_gid_;    ///< Client game id, as stored in meta-data.
};


#endif /* !CLIENTAPP_H_ */
