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
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <libintl.h>


#include "pgthemewidget.h"
#include "pgcolor.h"
#include "pglineedit.h"
#include "pglistbox.h"
#include "pglistboxitem.h"
#include "pgmessagebox.h"
#include "pgimage.h"
#include "pgbutton.h"

#include "BBowlWidget.hh"
#include "InputDialog.hh"
#include "BackgroundDialog.hh"
#include "TeamrosterApp.hh"
#include "PlayerLineWidget.hh"
#include "../../xml/TRParser.hh"
#include "../../xml/TeamHandler.hh"
#include "../../xml/RaceHandler.hh"
#include "../../xml/TeamWriter.hh"
#include "../../common/Team.hh"
#include "../../common/Race.hh"
#include "../../common/InvalidParameterException.hh"

#define LINE_HEIGHT  10

BBowlWidget::BBowlWidget(TeamrosterApp *app, PG_Widget *parent,PG_Rect rect) : PG_ThemeWidget(parent,rect, true)
{
    app_ = app;
    
    TRParser parser;
    
    // Parse parameters.xml file
    parser.parseParametersFile();
    
    // Parse races.xml file
    parser.parseRaceFile();
            
    // Create a team instance with the first available race
    team_ = new Team(&RaceHandler::vRaces_[0]);
	
    race_ = new PG_ListBox(NULL, PG_Rect(350,478,170,50));
    PG_ListBoxItem* item;
    for (unsigned int i=0; i<RaceHandler::vRaces_.size(); i++)
    {
        item = new PG_ListBoxItem(race_, LINE_HEIGHT, RaceHandler::vRaces_[i].getName());
    }      
    
    raceImg_ = new PG_Image (this, PG_Point(180,452),"emblems/amazon.jpg");

    backgroundBtn_ = new PG_Button(this, PG_Rect(70,485,80,20), gettext("Background"));
    loadBtn_ = new PG_Button(this, PG_Rect(100,510,50,20), gettext("Load"));
    saveBtn_ = new PG_Button(this, PG_Rect(100,535,50,20), gettext("Save"));
    quitBtn_ = new PG_Button(this, PG_Rect(100,560,50,20), gettext("Quit"));
  
    PG_Color black(0,0,0);
	
    teamName_ = new PG_LineEdit(this,PG_Rect(370,452,150,25),"LineEdit");
	bank_ = new PG_LineEdit(this,PG_Rect(370,529,150,25),"LineEdit",7);
    bank_->SetValidKeys("0123456789");

	headCoach_ = new PG_LineEdit(this,PG_Rect(370,556,150,25),"LineEdit");
	
	reroll_ = new PG_LineEdit(this,PG_Rect(639,452,24,15),"LineEdit",1);
	reroll_->SetValidKeys("0123456789");
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
	apothecary_->SetValidKeys("0123456789");
	apothecary_->SetText("0");
	
	apothecaryCost_ = new PG_Label(this,PG_Rect(729,517,50,20),"0","Label");	
	apothecaryCost_->SetAlignment(PG_Label::RIGHT);
	
	totalTeamValueCost_ = new PG_Label(this,PG_Rect(729,556,50,20),"0","Label");	
	totalTeamValueCost_->SetAlignment(PG_Label::RIGHT);
    
    // Create one line per player in the team
    for (int i=0; i<TEAM_SIZE; i++)
    {
      playerWidget_[i]= new PlayerLineWidget(app, this,PG_Rect(42,32+26*i,737,25), team_->getPlayer(i+1));
      playerWidget_[i]->updatePositionsList(RaceHandler::vRaces_[0].getPositions());
      playerWidget_[i]->SetTransparency(255, false);
      playerWidget_[i]->SetFontColor(black, true);
      playerWidget_[i]->Update();
      playerWidget_[i]->Show();
    }

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
	headCoach_->sigEditEnd.connect(slot(*this, &BBowlWidget::handleEditHeadCoach));
    bank_->sigEditEnd.connect(slot(*this, &BBowlWidget::handleEditBank));
    teamName_->sigEditEnd.connect(slot(*this, &BBowlWidget::handleEditName));
    
    apothecary_->sigEditEnd.connect(slot(*this, &BBowlWidget::handleEditApothecary));
	reroll_->sigEditEnd.connect(slot(*this, &BBowlWidget::handleEditReroll));
	assistantCoach_->sigEditEnd.connect(slot(*this, &BBowlWidget::handleEditAssistantCoach));
	fanFactor_->sigEditEnd.connect(slot(*this, &BBowlWidget::handleEditFanFactor));
	cheerleader_->sigEditEnd.connect(slot(*this, &BBowlWidget::handleEditCheerleader));

    backgroundBtn_->sigClick.connect(slot(*this, &BBowlWidget::handleButtonBackgroundClick));
    loadBtn_->sigClick.connect(slot(*this, &BBowlWidget::handleButtonLoadClick));
    saveBtn_->sigClick.connect(slot(*this, &BBowlWidget::handleButtonSaveClick));
    quitBtn_->sigClick.connect(slot(*this, &BBowlWidget::handleButtonQuitClick));
 
    race_->sigSelectItem.connect(slot(*this, &BBowlWidget::handleSelectItemRace));
    race_->SelectFirstItem();

    updateView();
}

BBowlWidget::~BBowlWidget()
{	
    Hide();
    delete team_;
    delete raceImg_;
	delete teamName_;
	delete race_;
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
    delete saveBtn_;

	for (int i=0; i<TEAM_SIZE; i++)
    {	
    		delete playerWidget_[i];
    }
}

void BBowlWidget::displayMessage(const char* title, const char* msg)
{
     PG_MessageBox msgbox(NULL, 
      PG_Rect(200,50,240,100), title, msg, 
      PG_Rect(100, 75, 20, 20), "OK");
      
      PG_Color white(255,255,255);
      PG_Color black(0,0,0);
      msgbox.SetTransparency(0, false);
      msgbox.SetFontColor(black, true);
      msgbox.SetSimpleBackground(true);
      msgbox.SetBackgroundColor(white);
      msgbox.SetTitlebarColor(white);
      msgbox.Update();
      msgbox.Show();          
      msgbox.WaitForClick();  
      msgbox.Hide();
}    

void BBowlWidget::displayError(const char* msg)
{
    displayMessage(gettext("Error"),msg);
}

/*
 * Method used to update the widget view
 */
void BBowlWidget::updateView()
{     
//std::cout<<"BBowlWidget::updateView()"<<std::endl;

    teamName_->SetText(team_->getName());
    
    char bank[6];
    sprintf(bank, "%ld", team_->getBank());
    bank_->SetText(bank);

    headCoach_->SetText(team_->getHeadCoach());

    char reroll[1];
    sprintf(reroll, "%d", team_->getReroll());
    reroll_->SetText(reroll);
    
    char rerollUnitCost[5];
    sprintf(rerollUnitCost, "%ld", team_->getRerollUnitaryCost());
    rerollCost_->SetText(rerollUnitCost);
    
    char rerollTotalCost[6];
    sprintf(rerollTotalCost, "%ld", team_->getRerollTotalCost());
    rerollTotalCost_->SetText(rerollTotalCost);
    
    char fanFactor[1];
    sprintf(fanFactor, "%d", team_->getFanFactor());
    fanFactor_->SetText(fanFactor);
    
    char fanFactorCost[6];
    sprintf(fanFactorCost, "%ld", team_->getFanFactorCost());
    fanFactorCost_->SetText(fanFactorCost);

    char assistantCoach[1];
    sprintf(assistantCoach, "%d", team_->getAssistantCoach());
    assistantCoach_->SetText(assistantCoach);
    
    char assistantCoachCost[6];
    sprintf(assistantCoachCost, "%ld", team_->getAssistantCoachCost());
    assistantCoachCost_->SetText(assistantCoachCost);

    char cheerleader[1];
    sprintf(cheerleader, "%d", team_->getCheerleader());
    cheerleader_->SetText(cheerleader);
    
    char cheerleaderCost[6];
    sprintf(cheerleaderCost, "%ld", team_->getCheerleaderCost());
    cheerleaderCost_->SetText(cheerleaderCost);

    char apothecary[1];
    sprintf(apothecary, "%d", team_->getApothecary());
    apothecary_->SetText(apothecary);
    
    char apothecaryCost[6];
    sprintf(apothecaryCost, "%ld", team_->getApothecaryCost());
    apothecaryCost_->SetText(apothecaryCost);

    char totalTeamValueCost[7];
    sprintf(totalTeamValueCost, "%ld", team_->getTotalValueCost());
    totalTeamValueCost_->SetText(totalTeamValueCost);
}  

bool BBowlWidget::handleEditName(PG_LineEdit* edit)
{
    team_->setName(edit->GetText());
    updateView();   
    return true;
}

bool BBowlWidget::handleEditBank(PG_LineEdit* edit)
{
    const char* text = edit->GetText();
    long val = (text != NULL) ? atol(text) : 0;
    
    team_->setBank(val);
    updateView();   
    return true;
}

bool BBowlWidget::handleEditHeadCoach(PG_LineEdit* edit)
{
    team_->setHeadCoach(edit->GetText());
    updateView();   
    return true;
}

bool BBowlWidget::handleEditApothecary(PG_LineEdit* edit) 
{
	const char* text = edit->GetText();
	int val = (text != NULL) ? atoi(text) : 0;
    try 
    {
        team_->setApothecary(val);
    } 
    catch (InvalidParameterException &ex)
    {
        displayError(ex.msg);
    }
    updateView();	
	return true;
}

bool BBowlWidget::handleEditReroll(PG_LineEdit* edit)
{
    const char* text = edit->GetText();
	int val = (text != NULL) ? atoi(text) : 0;
	if (val > 8) {
        displayError(gettext("No more than 8 rerolls. That's enough ! And it's the rule. ;)"));
    }
    else {    
        team_->setReroll(val);
    }
    updateView();   
	return true;
}

bool BBowlWidget::handleEditFanFactor(PG_LineEdit* edit) 
{
    const char* text = edit->GetText();
	int val = (text != NULL) ? atoi(text) : 0;
	
    team_->setFanFactor(val);
    updateView();   
	return true;
}

bool BBowlWidget::handleEditAssistantCoach(PG_LineEdit* edit) 
{
    const char* text = edit->GetText();
	int val = (text != NULL) ? atoi(text) : 0;
	
    team_->setAssistantCoach(val);
    updateView();   
	return true;
}

bool BBowlWidget::handleEditCheerleader(PG_LineEdit* edit) 
{
    const char* text = edit->GetText();
	int val = (text != NULL) ? atoi(text) : 0;
	
    team_->setCheerleader(val);
    updateView();   
	return true;
}

bool BBowlWidget::handleButtonLoadClick(PG_Button* button)
{
   button->SetInputFocus();
     
   InputDialog iDialog(NULL, 
      PG_Rect(240,50,240,170), gettext("Load"), gettext("Please enter the filename to load:"), 
      PG_Rect(70, 125, 30, 20), "OK",
      PG_Rect(120, 125, 60, 20), gettext("CANCEL"),
      PG_Rect(20, 75, 200, 20), "");
      
   iDialog.Show();          
   int btnClickedID = iDialog.WaitForClick();  
   iDialog.Hide();
   // If OK was clicked...
   if (btnClickedID == 1)
   { 
        std::cout << "Parse file: " << iDialog.getText();
   
        // Parse team XML file
        TRParser parser;
        try {
            parser.parseTeamFile(iDialog.getText());
            std::cout << " ... OK" << std::endl;
      
            // Update the team with the new one. 
            team_ = TeamHandler::team_;

            //std::cout << " race_->GetWidgetCount():"<< race_->GetWidgetCount() << std::endl;
            
            // select appropriate race 
            for (unsigned int i=0; i<race_->GetWidgetCount(); i++)
            {
                PG_ListBoxBaseItem* item = (PG_ListBoxBaseItem*)race_->FindWidget(i);
            
                if (strcmp(item->GetText(), team_->getRace()->getName()) == 0)
                {
                    //std::cout << " item found i:"<< i << std::endl;
                
                    item->Select(true);
                    race_->SelectItem(item, true);
                    race_->ScrollTo(i*LINE_HEIGHT);
                    break;
                }
            }
    
            // load team emblen  
            char* filename = new char[80];
            sprintf(filename,"emblems/%s", team_->getEmblem());
            raceImg_->LoadImage(filename);
            delete filename;
                    
            // Retrieve Postions vector for the selected race.
            std::vector<Position> vPos = team_->getRace()->getPositions();
    
            // Update all the dropdowns
            for (int i=0; i<TEAM_SIZE; i++)
            {
                playerWidget_[i]->Hide();
                playerWidget_[i]->updatePositionsList(vPos);
                playerWidget_[i]->updateModel(team_->getPlayer(i+1));
                playerWidget_[i]->Show();
            }
        }
        catch (const SAXException& toCatch)
        {
             std::cout << gettext(" ... Failed \nSAXException : An error occurred\n  Error: ")
                     << xercesc::XMLString::transcode(toCatch.getMessage())
                     << "\n" << std::endl;
        } 
    
        updateView();
   } 
    return true; 
}

bool BBowlWidget::handleButtonSaveClick(PG_Button* button)
{
   button->SetInputFocus();
 
   if (strcmp(team_->getName(),"") == 0)
   {
      displayError(gettext("Please fill the team name before saving. The file will be saved as teamName.xml"));
   }
   else 
   { 
       TeamWriter writer;
       std::string fileName = team_->getName();
       fileName += ".xml";
       writer.writeTeam(fileName.c_str(), team_);
       std::string msg = gettext("Successfully saved as ")+fileName;
       displayMessage(gettext("Save"),msg.c_str());
   }
   return true; 
}

bool BBowlWidget::handleButtonQuitClick(PG_Button* button)
{
   button->SetInputFocus();
   app_->Quit();
   return true;
}

bool BBowlWidget::handleButtonBackgroundClick(PG_Button* button)
{
   button->SetInputFocus();
   
   BackgroundDialog iDialog(NULL, PG_Rect(180,50,310,270), gettext("Background"), team_->getBackground());
   iDialog.Show();          
   int btnClickedID = iDialog.WaitForClick();  
   iDialog.Hide();
        
   // If OK was clicked...
   if (btnClickedID == 1)
   {
       // Update team background  
       team_->setBackground(iDialog.getBackground());
   }
   
   updateView();

   return true;
}

bool BBowlWidget::handleSelectItemRace(PG_ListBoxBaseItem* item)
{
    // if an old team exist...
    if (team_ != NULL)
    {
        if (strcmp(item->GetText(), team_->getRace()->getName()) == 0)
        {
            // Race hasn't changed...
            return true;
        }    
    
        // if the race has changed, remove old team 
        delete team_;
    }

    
    // Create team instance
    //   int idx = race_->GetSelectedIndex();
    // workaround to solve paragui bug which returns always 0
    int idx=0; 
    for (unsigned int i=0; i<RaceHandler::vRaces_.size(); i++)
    {
        if (strcmp(item->GetText(), RaceHandler::vRaces_[i].getName()) == 0)
        {
            idx = i;
            break;
        }
    }

    // Create new team 
    team_ = new Team(&RaceHandler::vRaces_[idx]);
    
    // Change team emblem
    team_->setEmblem(RaceHandler::vRaces_[idx].getEmblem());
    
    char* filename = new char[80];
    sprintf(filename,"emblems/%s", RaceHandler::vRaces_[idx].getEmblem());
    raceImg_->LoadImage(filename);
    delete filename;
        		
    // Retrieve Postions vector for the selected race.
    std::vector<Position> vPos = RaceHandler::vRaces_[idx].getPositions();

	// Update all the dropdowns
	for (int i=0; i<TEAM_SIZE; i++)
	{
        playerWidget_[i]->Hide();
        playerWidget_[i]->updatePositionsList(vPos);
        playerWidget_[i]->updateModel(team_->getPlayer(i+1));
        playerWidget_[i]->Show();
	}
    
    updateView();
	return true;
}
