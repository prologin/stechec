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
  Log l(-1);

  // try a malformated xml file.
  XMLFormation p;
  try {
    p.parse(std::string(getenv("srcdir")) + "/formations-bad.xml");
  } catch (const XMLError& e) {
    goto next;
  }
  return 1;

 next:

  // try a bad root node
  try {
    XMLTeam t;
    t.parse(std::string(getenv("srcdir")) + "/formations.xml");
  } catch (const XMLError& e) {
    goto nextone;
  }
  return 1;

 nextone:
  
  // try a non existing file.
  try {
    XMLFormation p2;
    p2.parse("non-existing.xml");
  } catch (const XMLError& e) {
    return 0;
  }
  return 1;
}
