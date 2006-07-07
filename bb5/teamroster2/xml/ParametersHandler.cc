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

#include "ParametersHandler.hh"
#include "Helper-classes.hh"

using std::vector;
using std::string;

vector<string> ParametersHandler::vSkillsGeneral_;
vector<string> ParametersHandler::vSkillsAgility_;
vector<string> ParametersHandler::vSkillsPassing_;
vector<string> ParametersHandler::vSkillsStrength_;
vector<string> ParametersHandler::vSkillsMutation_;
vector<string>* ParametersHandler::vCurrent_ = NULL;

string  ParametersHandler::BBversion_;   

ParametersHandler::ParametersHandler()
{
}

ParametersHandler::~ParametersHandler()
{
}


// ---------------------------------------------------------------------------
//  RaceHandler: Overrides of the SAX DocumentHandler interface
// ---------------------------------------------------------------------------
void ParametersHandler::startElement(const XMLCh* const name, AttributeList&  attributes)
{  
    // Store the current node name
    currentNode_ =  xercesc::XMLString::transcode(name);
    
    if (strcmp(DualString(name).asCString(), "parameters") == 0)
    {
        ParametersHandler::BBversion_ = 
            xercesc::XMLString::transcode(attributes.getValue("BBversion"));

    } else if (strcmp(DualString(name).asCString(), "General") == 0)
    {
        // Set current vector to store the skills
        ParametersHandler::vCurrent_ = &ParametersHandler::vSkillsGeneral_;
    } else if (strcmp(DualString(name).asCString(), "Agility") == 0)
    {
        // Set current vector to store the skills
        ParametersHandler::vCurrent_ = &ParametersHandler::vSkillsAgility_;
    } else if (strcmp(DualString(name).asCString(), "Passing") == 0)
    {
        // Set current vector to store the skills
        ParametersHandler::vCurrent_ = &ParametersHandler::vSkillsPassing_;
    } else if (strcmp(DualString(name).asCString(), "Strength") == 0)
    {
        // Set current vector to store the skills
        ParametersHandler::vCurrent_ = &ParametersHandler::vSkillsStrength_;
    } else if (strcmp(DualString(name).asCString(), "Mutation") == 0)
    {
        // Set current vector to store the skills
        ParametersHandler::vCurrent_ = &ParametersHandler::vSkillsMutation_;
    }
 }

void ParametersHandler::characters(const XMLCh* const chars, const unsigned int length)
{
    if (currentNode_ == "skill")
    {
      ParametersHandler::vCurrent_->push_back(xercesc::XMLString::transcode(chars));
    }
}

void ParametersHandler::endElement(const XMLCh* const name)
{
    // reset the current node name
    currentNode_ = "";
}
