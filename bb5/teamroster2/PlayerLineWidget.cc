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
#include <iostream>
#include <vector>
#include <string>

#include "pgthemewidget.h"
#include "pglabel.h"
#include "pgdropdown.h"
#include "pglineedit.h"
#include "pglistboxbaseitem.h"
#include "pgmessagebox.h"

#include "TeamrosterApp.hh"
#include "My_DropDown.hh"
#include "BBowlWidget.hh"
#include "PlayerLineWidget.hh" 
#include "Position.hh"
#include "Player.hh"
#include "InvalidParameterException.hh"

PlayerLineWidget::PlayerLineWidget(TeamrosterApp *app, PG_Widget *parent,PG_Rect rect, Player *player) : PG_ThemeWidget(parent,rect, true)
{
    parent_ = parent;
    player_ = player;
    
	name_ = new PG_LineEdit(this,PG_Rect(0,0,90,26));
	
	position_ = new My_DropDown(this, PG_Rect(90,0,92,26));	
	position_->SetEditable(false);
	
	movementAllowance_ = new PG_LineEdit(this,PG_Rect(182,0,25,26),"LineEdit",2);
	movementAllowance_->SetValidKeys("0123456789");
	
	strength_ = new PG_LineEdit(this,PG_Rect(207,0,25,26),"LineEdit",2);
	strength_->SetValidKeys("0123456789");
	
	agility_ = new PG_LineEdit(this,PG_Rect(233,0,25,26),"LineEdit",2);
	agility_->SetValidKeys("0123456789");
	
	armourValue_ = new PG_LineEdit(this,PG_Rect(258,0,26,26),"LineEdit",2);
	armourValue_->SetValidKeys("0123456789");
	
	skills_ = new PG_LineEdit(this,PG_Rect(285,0,194,26));
    skills_->SetEditable(false);
	
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
	app->Add(injuries_);
	app->Add(completions_);
	app->Add(touchdowns_);
	app->Add(interceptions_);
	app->Add(casualties_);
	app->Add(mostValuablePlayer_);

	// Create handlers
    name_->sigEditEnd.connect(slot(*this, &PlayerLineWidget::handleEditName));
    position_->sigSelectItem.connect(slot(*this, &PlayerLineWidget::handleSelectItemPosition));
    movementAllowance_->sigEditEnd.connect(slot(*this, &PlayerLineWidget::handleEditMa));
    strength_->sigEditEnd.connect(slot(*this, &PlayerLineWidget::handleEditSt));
    agility_->sigEditEnd.connect(slot(*this, &PlayerLineWidget::handleEditAg));
    armourValue_->sigEditEnd.connect(slot(*this, &PlayerLineWidget::handleEditAv));
    injuries_->sigEditEnd.connect(slot(*this, &PlayerLineWidget::handleEditInj));
    	completions_->sigEditEnd.connect(slot(*this, &PlayerLineWidget::handleEditComp));
	touchdowns_->sigEditEnd.connect(slot(*this, &PlayerLineWidget::handleEditTd));
	interceptions_->sigEditEnd.connect(slot(*this, &PlayerLineWidget::handleEditInter));
	casualties_->sigEditEnd.connect(slot(*this, &PlayerLineWidget::handleEditCasual));
	mostValuablePlayer_->sigEditEnd.connect(slot(*this, &PlayerLineWidget::handleEditMvp));

    updateView();
}

void PlayerLineWidget::updateModel(Player* player)
{
    player_ = player;
    updateView();
}

void PlayerLineWidget::updatePositionsList(std::vector<Position> vPos)
{
	// Empty the dropdown list
    delete position_;
    position_ = new My_DropDown(this, PG_Rect(90,0,92,26)); 
    position_->SetEditable(false);
    PG_Color black(0,0,0);
    position_->SetFontColor(black, true);
    position_->sigSelectItem.connect(slot(*this, &PlayerLineWidget::handleSelectItemPosition));
    position_->Show();
    
    // Fill it with the new position
    position_->AddItem("");
    for (unsigned int i=0; i<vPos.size(); i++)
    {
        position_->AddItem(vPos[i].getTitle());
    }
    position_->SelectFirstItem();
}
 
/*
 * Method used to update the widget view
 */
void PlayerLineWidget::updateView()
{  
std::cout<<"PlayerLineWidget::updateView()"<<std::endl;
    name_->SetText(player_->getName());
    position_->SetText(player_->getPositionTitle());
    
    char ma[2];
    sprintf(ma, "%d", player_->getMovementAllowance());
    movementAllowance_->SetText(ma);
    char st[2];
    sprintf(st, "%d", player_->getStrength());
    strength_->SetText(st);
    char ag[2];
    sprintf(ag, "%d", player_->getAgility());
    agility_->SetText(ag);
    char av[2];
    sprintf(av, "%d", player_->getArmourValue());
    armourValue_->SetText(av);
    
    skills_->SetText(player_->getSkillsAsString());
 	
    injuries_->SetText(player_->getInjuries());
    
    char com[2];
    sprintf(com, "%d", player_->getCompletions());
    completions_->SetText(com);
    char td[2];
    sprintf(td, "%d", player_->getTouchDowns());
    touchdowns_->SetText(td);
    char in[2];
    sprintf(in, "%d", player_->getInterceptions());
    interceptions_->SetText(in);
    char cas[2];
    sprintf(cas, "%d", player_->getCasualties());
    casualties_->SetText(cas);
    char mvp[2];
    sprintf(mvp, "%d", player_->getMostValuablePlayer());
    mostValuablePlayer_->SetText(mvp);
    char spp[2];
    sprintf(spp, "%d", player_->getStarPlayerPoints());
    starPlayerPoints_->SetText(spp);
    char cost[6];
    sprintf(cost, "%ld", player_->getValue());
    value_->SetText(cost);	
}  

void PlayerLineWidget::displayError(const char* msg)
{
      PG_MessageBox msgbox(parent_, 
      PG_Rect(200,50,240,100), "Erreur", msg, 
      PG_Rect(100, 75, 20, 20), "OK");
      
      PG_Color white(255,255,255);
      PG_Color black(0,0,0);
      msgbox.SetTransparency(0, false);
      msgbox.SetFontColor(black, true);
      msgbox.SetSimpleBackground(true);
      msgbox.SetBackgroundColor(white);
      msgbox.Update();
      msgbox.Show();          
      msgbox.WaitForClick();  
      msgbox.Hide();
}

bool PlayerLineWidget::handleEditName(PG_LineEdit* edit)
{
    player_->setName(edit->GetText());
    updateView();
    return true;
}

bool PlayerLineWidget::handleEditMa(PG_LineEdit* edit)
{
    if (position_->GetText() != NULL && 
        strcmp(position_->GetText(), "") != 0)
    {
        const char* text = edit->GetText();
        int val = (text != NULL) ? atoi(text) : 0;
        
        try 
        {
            player_->setMovementAllowance(val);
        } 
        catch (InvalidParameterException &ex)
        {
            displayError(ex.msg);
        }
    }
    else if (strcmp(edit->GetText(), "0") != 0)
    {
        displayError("Position must be chosen first.");        
    }
    updateView();
            
    // Refresh parent view to update totalTeamValue
    ((BBowlWidget*)parent_)->updateView();
    return true;
}

bool PlayerLineWidget::handleEditSt(PG_LineEdit* edit)
{
    if (position_->GetText() != NULL && 
        strcmp(position_->GetText(), "") != 0)
    {
        const char* text = edit->GetText();
        int val = (text != NULL) ? atoi(text) : 0;
        
        try 
        {
            player_->setStrength(val);
         } 
         catch (InvalidParameterException &ex)
         {
            displayError(ex.msg);
         }
    }
    else if (strcmp(edit->GetText(), "0") != 0)
    {
        displayError("Position must be chosen first.");        
    }
    updateView();
            
    // Refresh parent view to update totalTeamValue
    ((BBowlWidget*)parent_)->updateView();
    return true;
}

bool PlayerLineWidget::handleEditAg(PG_LineEdit* edit)
{
    if (position_->GetText() != NULL && 
        strcmp(position_->GetText(), "") != 0)
    {
        const char* text = edit->GetText();
        int val = (text != NULL) ? atoi(text) : 0;
        
        try 
        {
            player_->setAgility(val);
        } 
        catch (InvalidParameterException &ex)
        {
            displayError(ex.msg);
        }
    }
    else if (strcmp(edit->GetText(), "0") != 0)
    {
        displayError("Position must be chosen first.");        
    }
    updateView();
            
    // Refresh parent view to update totalTeamValue
    ((BBowlWidget*)parent_)->updateView();
    return true;
}

bool PlayerLineWidget::handleEditAv(PG_LineEdit* edit)
{
    if (position_->GetText() != NULL && 
        strcmp(position_->GetText(), "") != 0)
    {
        const char* text = edit->GetText();
        int val = (text != NULL) ? atoi(text) : 0;
        
        try 
        {
            player_->setArmourValue(val);
        } 
        catch (InvalidParameterException &ex)
        {
            displayError(ex.msg);
        }
    }
    else if (strcmp(edit->GetText(), "0") != 0)
    {
        displayError("Position must be chosen first.");        
    }
    updateView();
            
    // Refresh parent view to update totalTeamValue
    ((BBowlWidget*)parent_)->updateView();
    return true;
}
 
bool PlayerLineWidget::handleEditInj(PG_LineEdit* edit)
{
    player_->setInjuries(edit->GetText());
    updateView();
    return true;
}

bool PlayerLineWidget::handleEditComp(PG_LineEdit* edit)
{
    const char* text = edit->GetText();
    int val = (text != NULL) ? atoi(text) : 0;

    player_->setCompletions(val);
    updateView();
    return true;
}

bool PlayerLineWidget::handleEditTd(PG_LineEdit* edit)
{
    const char* text = edit->GetText();
    int val = (text != NULL) ? atoi(text) : 0;

    player_->setTouchDowns(val);
    updateView();
    return true;
}

bool PlayerLineWidget::handleEditInter(PG_LineEdit* edit)
{
    const char* text = edit->GetText();
    int val = (text != NULL) ? atoi(text) : 0;

    player_->setInterceptions(val);
    updateView();
    return true;
}

bool PlayerLineWidget::handleEditCasual(PG_LineEdit* edit)
{
    const char* text = edit->GetText();
    int val = (text != NULL) ? atoi(text) : 0;

    player_->setCasualties(val);
    updateView();
    return true;
}

bool PlayerLineWidget::handleEditMvp(PG_LineEdit* edit)
{
    const char* text = edit->GetText();
    int val = (text != NULL) ? atoi(text) : 0;

    player_->setMostValuablePlayer(val);
    updateView();
    return true;
}

bool PlayerLineWidget::handleSelectItemPosition(PG_ListBoxBaseItem* item)
{
 	const char* selectedPosition = item->GetText();
    
    // Assert the position has changed...
 	if (strcmp(selectedPosition, player_->getPositionTitle()) != 0)
    { 
    
      if (strcmp("",selectedPosition) == 0)
 	  {
            // Reset player caracteristics
            player_->reset();
 	  }
 	  else 
 	  {
            // Set player's position
            player_->setPosition(selectedPosition);
            
            // Validate update    
            try {
                player_->getTeam()->validateTeam();
            } 
            catch (InvalidParameterException &ex)
            {
                displayError(ex.msg);       
                // Reset player caracteristics
                player_->reset();
            }
 	  }
 	
      // Refresh the view 
      updateView(); 
    
      // Refresh parent view to update totalTeamValue
      ((BBowlWidget*)parent_)->updateView();
      
    }
    return true;
}
 
PlayerLineWidget::~PlayerLineWidget()  { }
