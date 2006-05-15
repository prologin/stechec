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

#include "StechecServer.hh"

StechecServer::StechecServer(GameData* game)
  : g_(game)
{
  s_ = reinterpret_cast<Server*>(this);
}
  
StechecServer::StechecServer(GameData* game, Server* server)
  : g_(game),
    s_(server)
{
}

StechecServer::~StechecServer()
{
}

void        StechecServer::SendToAll(const StechecPkt& data) 
{
  StechecServer* s = reinterpret_cast<StechecServer*>(s_);
  s->commit_.push_back(data);
}

void        StechecServer::SendToAll(int type, int player_id, int nbarg, ...)
{
  va_list l;
  StechecPkt data(type, player_id);
  int nb_arg_base = data.getNbArg();
  data.setNbArg(nb_arg_base + nbarg);
  
  va_start(l, nbarg);
  while (nbarg--)
    data.arg[nb_arg_base++] = va_arg(l, int);
  va_end(l);
  StechecServer* s = reinterpret_cast<StechecServer*>(s_);
  s->commit_.push_back(data);
}

CommandList&        StechecServer::getCommitList()
{
  StechecServer* s = reinterpret_cast<StechecServer*>(s_);
  return s->commit_;
}

void StechecServer::cleanCommitList()
{
  StechecServer* s = reinterpret_cast<StechecServer*>(s_);
  s->commit_.clear();
}
