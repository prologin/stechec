/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2010 Prologin
*/

#include "Api.hh"
#include "Contest.hh"

// from Api.cc
extern Api* api;

extern "C" bool api_mon_tour()
{
  return api->mon_tour();
}

extern "C" void api_send_actions()
{
  LOG3("internal_interface.cc  api_send_actions");
  return api->sendActions();
}
