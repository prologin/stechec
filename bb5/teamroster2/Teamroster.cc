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
#include "pgthemewidget.h"

#include "gui/paragui/TeamrosterApp.hh"
#include "gui/paragui/BBowlWidget.hh"

#define RESX 800
#define RESY 600

int main(int argc,char *argv[])

{
	TeamrosterApp *app;

	app=new TeamrosterApp();
	app->SetEmergencyQuit(true);
	app->DisableDirtyUpdates(true);
	app->LoadTheme("bbowl");
	app->InitScreen(RESX,RESY,0,SDL_SWSURFACE);

	//Initialisation gettext
	setlocale( LC_ALL, "" );
	bindtextdomain( "teamroster", "language" );
	textdomain( "teamroster" );
	 
	 
    BBowlWidget *mainPanel = new BBowlWidget(app, NULL, PG_Rect(0, 0, RESX, RESY));
	PG_Color black(0,0,0);
	mainPanel->SetFontColor(black, true);
	mainPanel->SetTransparency(255, false);
	mainPanel->Update();
	mainPanel->Show();
	
	app->GotoFirst();
	app->Run();
	
	delete mainPanel;
	delete app;
	
	return EXIT_SUCCESS;
}

