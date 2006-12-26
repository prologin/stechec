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

#include "champion_loader.hh"

ChampionLoader::ChampionLoader()
{
}

void ChampionLoader::loadLibrary(int argc,
                                 char** argv,
                                 xml::XMLConfig& cfg)
{
  champion_.open(cfg.getData<std::string>("client", "champion"));

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

int ChampionLoader::run(xml::XMLConfig& cfg,
                        BaseCRules* base_rules,
                        ClientCx* client_cx)
{
  return run_fun_(&cfg, base_rules->getApi(), client_cx);
}
