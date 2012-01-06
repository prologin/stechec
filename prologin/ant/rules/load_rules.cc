/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2005, 2006, 2007 Prologin
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


/*!
** @brief Module description
*/
extern "C" const struct RuleDescription rules_description = {
  "ant",
  "Prologin's world: ant",
  MODULE_NAME,
  1,	// version major
  1,	// version minor
};

extern "C" const struct ConfCmdLineOpt rules_cmd_opt[] = {
  { 'm', "map", 1, "map" },
  { 'n', "nb_team", 1, "number of teams" },
  { 't', "max_turn", 1, "maximum number of turn" },
  { 0, 0, 0, 0 }
};

// parse rules specific options
const ConfSection* parse_server_rules_option(ConfFile* cfg_file)
{
  ConfSection::RegList def;

  def["map"] = "";
  def["max_turn"] = "30";
  def["nb_team"] = "1";

  cfg_file->parseCmdLine(MODULE_NAME, rules_cmd_opt, 2);
  cfg_file->setDefaultEntries(MODULE_NAME, def);
  return cfg_file->getSection(MODULE_NAME);
}


//
// In stechec v2, these objects (GameData, ClientDiffer, ...) were
// created separately by the client/server, by getting a symbol from a
// "create" function (Factory) in each file. Then, they were directly
// managed and destroyed by the client/server.
//
// Now, things are a bit more arcane. Generic client and server don't
// know anything of these objects (they are a little more high-level),
// thus Rules (which did not exist in v2) is going to do the dirty
// job.
//

extern "C" BaseCRules* load_client_rules(ConfFile* cfg_file)
{
  const ConfSection* cfg = cfg_file->getSection("client");

  GameData*     data = new GameData;
  Client*       client = new Client(data);
  Api*          api = new Api(data, client);
  ClientDiffer* differ = new ClientDiffer(data, client);
  ClientEntry*  clientEntryPoint = new ClientEntry(data, differ, client);

  // Now create a CRules object, and give him all client objects.
  return new CRules(cfg, data, client, api, differ, clientEntryPoint);
}

extern "C" BaseSRules* load_server_rules(ConfFile* cfg_file)
{
  const ConfSection* cfg = parse_server_rules_option(cfg_file);

  GameData*             data = new GameData;
  Server*               server = new Server(data);
  ServerResolver*       resolver = new ServerResolver(data, server);
  ServerEntry*          serverEntryPoint = new ServerEntry(data, server, cfg);

  // Now create a SRules object, and give him all server objects.
  return new SRules(data, server, resolver, serverEntryPoint,
                    cfg->getValue<int>("nb_team"), 1);
}
