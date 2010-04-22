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

static Action* act_from_pkt(int type, StechecPkt* pkt)
{
  struct position p;

  switch(type)
  {
  case ACT_DEGUISEMENT:
    return new ActionDeguisement(pkt->arg[1], pkt->arg[2], (type_unite)pkt->arg[3]);
  case ACT_BANZAI:
    return new ActionBanzai(pkt->arg[1], pkt->arg[2]);
  case ACT_SOIN:
    return new ActionSoin(pkt->arg[1], pkt->arg[2]);
  case ACT_PACIFISME:
    return new ActionPacifisme(pkt->arg[1]);
  case ACT_DEPLACER:
    p.x = pkt->arg[3];
    p.y = pkt->arg[4];
    return new ActionDeplacer(pkt->arg[1], pkt->arg[2], p);
  default:
    LOG1("Unhandled action type : %1", type);
    abort();
  }
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

    // apply the actions
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
