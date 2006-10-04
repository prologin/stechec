/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2006 The TBT Team.
** 
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
** 
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
** 
** The TBT Team consists of people listed in the `AUTHORS' file.
*/

#include "tools.hh"
#include "client_cx.hh"
#include "Api.hh"
#include "interface.hh"

//
// Console client main() function.
//
extern "C" int run(xml::XMLConfig* cfg, Api* api, ClientCx* client_cx)
{
  bool use_readline = true;
  try {
    cfg->switchClientSection();
    cfg->getAttr<int>("redirection", "stdin");
    use_readline = false;
  } catch (...) {
  }

  CmdLineInterface i(cfg, api, client_cx, use_readline);

  i.hello();
  i.init();
  i.run();

  return 0;
}
