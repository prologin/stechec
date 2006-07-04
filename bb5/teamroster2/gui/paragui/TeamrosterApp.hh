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
#ifndef TEAMROSTERAPP_HH_
#define TEAMROSTERAPP_HH_

#include "pgapplication.h"
#include "pgnavigator.h"

class TeamrosterApp  : public PG_Application, public PG_Navigator 
{
public:
    
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
	TeamrosterApp();
	virtual ~TeamrosterApp();

protected:

    // -----------------------------------------------------------------------
    //  GUI Handlers
    // -----------------------------------------------------------------------
	bool eventKeyDown(const SDL_KeyboardEvent* key);
};

#endif /*TEAMROSTERAPP_HH_*/
