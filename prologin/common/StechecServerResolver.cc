/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#include "StechecServerResolver.hh"

StechecServerResolver::StechecServerResolver(GameData* game, Server* server)
  : StechecServer(game, server)
{
}

StechecServerResolver::~StechecServerResolver()
{
}

void StechecServerResolver::ApplyResolverPriv(CommandList& cmdList)
{
  CommandListRef filteredList[MAX_STECHEC_PKT_TYPE];

  // Sort cmdLine by token, thus it can be easily solved by the ApplyResolver.
  for (CommandList::iterator it = cmdList.begin();
       it != cmdList.end(); ++it)
    {
      assert(it->type >= 0 && it->type < MAX_STECHEC_PKT_TYPE);
      filteredList[it->type].push_back(&*it);
      LOG2("Resolver::Apply " << *it);
    }

  ApplyResolver(filteredList);
}
