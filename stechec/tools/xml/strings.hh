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

#ifndef XML_STRINGS_HH
# define XML_STRINGS_HH

/*!
** @file stechec/tools/xml/strings.hh
** @ingroup tools_xml
*/

# include <string>
# include <cassert>
# include <xercesc/util/XMLUniDefs.hpp>
# include <xercesc/util/XMLString.hpp>

namespace xml
{
  //! @ingroup tools_xml
  inline std::string xml2str(const XMLCh* xmlstr)
  {
    assert(xmlstr != 0);

    char* cstr = xercesc::XMLString::transcode(xmlstr);
    assert(cstr != 0);
    
    std::string ret(cstr);
    xercesc::XMLString::release(&cstr);

    return ret;
  }

  //! @ingroup tools_xml
  inline std::ostream& operator<< (std::ostream& os, const XMLCh* xmlstr)
  {
    return os << xml2str(xmlstr);
  }
}

#endif // ! VCSN_XML_STRINGS_HH
