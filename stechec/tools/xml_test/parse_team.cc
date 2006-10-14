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
#include "xml/xml_team.hh"
using namespace xml;

int main()
{
  Log l(-1);

  XMLTeam t;
  t.parse(std::string(getenv("srcdir")) + "/team.xml");

  t.switchToTeamGlobals();
  assert(t.getData<int>("reroll") == -4);
  assert(t.getData<int>("rating") == 25);
  assert(t.getData<int>("apothecary") == 0);
  assert(t.getData<std::string>("formation") == "default.xml");

  t.switchToPlayer(2);
  assert(t.getData<int>("ma") == 9);
  assert(t.getAttr<std::string>("player", "name") == "Mary-Jean");

  assert(t.getData<std::string>("skill", 0) == "block");
  assert(t.getData<std::string>("skill", 1) == "dodge");

  try {
    t.getData<std::string>("skill", 2);
    return 1;
  } catch (...) {}
    
  return 0;
}
