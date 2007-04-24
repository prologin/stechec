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

#include "StechecGameData.hh"
#include "StechecApi.hh"

StechecApi::StechecApi(GameData* gameData, Client* c)
  : BaseApi<CRules>(NULL), // workaround
    g_(gameData),
    c_(c)
{
}

StechecApi::~StechecApi()
{
}

CommandList& StechecApi::getCommitList()
{
  return commit_;
}


void StechecApi::setClientRules(CRules* rules)
{
  rules_ = rules;
}

void StechecApi::doEndTurn()
{
  rules_->endTurn();
}

bool StechecApi::switchTeam(int team_id)
{
  if (!isViewer())
    {
      WARN("Coach is not allowed to switch team.");
      return false;
    }
  StechecGameData* g = reinterpret_cast<StechecGameData*>(g_);
  if (team_id < 0)
    team_id = -1;
  if (team_id >= g->getNbPlayer())
    return false;
  g->id_ = team_id;
  teamSwitched();
  return true;
}


//
// C interface to common api functions used by all contests.
//
extern "C" int api_state_is_end(void* api)
{
  StechecApi* stechec_api = reinterpret_cast<StechecApi*>(api);
  if (stechec_api->getState() == GS_END)
    return 1;
  return 0;
}

extern "C" int api_state_is_playturn(void* api)
{
  StechecApi* stechec_api = reinterpret_cast<StechecApi*>(api);
  if (stechec_api->getState() == GS_PLAYTURN)
    return 1;
  return 0;
}

extern "C" void api_do_end_turn(void* api)
{
  StechecApi* stechec_api = reinterpret_cast<StechecApi*>(api);
  stechec_api->doEndTurn();
}
