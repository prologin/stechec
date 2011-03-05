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
#ifndef PARAMETERSHANDLER_HH_
#define PARAMETERSHANDLER_HH_

#include    <xercesc/sax/HandlerBase.hpp>

XERCES_CPP_NAMESPACE_USE

class ParametersHandler : public HandlerBase
{
public:
    static std::vector<std::string> vSkillsGeneral_;
    static std::vector<std::string> vSkillsAgility_;
    static std::vector<std::string> vSkillsPassing_;
    static std::vector<std::string> vSkillsStrength_;
    static std::vector<std::string> vSkillsMutation_;
    static std::vector<std::string> vSkillsExtraordinary_;

    static std::string BBversion_;   

    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
	ParametersHandler();
	virtual ~ParametersHandler();

    // -----------------------------------------------------------------------
    //  Handlers for the SAX DocumentHandler interface
    // -----------------------------------------------------------------------
    void startElement(const XMLCh* const name, AttributeList& attributes);
    void characters(const XMLCh* const chars, const unsigned int length);
    void endElement(const XMLCh* const name);
  

private:
   static std::vector<std::string> *vCurrent_;
   
   std::string    currentNode_;
};

#endif /*PARAMETERSHANDLER_HH_*/
