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

#include "CRules.hh"
#include "SRules.hh"
#include "Client.hh"
#include "Api.hh"
#include "ClientDiffer.hh"
#include "ClientEntry.hh"
#include "Server.hh"
#include "ServerEntry.hh"
#include "ServerResolver.hh"

// see comments in stechec/ant/rules.
extern "C" BaseRules* load_client_rules(xml::XMLConfig* cfg)
{
  GameData*             data = new GameData;
  Client*               client = new Client(data);
  Api*                  api = new Api(data, client);
  ClientDiffer*         differ = new ClientDiffer(data, client);
  ClientEntry*          clientEntryPoint = new ClientEntry(data, differ, client);

  // Now create a CRules object, and give him all client objects.
  return new CRules(*cfg, data, client, api, differ, clientEntryPoint);
}

extern "C" BaseRules* load_server_rules(xml::XMLConfig* cfg)
{
  GameData*             data = new GameData;
  Server*               server = new Server(data);
  ServerResolver*       resolver = new ServerResolver(data, server);
  ServerEntry*          serverEntryPoint = new ServerEntry(data, server, *cfg);

  // Now create a SRules object, and give him all server objects.
  return new SRules(data, server, resolver, serverEntryPoint);
}
