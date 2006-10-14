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
#include "xml/xml_config.hh"
using namespace xml;

int main()
{
  Log l(1);

  XMLConfig cfg;
  cfg.parse(std::string(getenv("srcdir")) + "/config.xml");

  assert(cfg.getAttr<int>("server", "debug", "verbose") == 2);
  assert(cfg.getAttr<bool>("server", "debug", "printloc") == false);

  cfg.setAttr("server", "debug", "verbose", 6);
  assert(cfg.getAttr<int>("server", "debug", "verbose") == 6);
  cfg.setData("server", "lalal", std::string("lili"));

  // default attr
  assert(cfg.getAttr<std::string>("client", "connect", "val") == std::string("network"));

  // non-existing section
  cfg.switchClientSection(3);
  assert(cfg.getAttr<int>("client", "connect", "port") == 25151);

  // existing section
  cfg.switchClientSection(1);
  assert(cfg.getAttr<int>("client", "debug", "verbose") == 4);
  assert(cfg.getAttr<bool>("client", "debug", "printloc") == true);
  assert(cfg.getAttr<std::string>("client", "connect", "host") == std::string("localhost"));

  return 0;
}
