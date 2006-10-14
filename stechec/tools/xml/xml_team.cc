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
#include "xml/xml_internal.hh"

BEGIN_NS(xml)

void XMLTeam::switchToTeamGlobals()
{
  xml_internal_->setCurrentNode("team");
}

bool XMLTeam::switchToPlayer(int index)
{
  std::ostringstream os;
  os << index;
  return xml_internal_->setCurrentNodeByAttr("player", "number", os.rdbuf()->str());
}

const char* XMLTeam::getExpectedRoot() const
{
  return "team";
}
  
END_NS(xml)
