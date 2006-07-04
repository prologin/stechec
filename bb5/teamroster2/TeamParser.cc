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
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

#include <iostream>
#include <vector>
#include "TeamParser.hh"
#include "TeamHandler.hh"

TeamParser::TeamParser()
{
}

TeamParser::~TeamParser()
{
}

void TeamParser::parse(const char* filename)
{
    // Initialize the XML4C2 system
    try
    {
         XMLPlatformUtils::Initialize();
    }

    catch (const XMLException& toCatch)
    {
         std::cerr << "Error during initialization! :\n"
              << toCatch.getMessage() << std::endl;
    }

    //
    //  Create a SAX parser object. Then, according to what we were told on
    //  the command line, set it to validate or not.
    //
    SAXParser* parser = new SAXParser;

//FIXME: Make schema validation
    parser->setValidationScheme(SAXParser::Val_Auto);
    parser->setDoNamespaces(false);
    parser->setDoSchema(true);
    parser->setValidationSchemaFullChecking(true);

    //
    //  Create the handler object and install it as the document and error
    //  handler for the parser-> Then parse the file and catch any exceptions
    //  that propogate out
    //
    TeamHandler handler;
    try
    {
        parser->setDocumentHandler(&handler);
        parser->setErrorHandler(&handler);
        parser->parse(filename);
        
        //printRaces();
    }
    catch (const OutOfMemoryException&)
    {
         std::cerr << "OutOfMemoryException" << std::endl;
    }
    catch (const XMLException& toCatch)
    {
         std::cerr << "\nAn error occurred\n  Error: "
             << toCatch.getMessage()
             << "\n" << std::endl;
    } 
     
    //
    //  Delete the parser itself.  Must be done prior to calling Terminate, below.
    //
    delete parser;

    // And call the termination method
    XMLPlatformUtils::Terminate();  

}
