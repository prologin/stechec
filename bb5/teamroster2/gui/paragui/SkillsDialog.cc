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
#include "SkillsDialog.hh"
#include "../../xml/ParametersHandler.hh"

#include "pgwindow.h"
#include "pglistbox.h"
#include "pglistboxitem.h"
#include "pglabel.h"

#include <iostream>
#include <vector>
#include <string>

#define LINE_HEIGHT  10

using std::vector;
using std::string;

SkillsDialog::SkillsDialog(PG_Widget* parent, const PG_Rect& r, const char* windowtitle, Player* player, const char* style) : 
PG_Window(parent, r, windowtitle, MODAL) 
{ 
    player_ = player;
    
    PG_Color white(255,255,255);  
    PG_Color black(0,0,0);

    l1_ = new PG_Label(this, PG_Rect(20,40,150,20), "Player's skills");
    l2_ = new PG_Label(this, PG_Rect(230,40,150,20), "Available skills");
    
    // Initialize skills owned by the player
    selectedSkills_ = new PG_ListBox(this, PG_Rect(20,60,150,70));
    PG_ListBoxItem* itemS;
    vector<const char*> vSkills = player_->getSkillsNormal();
    for (unsigned int i=0; i<vSkills.size(); i++)
    {
        itemS = new PG_ListBoxItem(selectedSkills_, LINE_HEIGHT, vSkills[i]);
    }

    // Initialize available skills for that specific player
    availableSkills_ = new PG_ListBox(this, PG_Rect(230,60,150,70));
 
    l3_ = new PG_Label(this, PG_Rect(20,130,150,20), "Player's skills Double");
    l4_ = new PG_Label(this, PG_Rect(230,130,150,20), "Available skills Double");
    
    // Initialize skills double owned by the player
    selectedSkillsDouble_ = new PG_ListBox(this, PG_Rect(20,150,150,70));
    PG_ListBoxItem* itemSD;
    vector<const char*> vSkillsDouble = player_->getSkillsDouble();
    for (unsigned int i=0; i<vSkillsDouble.size(); i++)
    {
        itemSD = new PG_ListBoxItem(selectedSkillsDouble_, LINE_HEIGHT, vSkillsDouble[i]);
    }

    // Initialize available skills double for that specific player
    availableSkillsDouble_ = new PG_ListBox(this, PG_Rect(230,150,150,70));
 
    // Fill available skills
    FillAvailableSkills();
 
    btnAdd_ = new PG_Button(this, PG_Rect(180, 70, 30, 20), "<<");
    btnAdd_->sigClick.connect(slot(*this, &SkillsDialog::handleButtonAddClick));
    
    btnRemove_ = new PG_Button(this, PG_Rect(180, 100, 30, 20), ">>");
    btnRemove_->sigClick.connect(slot(*this, &SkillsDialog::handleButtonRemoveClick));
 
    btnAddDouble_ = new PG_Button(this, PG_Rect(180, 160, 30, 20), "<<");
    btnAddDouble_->sigClick.connect(slot(*this, &SkillsDialog::handleButtonAddDoubleClick));
    
    btnRemoveDouble_ = new PG_Button(this, PG_Rect(180, 190, 30, 20), ">>");
    btnRemoveDouble_->sigClick.connect(slot(*this, &SkillsDialog::handleButtonRemoveDoubleClick));
  
    btnOk_ = new PG_Button(this, PG_Rect(150, 235, 30, 20), "OK");
    btnOk_->SetID(1);
    btnOk_->sigClick.connect(slot(*this, &SkillsDialog::handleButton));
    
    btnCancel_ = new PG_Button(this, PG_Rect(200, 235, 60, 20), "CANCEL");
    btnCancel_->SetID(2);
    btnCancel_->sigClick.connect(slot(*this, &SkillsDialog::handleButton));

    Init(style);

    SetTransparency(0, false);
    SetFontColor(black, true);
    SetSimpleBackground(true);
    SetBackgroundColor(white);
    SetTitlebarColor(white);
    Update();
}
    
SkillsDialog::~SkillsDialog()
{
    delete btnOk_;
    delete btnCancel_;
    delete btnAdd_;
    delete btnRemove_;
    delete btnAddDouble_;
    delete btnRemoveDouble_;
    delete selectedSkills_;
    delete availableSkills_;
    delete selectedSkillsDouble_;
    delete availableSkillsDouble_;
    delete l1_;
    delete l2_;
    delete l3_;
    delete l4_;
}

void SkillsDialog::FillAvailableSkills() {

    // Add the general skills if they are available for that player
    if (strchr(player_->getPosition()->getNormalSkills(), 'g') != NULL)
    {
        InsertSkillsFromVector(ParametersHandler::vSkillsGeneral_, availableSkills_);
    } else if (strchr(player_->getPosition()->getDoubleSkills(), 'g') != NULL)
    {
        InsertSkillsFromVector(ParametersHandler::vSkillsGeneral_, availableSkillsDouble_);
    }        

    // Add the agility skills if they are available for that player
    if (strchr(player_->getPosition()->getNormalSkills(), 'a') != NULL) 
    {
        InsertSkillsFromVector(ParametersHandler::vSkillsAgility_, availableSkills_);
    } else if (strchr(player_->getPosition()->getDoubleSkills(), 'a') != NULL)
    {
        InsertSkillsFromVector(ParametersHandler::vSkillsAgility_, availableSkillsDouble_);
    }        

    // Add the pass skills if they are available for that player
    if (strchr(player_->getPosition()->getNormalSkills(), 'p') != NULL)
    {
        InsertSkillsFromVector(ParametersHandler::vSkillsPassing_, availableSkills_);
    } else if (strchr(player_->getPosition()->getDoubleSkills(), 'p') != NULL)
    {
        InsertSkillsFromVector(ParametersHandler::vSkillsPassing_, availableSkillsDouble_);
    }

    // Add the strength skills if they are available for that player
    if (strchr(player_->getPosition()->getNormalSkills(), 's') != NULL)
    {
        InsertSkillsFromVector(ParametersHandler::vSkillsStrength_, availableSkills_);
    } else if (strchr(player_->getPosition()->getDoubleSkills(), 's') != NULL)
    {
        InsertSkillsFromVector(ParametersHandler::vSkillsStrength_, availableSkillsDouble_);
    }

    // Add the mutation skills if they are available for that player
    if (strchr(player_->getPosition()->getNormalSkills(), 'm') != NULL) 
    {
        InsertSkillsFromVector(ParametersHandler::vSkillsMutation_, availableSkills_);
    } else if (strchr(player_->getPosition()->getDoubleSkills(), 'm') != NULL)
    {
        InsertSkillsFromVector(ParametersHandler::vSkillsMutation_, availableSkillsDouble_);
    }
}


void SkillsDialog::InsertSkillsFromVector(vector<string> v, PG_ListBox* list)
{
    PG_ListBoxItem* item;
    for (unsigned int i=0; i<v.size(); i++)
    {
        // Add the skill if this skill is not already owned by the player
        if (!IsPlayerSkill(v[i].c_str()))
        {
            item = new PG_ListBoxItem(list, LINE_HEIGHT, v[i].c_str());
        }
    }      
}

/**
 * Return true is the skill s is already owned by the player
 */
bool SkillsDialog::IsPlayerSkill(const char* s)
{
    bool result = false;
    
    vector<const char*> vSkills = player_->getSkills();
    for (unsigned int i=0; i<vSkills.size(); i++)
    {
        if (strcmp(s, vSkills[i]) == 0)
        {
            result = true;
            break;
        }
    }
    
    return result;
}

void SkillsDialog::Init(const char* style) {
    LoadThemeStyle(style);
}

void SkillsDialog::LoadThemeStyle(const char* widgettype) {
    PG_Window::LoadThemeStyle(widgettype);

    btnOk_->LoadThemeStyle(widgettype, "Button1");
    if(btnCancel_) {
        btnCancel_->LoadThemeStyle(widgettype, "Button2");
    }
}

// Return the list of the player's skills
vector<string> SkillsDialog::getPlayerSkillsDouble()
{
    vector<string>* vSkills = new vector<string>();
    
    for (unsigned int i=0; i<selectedSkillsDouble_->GetWidgetCount(); i++)
    {
        char* str = new char[strlen(selectedSkillsDouble_->FindWidget(i)->GetText())];
        sprintf(str, "%s", selectedSkillsDouble_->FindWidget(i)->GetText());
       // std::cout << str << std::endl;
        vSkills->push_back(str);
    }
    
    return *vSkills;
}

// Return the list of the player's skills
vector<string> SkillsDialog::getPlayerSkills()
{
    vector<string>* vSkills = new vector<string>();
    
    for (unsigned int i=0; i<selectedSkills_->GetWidgetCount(); i++)
    {
        char* str = new char[strlen(selectedSkills_->FindWidget(i)->GetText())];
        sprintf(str, "%s", selectedSkills_->FindWidget(i)->GetText());
       // std::cout << str << std::endl;
        vSkills->push_back(str);
    }
    
    return *vSkills;
}

//Events
bool SkillsDialog::handleButtonAddClick() {
 
    PG_ListBoxBaseItem* item2remove = availableSkills_->GetSelectedItem();
    
    if (item2remove != NULL)
    {    
        // Create new skill in the appropriate list
        new PG_ListBoxItem(selectedSkills_, LINE_HEIGHT, item2remove->GetText()); 
        PG_Color black(0,0,0);
        selectedSkills_->SetFontColor(black, true);
        selectedSkills_->Update();
        delete item2remove;
    }
    return true;
}

bool SkillsDialog::handleButtonRemoveClick() {
 
    PG_ListBoxBaseItem* item2remove = selectedSkills_->GetSelectedItem();
    
    if (item2remove != NULL)
    {
        // Create new skill in the appropriate list
        new PG_ListBoxItem(availableSkills_, LINE_HEIGHT, item2remove->GetText()); 
        PG_Color black(0,0,0);
        availableSkills_->SetFontColor(black, true);
        availableSkills_->Update();
    
        delete item2remove;
    }
    return true;
}

bool SkillsDialog::handleButtonAddDoubleClick() {
 
    PG_ListBoxBaseItem* item2remove = availableSkillsDouble_->GetSelectedItem();
    
    if (item2remove != NULL)
    {    
        // Create new skill in the appropriate list
        new PG_ListBoxItem(selectedSkillsDouble_, LINE_HEIGHT, item2remove->GetText()); 
        PG_Color black(0,0,0);
        selectedSkillsDouble_->SetFontColor(black, true);
        selectedSkillsDouble_->Update();
        delete item2remove;
    }
    return true;
}

bool SkillsDialog::handleButtonRemoveDoubleClick() {
 
    PG_ListBoxBaseItem* item2remove = selectedSkillsDouble_->GetSelectedItem();
    
    if (item2remove != NULL)
    {
        // Create new skill in the appropriate list
        new PG_ListBoxItem(availableSkillsDouble_, LINE_HEIGHT, item2remove->GetText()); 
        PG_Color black(0,0,0);
        availableSkillsDouble_->SetFontColor(black, true);
        availableSkillsDouble_->Update();
    
        delete item2remove;
    }
    return true;
}

bool SkillsDialog::handleButton(PG_Button* button) {
    //Set Buttonflag to ButtonID
    button->SetInputFocus();
    SetModalStatus(button->GetID());
    QuitModal();
    return true;
}
