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

#ifndef CHAMPION_LOADER_HH_
# define CHAMPION_LOADER_HH_

# include "BaseCRules.hh"
# include "ClientCx.hh"

/*!
** @addtogroup client
** @{
*/

/*!
** @brief Helper class that load/run an UI.
*/
class ChampionLoader
{
public:
  ChampionLoader();

  void loadLibrary(int argc, char** argv, xml::XMLConfig& cfg);

  int run(xml::XMLConfig& cfg,
          BaseCRules* base_rules,
          ClientCx* client_cx);

private:
  Library champion_;

  typedef int (*run_func_t)(xml::XMLConfig*, Api*, ClientCx*);
  run_func_t run_fun_;
  Api* api_;
};

//! @}

#endif // !CHAMPION_LOADER_HH_
