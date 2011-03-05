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

#include "misc/Conf.hh"
#include "ChampionLoader.hh"
#include <unistd.h>
#include <fcntl.h>

ChampionLoader::ChampionLoader()
{
}

void ChampionLoader::loadLibrary(int argc,
                                 char** argv,
                                 const ConfSection& cfg)
{
  std::string path = cfg.getValue<std::string>("path");
  if (path.length() && path[path.length() - 1] != '/')
    path += "/";

  setenv("CHAMPION_PATH", path.c_str(), 1);
  champion_.open(path + cfg.getValue<std::string>("library"));

  // Special Ocaml
  typedef void (*caml_init)(char**);
  caml_init caml_startup = (caml_init) champion_.getSymbol("caml_startup", false);
  if (caml_startup != NULL)
    caml_startup(argv);

  // Special Haskell
  typedef void (*haskell_init)(int, char**);
  haskell_init haskell_startup = (haskell_init) champion_.getSymbol("haskell_startup", false);
  if (haskell_startup != NULL)
    haskell_startup(argc, argv);
  
  run_fun_ = (run_func_t)(champion_.getSymbol("run"));
}

int ChampionLoader::run(ConfFile* cfg_file,
                        BaseCRules* base_rules,
                        ClientCx* client_cx)
{
  return run_fun_(cfg_file, base_rules->getApi(), client_cx);
}
