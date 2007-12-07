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
#include "xml/xml_formation.hh"
using namespace xml;

struct pt {
  int col, row;
} formid1[] = {
  {1, 12},
  {2, 12},
  {4, 12},
  {3, 6},
  {6, 12},
  {8, 12},
  {9, 8},
  {10, 12},
  {12, 11},
  {13, 12},
  {7, 9}
};

int main()
{
  Log l(-2);

  XMLFormation p;
  p.parse(std::string(getenv("srcdir")) + "/formations.xml");

  // try all id == 1 formations
  p.switchFormationId(1);
  for (int pid = 1; pid <= 11; pid++)
    assert(p.getPos(pid) == Position(formid1[pid - 1].row, formid1[pid - 1].col));

  // try some id == 4
  p.switchFormationId(4);
  assert(p.getPos(8) == Position(12, 11));
  assert(p.getPos(11) == Position(7, 10));

  // try some non-existing
  try {
    p.getPos(12);
    return 1;
  } catch (...) {}

  p.switchFormationId(2);
  try {
    p.getPos(-1);
    return 1;
  } catch (...) {}
  try {
    p.getPos(0);
    return 1;
  } catch (...) {}

  p.switchFormationId(5);
  try {
    p.getPos(4);
    return 1;
  } catch (...) {}

  return 0;
}
