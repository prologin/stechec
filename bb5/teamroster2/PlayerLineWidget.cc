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
#include <stdlib.h>
#include <iostream.h>
#include <vector.h>
#include <string.h>

#include "pgthemewidget.h"
#include "pglabel.h"
#include "pgdropdown.h"
#include "pglineedit.h"
#include "pglistboxbaseitem.h"

#include "TeamrosterApp.hh"
#include "My_DropDown.hh"
#include "BBowlWidget.hh"
#include "PlayerLineWidget.hh" 
#include "Position.hh"

PlayerLineWidget::PlayerLineWidget(TeamrosterApp *app, PG_Widget *parent,PG_Rect rect) : PG_ThemeWidget(parent,rect, true)
{
	name_ = new PG_LineEdit(this,PG_Rect(0,0,90,26));
	
	position_ = new My_DropDown(this, PG_Rect(90,0,92,26));	
	position_->SetEditable(false);
	position_->sigSelectItem.connect(slot(*this, &PlayerLineWidget::handleSelectItemPosition));
	
	movementAllowance_ = new PG_LineEdit(this,PG_Rect(182,0,25,26),"LineEdit",2);
	movementAllowance_->SetValidKeys("0123456789");
	
	strength_ = new PG_LineEdit(this,PG_Rect(207,0,25,26),"LineEdit",2);
	strength_->SetValidKeys("0123456789");
	
	agility_ = new PG_LineEdit(this,PG_Rect(233,0,25,26),"LineEdit",2);
	agility_->SetValidKeys("0123456789");
	
	armourValue_ = new PG_LineEdit(this,PG_Rect(258,0,26,26),"LineEdit",2);
	armourValue_->SetValidKeys("0123456789");
	
	skills_ = new PG_LineEdit(this,PG_Rect(285,0,194,26));
	
	injuries_ = new PG_LineEdit(this,PG_Rect(479,0,37,26));
	
	completions_ = new PG_LineEdit(this,PG_Rect(517,0,37,26),"LineEdit",2);
	completions_->SetValidKeys("0123456789");
	
	touchdowns_ = new PG_LineEdit(this,PG_Rect(555,0,26,26),"LineEdit",2);
	touchdowns_->SetValidKeys("0123456789");
	
	interceptions_ = new PG_LineEdit(this,PG_Rect(582,0,26,26),"LineEdit",2);
	interceptions_->SetValidKeys("0123456789");
	
	casualties_ = new PG_LineEdit(this,PG_Rect(608,0,26,26),"LineEdit",2);
	casualties_->SetValidKeys("0123456789");
	
	mostValuablePlayer_ = new PG_LineEdit(this,PG_Rect(634,0,25,26),"LineEdit",2);
	mostValuablePlayer_->SetValidKeys("0123456789");
	
	starPlayerPoints_ = new PG_Label(this,PG_Rect(660,0,26,26),"0","Label");
	starPlayerPoints_->SetAlignment(PG_Label::CENTER);
	value_ = new PG_Label(this,PG_Rect(687,0,50,26),"0","Label");
	value_->SetAlignment(PG_Label::RIGHT);
    
	AddChild(name_);
 	AddChild(position_);
	AddChild(movementAllowance_);
	AddChild(strength_);
	AddChild(agility_);
	AddChild(armourValue_);
	AddChild(skills_);
	AddChild(injuries_);
	AddChild(completions_);
	AddChild(touchdowns_);
	AddChild(interceptions_);
	AddChild(casualties_);
	AddChild(mostValuablePlayer_);
	AddChild(starPlayerPoints_);
	AddChild(value_);

	// Add component for the navigation PG_Navigator
	app->Add(name_);
 	app->Add(position_);
	app->Add(movementAllowance_);
	app->Add(strength_);
	app->Add(agility_);
	app->Add(armourValue_);
	app->Add(skills_);
	app->Add(injuries_);
	app->Add(completions_);
	app->Add(touchdowns_);
	app->Add(interceptions_);
	app->Add(casualties_);
	app->Add(mostValuablePlayer_);

	// Create handlers
	completions_->sigEditEnd.connect(slot(*this, &PlayerLineWidget::handleEvaluateSSP));
	touchdowns_->sigEditEnd.connect(slot(*this, &PlayerLineWidget::handleEvaluateSSP));
	interceptions_->sigEditEnd.connect(slot(*this, &PlayerLineWidget::handleEvaluateSSP));
	casualties_->sigEditEnd.connect(slot(*this, &PlayerLineWidget::handleEvaluateSSP));
	mostValuablePlayer_->sigEditEnd.connect(slot(*this, &PlayerLineWidget::handleEvaluateSSP));
	
}

void PlayerLineWidget::updatePosition(vector<Position> vPos)
{
	vPos_ = vPos;
	
	// Empty the dropdown list
    position_->DeleteAll();
    
    // Fill it with the new position
    position_->AddItem("");
    for (unsigned int i=0; i<vPos.size(); i++)
    {
        position_->AddItem(vPos[i].getTitle());
    }
    
   	position_->SelectFirstItem();
 }
 
 void PlayerLineWidget::emptyPlayerLine()
 {
		movementAllowance_->SetText("");
		strength_->SetText("");
		agility_->SetText("");
		armourValue_->SetText("");
		skills_->SetText("");
 		injuries_->SetText("");
		completions_->SetText("");
		touchdowns_->SetText("");
		interceptions_->SetText("");
		casualties_->SetText("");
		mostValuablePlayer_->SetText("");
 	    starPlayerPoints_->SetText("0");
	    value_->SetText("0");
}
 
 void PlayerLineWidget::fillPlayerLine(Position pos)
 {
 	    char ma[2];
 	    sprintf(ma, "%d", pos.getMovementAllowance());
 		movementAllowance_->SetText(ma);
 		char st[2];
		sprintf(st, "%d", pos.getStrength());
 		strength_->SetText(st);
		char ag[2];
		sprintf(ag, "%d", pos.getAgility());
		agility_->SetText(ag);
		char av[2];
		sprintf(av, "%d", pos.getArmourValue());
		armourValue_->SetText(av);
		skills_->SetText(pos.getSkills());
 	    starPlayerPoints_->SetText("0");
		char cost[6];
		sprintf(cost, "%ld", pos.getCost());
	    value_->SetText(cost);	
 }  
 
 bool PlayerLineWidget::handleEvaluateSSP(PG_LineEdit* edit) {

	const char* text = completions_->GetText();
	int com = (text != NULL) ? atoi(text) : 0;
	
	text = casualties_->GetText();
	int cas = (text != NULL) ? atoi(text) : 0;
	
	text = interceptions_->GetText();
	int inter = (text != NULL) ? atoi(text) : 0;
	
	text = touchdowns_->GetText();
	int td = (text != NULL) ? atoi(text) : 0;
	
	text = mostValuablePlayer_->GetText();
	int jpv = (text != NULL) ? atoi(text) : 0;
	
	char buf[6];
	sprintf(buf, "%d", (com + 2*cas + 2*inter + 3*td + 5*jpv));
	starPlayerPoints_->SetText(buf);
	
	return true;
}
 
 bool PlayerLineWidget::handleSelectItemPosition(PG_ListBoxBaseItem* item)
 {
 	const char* selectedPosition = item->GetText();
 	
 	// Empty all the characteristics
 	if (strcmp("",selectedPosition) == 0)
 	{
 		emptyPlayerLine();
 	}
 	else 
 	{
 		for (unsigned int i=0; i< vPos_.size(); i++)
		{
			if (strcmp(vPos_[i].getTitle(), selectedPosition) == 0)
			{
				fillPlayerLine(vPos_[i]);
				break;
			}
		}
 	}
 	
 	// Re-evaluate the total cost of the team
 	((BBowlWidget*)GetParent())->evaluateTotalTeamCost();
 	
 	return true;
 }
 
 long PlayerLineWidget::getPlayerCost()
 {	
 	return atol(value_->GetText());
 }
 

PlayerLineWidget::~PlayerLineWidget()
{
}
