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

#ifndef RULES_LOADER_HH_
# define RULES_LOADER_HH_

# include "BaseRules.hh"
# include "ClientCx.hh"

/*!
** @brief Helper class that load/initialize/unload rules.
** @ingroup client
*/
class RulesLoader
{
public:
  RulesLoader();
  ~RulesLoader();

  BaseCRules*   loadRules(ConfFile* cfg_file, const ConfSection* cfg);
  bool          initRules(ClientCx* ccx);
  const struct RuleDescription& getModuleDesc();

private:
  Library       lib_rules_;
  BaseCRules*   base_rules_;
};

#endif /* !RULES_LOADER_HH_ */
