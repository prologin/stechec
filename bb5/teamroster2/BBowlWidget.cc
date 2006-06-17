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
#include <iostream.h>
#include <vector.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "pgthemewidget.h"
#include "pgcolor.h"
#include "pglistbox.h"
#include "pglistboxitem.h"

#include "TeamrosterApp.hh"
#include "PlayerLineWidget.hh"
#include "RaceParser.hh"
#include "RaceHandler.hh"
#include "BBowlWidget.hh"
#include "Race.hh"

#define APOTHECARY_COST 50000
#define FAN_FACTOR_COST 10000
#define CHEERLEADER_COST 10000
#define ASSISTANT_COACH_COST 10000

BBowlWidget::BBowlWidget(TeamrosterApp *app, PG_Widget *parent,PG_Rect rect) : PG_ThemeWidget(parent,rect, true)
{
	// Parse races.xml file
	RaceParser parser;
	parser.parseFile();
	
	PG_Color black(0,0,0);
	
	// Create one line per player in the team
	for (int i=0; i<16; i++)
	{
	  mywidget[i]= new PlayerLineWidget(app, this,PG_Rect(42,32+26*i,737,25));
	  mywidget[i]->SetTransparency(255, false);
	  mywidget[i]->SetFontColor(black, true);
	  mywidget[i]->Update();
	  mywidget[i]->Show();
	}

	teamName_ = new PG_LineEdit(this,PG_Rect(370,452,150,25),"LineEdit");
	
	race_ = new PG_ListBox(NULL, PG_Rect(350,478,170,50));
	PG_ListBoxItem* item;
	for (unsigned int i=0; i<RaceHandler::vRaces_.size(); i++)
    {
    		item = new PG_ListBoxItem(race_, 10, RaceHandler::vRaces_[i].getName());
    }	
	
	bank_ = new PG_LineEdit(this,PG_Rect(370,529,150,25),"LineEdit",7);
	headCoach_ = new PG_LineEdit(this,PG_Rect(370,556,150,25),"LineEdit");
	
	reroll_ = new PG_LineEdit(this,PG_Rect(639,452,24,15),"LineEdit",1);
	reroll_->SetValidKeys("012345678");
	reroll_->SetText("0");
	
	rerollCost_ = new PG_Label(this,PG_Rect(681,452,45,15),"0","Label");
	rerollCost_->SetAlignment(PG_Label::CENTER);
	
	rerollTotalCost_ = new PG_Label(this,PG_Rect(729,452,50,15),"0","Label");
	rerollTotalCost_->SetAlignment(PG_Label::RIGHT);
	
	fanFactor_ = new PG_LineEdit(this,PG_Rect(639,468,24,15),"LineEdit",1);
	fanFactor_->SetValidKeys("0123456789");
	fanFactor_->SetText("0");
	
	fanFactorCost_ = new PG_Label(this,PG_Rect(729,468,50,15),"0","Label");
	fanFactorCost_->SetAlignment(PG_Label::RIGHT);
	
	assistantCoach_ = new PG_LineEdit(this,PG_Rect(639,484,24,15),"LineEdit",1);
	assistantCoach_->SetValidKeys("0123456789");
	assistantCoach_->SetText("0");
	
	assistantCoachCost_ = new PG_Label(this,PG_Rect(729,484,50,15),"0","Label");	
	assistantCoachCost_->SetAlignment(PG_Label::RIGHT);
	
	cheerleader_ = new PG_LineEdit(this,PG_Rect(639,500,24,15),"LineEdit",1);
	cheerleader_->SetValidKeys("0123456789");
	cheerleader_->SetText("0");
	
	cheerleaderCost_ = new PG_Label(this,PG_Rect(729,500,50,20),"0","Label");
	cheerleaderCost_->SetAlignment(PG_Label::RIGHT);
	
	apothecary_ = new PG_LineEdit(this,PG_Rect(639,517,24,15),"LineEdit",1);
	apothecary_->SetValidKeys("01");
	apothecary_->SetText("0");
	
	apothecaryCost_ = new PG_Label(this,PG_Rect(729,517,50,20),"0","Label");	
	apothecaryCost_->SetAlignment(PG_Label::RIGHT);
	
	totalTeamValueCost_ = new PG_Label(this,PG_Rect(729,556,50,20),"0","Label");	
	totalTeamValueCost_->SetAlignment(PG_Label::RIGHT);
	
	AddChild(teamName_);
	AddChild(race_);
	AddChild(bank_);
	AddChild(headCoach_);
	AddChild(reroll_);
	AddChild(rerollCost_);
	AddChild(fanFactor_);
	AddChild(assistantCoach_);
	AddChild(cheerleader_);
	AddChild(apothecary_);
	AddChild(rerollTotalCost_);
	AddChild(fanFactorCost_);
	AddChild(assistantCoachCost_);
	AddChild(cheerleaderCost_);
	AddChild(apothecaryCost_);
	AddChild(totalTeamValueCost_);
	
	// Add component for the navigation PG_Navigator
	app->Add(teamName_);
	app->Add(race_);
	app->Add(bank_);
	app->Add(headCoach_);
	app->Add(reroll_);
	app->Add(fanFactor_);
	app->Add(assistantCoach_);
	app->Add(cheerleader_);
	app->Add(apothecary_);
		
	// Create handlers
	apothecary_->sigEditEnd.connect(slot(*this, &BBowlWidget::handleEditApothecary));
	reroll_->sigEditEnd.connect(slot(*this, &BBowlWidget::handleEditReroll));
	assistantCoach_->sigEditEnd.connect(slot(*this, &BBowlWidget::handleEditAssistantCoach));
	fanFactor_->sigEditEnd.connect(slot(*this, &BBowlWidget::handleEditFanFactor));
	cheerleader_->sigEditEnd.connect(slot(*this, &BBowlWidget::handleEditCheerleader));
	
	race_->sigSelectItem.connect(slot(*this, &BBowlWidget::handleSelectItemRace));
	race_->SelectFirstItem();
}

BBowlWidget::~BBowlWidget()
{	
	delete teamName_;
	delete race_;
	delete teamRating_;	
	delete bank_;	
	delete headCoach_;
	delete reroll_;
	delete rerollCost_;
	delete rerollTotalCost_;
	delete fanFactor_;
	delete assistantCoach_;
	delete cheerleader_;
	delete apothecary_;
	delete fanFactorCost_;
	delete assistantCoachCost_;	
	delete cheerleaderCost_;	
	delete apothecaryCost_;	
	delete totalTeamValueCost_;	

	for (int i=0; i<16; i++)
    {	
    		delete mywidget[i];
    }
}

bool BBowlWidget::handleEditApothecary(PG_LineEdit* edit) {
	const char* text = apothecary_->GetText();
	int val = (text != NULL) ? atoi(text) : 0;
	
	char buf[6];
	sprintf(buf, "%ld", APOTHECARY_COST * (long)val);
	apothecaryCost_->SetText(buf);
	evaluateTotalTeamCost();
	
	return true;
}

bool BBowlWidget::handleEditReroll(PG_LineEdit* edit)
 {
	const char* text = reroll_->GetText();
	int val = (text != NULL) ? atoi(text) : 0;
	
	char buf[6];
	sprintf(buf, "%ld", atol(rerollCost_->GetText()) * (long)val);
	rerollTotalCost_->SetText(buf);
	evaluateTotalTeamCost();
	
	return true;
}

bool BBowlWidget::handleEditFanFactor(PG_LineEdit* edit) {
	const char* text = fanFactor_->GetText();
	int val = (text != NULL) ? atoi(text) : 0;
	
	char buf[6];
	sprintf(buf, "%ld", FAN_FACTOR_COST * (long)val);
	fanFactorCost_->SetText(buf);
	evaluateTotalTeamCost();
	
	return true;
}

bool BBowlWidget::handleEditAssistantCoach(PG_LineEdit* edit) {
	const char* text = assistantCoach_->GetText();
	int val = (text != NULL) ? atoi(text) : 0;
	
	char buf[6];
	sprintf(buf, "%ld", ASSISTANT_COACH_COST * (long)val);
	assistantCoachCost_->SetText(buf);
	evaluateTotalTeamCost();
	
	return true;
}

bool BBowlWidget::handleEditCheerleader(PG_LineEdit* edit) {
	const char* text = cheerleader_->GetText();
	int val = (text != NULL) ? atoi(text) : 0;
	
	char buf[6];
	sprintf(buf, "%ld", CHEERLEADER_COST * (long)val);
	cheerleaderCost_->SetText(buf);
	evaluateTotalTeamCost();
	
	return true;
}


void BBowlWidget::evaluateTotalTeamCost()
{
	long totalCost = 0;
	
	// Sum up all the players costs
	for (int i=0; i<16; i++)
    {
    		totalCost += mywidget[i]->getPlayerCost();
    }
    
    // Add the additional costs
 	totalCost += atol(rerollTotalCost_->GetText());
	totalCost += atol(fanFactorCost_->GetText());
	totalCost += atol(assistantCoachCost_->GetText());
	totalCost += atol(cheerleaderCost_->GetText());
	totalCost += atol(apothecaryCost_->GetText());
	
	char buf[7];
	sprintf(buf, "%ld", totalCost);
	totalTeamValueCost_->SetText(buf);
}

bool BBowlWidget::handleSelectItemRace(PG_ListBoxBaseItem* item)
{
	const char* selectedRace = item->GetText();
	
	// Retrieve Postions vector for the selected race.
	vector<Position> vPos;
	for (unsigned int i=0; i< RaceHandler::vRaces_.size(); i++)
	{
		if (strcmp(selectedRace, RaceHandler::vRaces_[i].getName()) == 0)
		{
			
			// Get Positions for the selected race
			vPos = RaceHandler::vRaces_[i].getPositions();

			// Update apothecary use
			if (RaceHandler::vRaces_[i].getApothecaryUse())
			{
				apothecary_->SetValidKeys("01");
			}
			else
			{
				// Remove any old apothecary
				apothecary_->SetValidKeys("0");
				apothecary_->SetText("0");
				apothecaryCost_->SetText("0");
			}	
			
			// Update rerollcost
			char buf[6]; 
			sprintf(buf, "%ld", RaceHandler::vRaces_[i].getRerollCost());
			rerollCost_->SetText(buf);
		
			// Update the total cost for reroll because the price can change between races.
			handleEditReroll(NULL);
		
			break;
		}
	}
		
	// Update all the dropdowns
	for (int i=0; i<16; i++)
	{
		mywidget[i]->updatePosition(vPos);
	}
	return true;
}


