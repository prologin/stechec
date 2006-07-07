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
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax/AttributeList.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>

#include "RaceHandler.hh"
#include "Helper-classes.hh"

std::vector<Race> RaceHandler::vRaces_;
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
	// A new race is found
	if (strcmp(DualString(name).asCString(), "races") == 0)
 	{
  		RaceHandler::BBversion_ = 
  			xercesc::XMLString::transcode(attributes.getValue("BBversion"));

 	} else if (strcmp(DualString(name).asCString(), "race") == 0)
 	{
  		Race* r = new Race();
		r->setName(xercesc::XMLString::transcode(attributes.getValue("name")));
        r->setEmblem(xercesc::XMLString::transcode(attributes.getValue("emblem")));

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
 	  currentPos_ = new Position();

	  currentPos_->setQuantity(atoi(DualString(attributes.getValue("qty")).asCString()));
	  currentPos_->setTitle(xercesc::XMLString::transcode(attributes.getValue("title")));
	  currentPos_->setCost(atol(DualString(attributes.getValue("cost")).asCString()));
	  currentPos_->setMovementAllowance(atoi(DualString(attributes.getValue("ma")).asCString()));
	  currentPos_->setStrength(atoi(DualString(attributes.getValue("st")).asCString()));
	  currentPos_->setAgility(atoi(DualString(attributes.getValue("ag")).asCString()));
	  currentPos_->setArmourValue(atoi(DualString(attributes.getValue("av")).asCString()));
	  currentPos_->setNormalSkills(xercesc::XMLString::transcode(attributes.getValue("normal")));
	  currentPos_->setDoubleSkills(xercesc::XMLString::transcode(attributes.getValue("double")));
      currentPos_->setDisplay(xercesc::XMLString::transcode(attributes.getValue("display")));
    	}
    else if (strcmp(DualString(name).asCString(), "skill") == 0)
    {
        char* tmp = xercesc::XMLString::transcode(attributes.getValue("name"));
        char* newSkill = new char[strlen(tmp) + 1];
        sprintf(newSkill,"%s",tmp);
        currentPos_->addSkill(newSkill);
    }      
 }

void RaceHandler::characters(const XMLCh* const chars, const unsigned int length)
{
    chars_ = chars;
}

void RaceHandler::endElement(const XMLCh* const name)
{
  	if (strcmp(DualString(name).asCString(), "background") == 0)
 	{
 		// Store the race's background 
 		currentRace_->setBackground(xercesc::XMLString::transcode(chars_));
 	}
	else if (strcmp(DualString(name).asCString(), "race") == 0)
 	{
 		// Store the current race
  		RaceHandler::vRaces_.push_back(*currentRace_);
 	}
    else if (strcmp(DualString(name).asCString(), "position") == 0)
    {
        //add the current position to the current race.
        currentRace_->addPosition(*currentPos_);
    }
}


