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
#include "pgapplication.h"
#include "pgnavigator.h"

#include "TeamrosterApp.hh"

#include <iostream>

TeamrosterApp::TeamrosterApp()
{
}

TeamrosterApp::~TeamrosterApp()
{
}

bool TeamrosterApp::eventKeyDown(const SDL_KeyboardEvent* key) {
	PG_Widget* dest = NULL;

	switch(key->keysym.sym) {
		case SDLK_UP:
			dest = FindUp();
			break;
		case SDLK_DOWN:
			dest = FindDown();
			break;
		case SDLK_LEFT:
			dest = FindLeft();
			break;
		case SDLK_RIGHT:
			dest = FindRight();
			break;
		case SDLK_TAB:
			dest = GotoNext();
			if(dest == NULL) {
				GotoFirst();
			}
			return true;
		case SDLK_SPACE:
			Action(PG_Widget::ACT_OK);
			return true;
		default:
			dest = NULL;
			break;
	}

	if(dest != NULL) {
		Goto(dest);
		
		return true;
	}
	return false;
}
