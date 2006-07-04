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
#ifndef TEAMHANDLER_HH_
#define TEAMHANDLER_HH_

#include    <xercesc/sax/HandlerBase.hpp>
#include    <vector>
#include    <string>
#include    "common/Team.hh"
#include    "common/Player.hh"

XERCES_CPP_NAMESPACE_USE

class TeamHandler: public HandlerBase
{
public:
    static Team* team_;

    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
	TeamHandler();
	virtual ~TeamHandler();
    // -----------------------------------------------------------------------
    //  Handlers for the SAX DocumentHandler interface
    // -----------------------------------------------------------------------
    void startDocument();
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
    std::string    currentNode_;
    Player*        currentPlayer_;
    int            currentPlayerNumber_;
};

#endif /*TEAMHANDLER_HH_*/
