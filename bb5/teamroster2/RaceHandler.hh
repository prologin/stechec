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
#ifndef RACEHANDLER_HH_
#define RACEHANDLER_HH_

#include    <xercesc/sax/HandlerBase.hpp>
#include    <vector>
#include    "Race.hh"

XERCES_CPP_NAMESPACE_USE


class RaceHandler : public HandlerBase
{

public :
	static std::vector<Race> vRaces_;
    static char*        BBversion_;   
    
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
    RaceHandler();
    ~RaceHandler();

    // -----------------------------------------------------------------------
    //  Handlers for the SAX DocumentHandler interface
    // -----------------------------------------------------------------------
    void startElement(const XMLCh* const name, AttributeList& attributes);
    void characters(const XMLCh* const chars, const unsigned int length);
    void endElement(const XMLCh* const name);
  
    // -----------------------------------------------------------------------
    //  Implementations of the SAX ErrorHandler interface
    // -----------------------------------------------------------------------
    void warning(const SAXParseException& exc);
    void error(const SAXParseException& exc);
    void fatalError(const SAXParseException& exc);

    private:
    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    Race*                  currentRace_;
    Position*              currentPos_;
    const XMLCh*           chars_;
};

#endif /*RACEHANDLER_HH_*/
