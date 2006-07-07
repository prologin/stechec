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
#ifndef TRPARSER_HH_
#define TRPARSER_HH_

class TRParser
{
public:
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
	TRParser();
	virtual ~TRParser();

    void parseTeamFile(const char* filename);
    void parseRaceFile();
    void parseParametersFile();
    
private:
    void printRaces();
};

#endif /*TRPARSER_HH_*/
