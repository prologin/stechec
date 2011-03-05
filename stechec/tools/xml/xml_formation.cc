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
#include "xml/xml_internal.hh"

BEGIN_NS(xml)

XMLFormation::XMLFormation()
  : formation_id_(-1)
{
}

bool XMLFormation::switchFormationId(int formation_id)
{
  formation_id_ = formation_id;
  return true;
}

Position XMLFormation::getPos(int player_id)
{
  std::string srow, scol;
  std::ostringstream sid, splayer_id;
  sid << formation_id_;
  splayer_id << player_id;
  if (!xml_internal_->getFormationAttr(sid.str(), splayer_id.str(), srow, scol))
    {
      PRINT_AND_THROW(XMLError, "Can't get position for player `"
		      << player_id << "' in xml file `" << filename_ << "'.");
    }
  std::istringstream isrow(srow), iscol(scol);
  Position pos;
  isrow >> pos.row;
  iscol >> pos.col;
  return pos;
}

void XMLFormation::parse(const std::string& filename)
{
  XML::parse(filename);
  xml_internal_->setCurrentNode("formations");
}

const char* XMLFormation::getExpectedRoot() const
{
  return "formations";
}

END_NS(xml)
