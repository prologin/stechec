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

#include "Api.hh"

// global used in interface.cc
Api* api;

Api::Api(GameData *gameData, Client* c)
  : StechecApi(gameData, c)
{
  api = this;
}

void Api::teamSwitched()
{
  // Shortcut to get the selected team for viewers.
  if (g_->getUid() >= 0)
    g_->p = &g_->players[g_->getUid()];
  else
    g_->p = NULL;
}


//
// yes... empty... everything on the header.
//
