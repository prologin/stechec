/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2010 Prologin
*/

#include "Actions.hh"
#include "ServerResolver.hh"

#include <cstdlib>
#include <map>

ServerResolver::ServerResolver(GameData* game, Server* server)
  : StechecServerResolver(game, server)
{
}

typedef std::map<int, std::pair<int, StechecPkt*> > pktmap;
void ServerResolver::ApplyResolver(CommandListRef cmdList[])
{
  pktmap packets;

  for (int type = 0; type < LAST_MSG; ++type)
  {
    for (CommandListRef::iterator it = cmdList[type].begin();
         it != cmdList[type].end(); ++it)
    {
      packets[(*it)->arg[0]] = std::make_pair(type, *it);
    }
  }

  g_->packets.resize(0);
  g_->packets.reserve(packets.size());
  for (pktmap::iterator it = packets.begin();
       it != packets.end(); ++it)
  {
    std::vector<int> data;
    data.push_back(it->second.first);
    for (int i = 0; i < MAX_ARG; ++i)
      data.push_back(it->second.second->arg[i]);

    g_->packets.push_back(data);
    switch (it->second.first){
    case RETIRER_KO:
      g_->retirer_ko( it->second.second->arg[0]);
      break;
    case RETRECIR:
      abort();
    default:
      // apply the action
      Action* act = act_from_pkt(it->second.first,
				 it->second.second);
      try {
	act->verifier(g_);
	g_->appliquer_action(act);
      } catch (erreur err) {
	LOG1("Action error : %1", err);
	abort();
      }
    }
  }
}
