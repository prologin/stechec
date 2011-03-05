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

#include "Api.hh"

// from Api.cc
extern Api* api;

extern "C" bool api_need_retirer_ko()
{
  return api->need_retirer_ko();
}

extern "C" bool api_retirer_ko(position u)
{
  return api->retirer_ko(u);
}

extern "C" bool api_mon_tour()
{
  return api->mon_tour();
}

extern "C" void api_send_actions()
{
  return api->sendActions();
}
