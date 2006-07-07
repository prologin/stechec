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
#include "TeamHandler.hh"
#include "Helper-classes.hh"

Team* TeamHandler::team_;


TeamHandler::TeamHandler()
{
}

TeamHandler::~TeamHandler()
{
}

// ---------------------------------------------------------------------------
//  TeamHandler: Overrides of the SAX DocumentHandler interface
// ---------------------------------------------------------------------------
void TeamHandler::startDocument() 
{
    if (TeamHandler::team_ != NULL)
    {
        delete TeamHandler::team_;
    }
}
    
void TeamHandler::startElement(const XMLCh* const name, AttributeList&  attributes)
{   
    currentNode_ =  xercesc::XMLString::transcode(name);
    
    // The team is found
    // <team race="Orc" BBversion="5" emblem="orc.jpg" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="team.xsd">
    if (strcmp(DualString(name).asCString(), "team") == 0)
    {
        std::string BBversion = xercesc::XMLString::transcode(attributes.getValue("BBversion"));
        if (BBversion.compare(RaceHandler::BBversion_) != 0)
        {
            //FIXME: generate an exception here...
        }    
         
        std::string raceStr = xercesc::XMLString::transcode(attributes.getValue("race"));
        std::string emblem = xercesc::XMLString::transcode(attributes.getValue("emblem"));
        
        // Search race
        Race* race = NULL;
        for (unsigned int i=0; i<RaceHandler::vRaces_.size(); i++)
        {
            if (strcmp(raceStr.c_str(), RaceHandler::vRaces_[i].getName()) == 0)
            {
                race = &RaceHandler::vRaces_[i];
                break;
            }
        }
        
        // create a new team
        TeamHandler::team_ = new Team(race);
        // Change team emblem
        TeamHandler::team_->setEmblem(emblem.c_str());
 //        std::cout << "Team found of race :" << TeamHandler::team_->getRace()->getName() << std::endl;
  
    } else if (strcmp(DualString(name).asCString(), "player") == 0) 
    {
        currentPlayer_ = new Player(TeamHandler::team_);
        currentPlayer_->setName(xercesc::XMLString::transcode(attributes.getValue("name")));
        currentPlayer_->setPosition(xercesc::XMLString::transcode(attributes.getValue("position")));
        
        currentPlayerNumber_ = atoi(xercesc::XMLString::transcode(attributes.getValue("number")));
    }
  }

void TeamHandler::characters (const XMLCh* const chars, const unsigned int length)
{
    if (currentNode_ == "name")
    {
        TeamHandler::team_->setName(xercesc::XMLString::transcode(chars));
    }
    else if (currentNode_ == "coach")
    {
        TeamHandler::team_->setHeadCoach(xercesc::XMLString::transcode(chars));
    }
    else if (currentNode_ == "bank")
    {
        TeamHandler::team_->setBank(atol(xercesc::XMLString::transcode(chars)));
    }
    else if (currentNode_ == "reroll")
    {
        TeamHandler::team_->setReroll(atoi(xercesc::XMLString::transcode(chars)));
    }
    else if (currentNode_ == "fanfactor")
    {
        TeamHandler::team_->setFanFactor(atoi(xercesc::XMLString::transcode(chars)));
    }
    else if (currentNode_ == "assistant")
    {
        TeamHandler::team_->setAssistantCoach(atoi(xercesc::XMLString::transcode(chars)));
    }
    else if (currentNode_ == "cheerleader")
    {
        TeamHandler::team_->setCheerleader(atoi(xercesc::XMLString::transcode(chars)));
    }
    else if (currentNode_ == "apothecary")
    {
        TeamHandler::team_->setApothecary(atoi(xercesc::XMLString::transcode(chars)));
    }
    else if (currentNode_ == "ma")
    {
        currentPlayer_->setMovementAllowance(atoi(xercesc::XMLString::transcode(chars)));
    }
    else if (currentNode_ == "st")
    {
        currentPlayer_->setStrength(atoi(xercesc::XMLString::transcode(chars)));
    }
    else if (currentNode_ == "ag")
    {
        currentPlayer_->setAgility(atoi(xercesc::XMLString::transcode(chars)));
    }
    else if (currentNode_ == "av")
    {
        currentPlayer_->setArmourValue(atoi(xercesc::XMLString::transcode(chars)));
    }
    else if (currentNode_ == "cost")
    {
        // Ignored. The cost will be automatically computed.
    }
    else if (currentNode_ == "inj")
    {
        currentPlayer_->setInjuries(xercesc::XMLString::transcode(chars));
    }
    else if (currentNode_ == "com")
    {
        currentPlayer_->setCompletions(atoi(xercesc::XMLString::transcode(chars)));
    }
    else if (currentNode_ == "td")
    {
        currentPlayer_->setTouchDowns(atoi(xercesc::XMLString::transcode(chars)));
    }
    else if (currentNode_ == "int")
    {
        currentPlayer_->setInterceptions(atoi(xercesc::XMLString::transcode(chars)));
    }
    else if (currentNode_ == "cas")
    {
        currentPlayer_->setCasualties(atoi(xercesc::XMLString::transcode(chars)));
    }
    else if (currentNode_ == "mpv")
    {
        currentPlayer_->setMostValuablePlayer(atoi(xercesc::XMLString::transcode(chars)));
    }
    else if (currentNode_ == "spp")
    {
         // Ignored. The cost will be automatically computed.
    }
    else if (currentNode_ == "skill")
    {
    //    std::cout << "add skill:" << xercesc::XMLString::transcode(chars) <<":" << std::endl;
        currentPlayer_->addSkill(xercesc::XMLString::transcode(chars));
    }
}

void TeamHandler::endElement(const XMLCh* const name)
{
    currentNode_ = "";
    
    if (strcmp(DualString(name).asCString(), "player") == 0)
    {
        TeamHandler::team_->setPlayer(currentPlayerNumber_, currentPlayer_);
    }
}

