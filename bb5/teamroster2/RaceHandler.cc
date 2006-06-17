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
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>
#include<xercesc/util/XMLString.hpp>
#include <xercesc/sax/AttributeList.hpp>

#include <iostream.h>
#include <vector.h>
#include <string.h>
#include <stdlib.h>

#include "Race.hh"
#include "RaceHandler.hh"
#include "Helper-classes.hh"

vector<Race> RaceHandler::vRaces_;
char* RaceHandler::BBversion_;   
    
RaceHandler::RaceHandler()
{
}

RaceHandler::~RaceHandler()
{
}


// ---------------------------------------------------------------------------
//  RaceHandler: Overrides of the SAX DocumentHandler interface
// ---------------------------------------------------------------------------
void RaceHandler::startElement(const XMLCh* const name, AttributeList&  attributes)
{
	// Initialise buffer for the element characters parsing.
 	//buf_ = "";
	
	// A new race is found
	if (strcmp(DualString(name).asCString(), "races") == 0)
 	{
  		RaceHandler::BBversion_ = 
  			xercesc::XMLString::transcode(attributes.getValue("BBversion"));

 	} else if (strcmp(DualString(name).asCString(), "race") == 0)
 	{
  		Race* r = new Race();
		r->setName(xercesc::XMLString::transcode(attributes.getValue("name")));

		// Store pointer on the current race parsed.
 		currentRace_ = r;
 	}
 	else if (strcmp(DualString(name).asCString(), "apothecary") == 0)
 	{
 		bool canUse = strcmp(DualString(attributes.getValue("use")).asCString(), "true") == 0;
 		currentRace_->setApothecaryUse(canUse);
 	}
 	else if (strcmp(DualString(name).asCString(), "reroll") == 0)
 	{
 		long cost = atol(DualString(attributes.getValue("cost")).asCString());
 		currentRace_->setRerollCost(cost);
 		
 		int qty = atoi(DualString(attributes.getValue("qty")).asCString());
 		currentRace_->setRerollQuantity(qty);
 	}
 	else if (strcmp(DualString(name).asCString(), "position") == 0)
 	{
 	  // Parse position element like above: 
	  // <position qty="16" title="Linewomam" cost="50000" ma="6" st="3" ag="3" av="7" skills="dodge" normal="g" double="asp" />
 
 	  Position* pos = new Position();

	  pos->setQuantity(atoi(DualString(attributes.getValue("qty")).asCString()));
	  pos->setTitle(xercesc::XMLString::transcode(attributes.getValue("title")));
	  pos->setCost(atol(DualString(attributes.getValue("cost")).asCString()));
	  pos->setMovementAllowance(atoi(DualString(attributes.getValue("ma")).asCString()));
	  pos->setStrength(atoi(DualString(attributes.getValue("st")).asCString()));
	  pos->setAgility(atoi(DualString(attributes.getValue("ag")).asCString()));
	  pos->setArmourValue(atoi(DualString(attributes.getValue("av")).asCString()));
	  pos->setSkills(xercesc::XMLString::transcode(attributes.getValue("skills")));
	  pos->setNormalSkills(xercesc::XMLString::transcode(attributes.getValue("normal")));
	  pos->setDoubleSkills(xercesc::XMLString::transcode(attributes.getValue("double")));
	  
	  //add this position to the current race.
	  currentRace_->addPosition(*pos);
    	}
 }

void RaceHandler::characters(const XMLCh* const chars, const unsigned int length)
{

}

void RaceHandler::endElement(const XMLCh* const name)
{
  	if (strcmp(DualString(name).asCString(), "background") == 0)
 	{
 		// Store the race's background 
// 		currentRace_->setBackground(buf_);
 	}
	else if (strcmp(DualString(name).asCString(), "race") == 0)
 	{
 		// Store the current race
  		RaceHandler::vRaces_.push_back(*currentRace_);
 	}
}


// ---------------------------------------------------------------------------
//  RaceHandler: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void RaceHandler::error(const SAXParseException& e)
{
    XERCES_STD_QUALIFIER cerr << "\nError at file " << e.getSystemId()
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << e.getMessage() << XERCES_STD_QUALIFIER endl;
}

void RaceHandler::fatalError(const SAXParseException& e)
{
    XERCES_STD_QUALIFIER cerr << "\nFatal Error at file " << e.getSystemId()
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << e.getMessage() << XERCES_STD_QUALIFIER endl;
}

void RaceHandler::warning(const SAXParseException& e)
{
    XERCES_STD_QUALIFIER cerr << "\nWarning at file " << e.getSystemId()
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << e.getMessage() << XERCES_STD_QUALIFIER endl;
}

