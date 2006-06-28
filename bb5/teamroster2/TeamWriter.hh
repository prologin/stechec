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
#ifndef TEAMWRITER_HH_
#define TEAMWRITER_HH_

#include "Team.hh"

class TeamWriter
{
public:
    void writeTeam(const char* filename, Team* team);
    
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
	TeamWriter();
	virtual ~TeamWriter();
};

#endif /*TEAMWRITER_HH_*/
