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

#include "misc/Conf.hh"
#include "CRules.hh"
#include "SRules.hh"
#include "Client.hh"
#include "Api.hh"
#include "ClientDiffer.hh"
#include "ClientEntry.hh"
#include "Server.hh"
#include "ServerEntry.hh"
#include "ServerResolver.hh"

extern "C" const struct RuleDescription rules_description = {
  "prolo2010",
  "Prologin 2010 final contest rules",
  MODULE_NAME,
  1,
  0
};

extern "C" BaseRules* load_client_rules(ConfFile* cfg_file)
{
  const ConfSection* cfg = cfg_file->getSection("client");
    
  GameData*             data = new GameData;
  Client*               client = new Client(data);
  Api*                  api = new Api(data, client);
  ClientDiffer*         differ = new ClientDiffer(data, client);
  ClientEntry*          clientEntryPoint = new ClientEntry(data, differ, client);

  return new CRules(cfg, data, client, api, differ, clientEntryPoint);
}

extern "C" BaseRules* load_server_rules(ConfFile* cfg_file)
{
  const ConfSection* cfg = cfg_file->getSection(MODULE_NAME);

  GameData*             data = new GameData;
  Server*               server = new Server(data);
  ServerResolver*       resolver = new ServerResolver(data, server);
  ServerEntry*          serverEntryPoint = new ServerEntry(data, server, cfg);

  return new SRules(data, server, resolver, serverEntryPoint, 2, 1);
}
