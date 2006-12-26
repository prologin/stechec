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

#ifndef RULES_LOADER_HH_
# define RULES_LOADER_HH_

# include "BaseRules.hh"
# include "client_cx.hh"

/*!
** @addtogroup client
** @{
*/

/*!
** @brief Helper class that load/initialize/unload rules.
*/
class RulesLoader
{
public:
  RulesLoader();
  ~RulesLoader();

  BaseCRules*   loadRules(xml::XMLConfig& cfg);
  bool          initRules(ClientCx* ccx);
  const struct RuleDescription& getModuleDesc();

private:
  Library       lib_rules_;
  BaseCRules*   base_rules_;
};

//! @}

#endif /* !RULES_LOADER_HH_ */
