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
