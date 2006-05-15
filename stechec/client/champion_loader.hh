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

#ifndef CHAMPION_LOADER_HH_
# define CHAMPION_LOADER_HH_

# include "BaseCRules.hh"
# include "client_cx.hh"

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

#endif // !CHAMPION_LOADER_HH_
