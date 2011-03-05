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

#ifndef XML_FORMATION_HH_
# define XML_FORMATION_HH_

# include "xml/xml.hh"

BEGIN_NS(xml);

/*!
** @brief %XML parser/writer tailored for team formation
** document files.
** @ingroup tools_xml
**
** @todo Explain the basic document structure.
*/
class XMLFormation : public XML
{
public:
  XMLFormation();

  //! @brief Switch to a specific formation id.
  bool switchFormationId(int formation_id);
    
  //! @brief Retrieve row and col formation for all players
  //! Methods in XML aren't very useful here... :/
  //! @return false if document doesn't contain a line for requested ids.
  Position getPos(int player_id);

  virtual void parse(const std::string& filename);

protected:
  virtual const char* getExpectedRoot() const;
  int formation_id_;
};

END_NS(xml);

#endif /* !XML_FORMATION_HH_ */
