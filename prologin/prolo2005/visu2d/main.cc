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

#include "client_cx.hh"
#include "Api.hh"
#include "Gui2D.hh"

extern "C" int run(xml::XMLConfig*, Api* api, ClientCx* ccx)
{
  LOG1("QT initialization ->");

  Gui2D gui(ccx, api);
  gui.run();

  LOG1("<- QT app terminated");

  return 0;
}
