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
using namespace xml;

int main()
{
  Log l(1);

  XMLConfig cfg;
  cfg.parse(std::string(getenv("srcdir")) + "/tbtrc.xml");

  cfg.switchSection("server");
  assert(cfg.getAttr<int>("debug", "verbose") == 2);
  assert(cfg.getAttr<bool>("debug", "printloc") == false);

  cfg.setAttr("debug", "verbose", 6);
  assert(cfg.getAttr<int>("debug", "verbose") == 6);
  cfg.setData("lalal", std::string("lili"));
  
  cfg.switchSection("client");
  assert(cfg.getAttr<int>("debug", "verbose") == 3);
  assert(cfg.getAttr<bool>("debug", "printloc") == true);

  return 0;
}
