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

#include "misc/Conf.hh"
#include "RulesLoader.hh"

RulesLoader::RulesLoader()
  : base_rules_(NULL)
{
}

RulesLoader::~RulesLoader()
{
  delete base_rules_;
}

BaseCRules* RulesLoader::loadRules(ConfFile* cfg_file, const ConfSection* cfg)
{
  typedef BaseCRules *(*load_func_t)(ConfFile* cfg);

  load_func_t load_fun;
  const std::string& rules = cfg->getValue<std::string>("rules");

  if (rules == "")
    THROW(Exception, "No rules specified");
  lib_rules_.open(rules);
  load_fun = (load_func_t)(lib_rules_.getSymbol("load_client_rules"));
  return base_rules_ = load_fun(cfg_file);
}

const struct RuleDescription& RulesLoader::getModuleDesc()
{
  return *(const struct RuleDescription*)(lib_rules_.getSymbol("rules_description"));
}


// Return true if init successful.
bool RulesLoader::initRules(ClientCx* ccx)
{
  typedef bool (*init_func_t)(ClientCx*, BaseCRules*);
  init_func_t init_fun;

  // if this hook exists, jump into.
  init_fun = (init_func_t)(lib_rules_.getSymbol("hook_before_ui_load", false));
  if (init_fun != NULL)
    return init_fun(ccx, base_rules_);
  return true;
}
