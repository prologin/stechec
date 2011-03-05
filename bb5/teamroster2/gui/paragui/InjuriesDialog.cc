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
#include <string>
#include <sstream>
#include <libintl.h>

#include "InjuriesDialog.hh"
#include "../../common/Player.hh"

#include "pgwindow.h"
#include "pglabel.h"
#include "pgthemewidget.h"
#include "pglineedit.h"
#include "pgmessagebox.h"

InjuriesDialog::InjuriesDialog(PG_Widget* parent, const PG_Rect& r, const char* windowtitle, Player* player, const char* style) : 
PG_Window(parent, r, windowtitle, MODAL) 
{ 
    parent_ = parent;
    player_ = player;
    
    PG_Color white(255,255,255);  
    PG_Color black(0,0,0);
    
    btnOk_ = new PG_Button(this, PG_Rect(60, 235, 30, 20), "OK");
    btnOk_->SetID(1);
    btnOk_->sigClick.connect(slot(*this, &InjuriesDialog::handleButton));
    
    btnCancel_ = new PG_Button(this, PG_Rect(130, 235, 60, 20), gettext("CANCEL"));
    btnCancel_->SetID(2);
    btnCancel_->sigClick.connect(slot(*this, &InjuriesDialog::handleButton));

    cbtnMissNextMatch_ = new PG_CheckButton(this, PG_Rect(20,40,200,20), gettext(" Miss next match"));
    if (player_->getMissNextMatch())  {
        cbtnMissNextMatch_->SetPressed();
    } else {
        cbtnMissNextMatch_->SetUnpressed();
    }        

    lblNigglingInjuries_ = new PG_Label(this, PG_Rect(20,70,150,20), gettext("Number of Niggling injuries:"));
    leNigglingInjuries_ = new PG_LineEdit(this,PG_Rect(175,70,26,26),"LineEdit",2);
    leNigglingInjuries_->SetValidKeys("0123456789");
    leNigglingInjuries_->SetText(to_string(player_->getNigglingInjuries()).c_str());

    lblMaReducted_ = new PG_Label(this, PG_Rect(20,100,150,20), gettext("Number of -1 MA (Max 2):"));
    leMaReducted_ = new PG_LineEdit(this,PG_Rect(175,100,26,26),"LineEdit",2);
    leMaReducted_->SetValidKeys("0123456789");
    leMaReducted_->SetText(to_string(player_->getMaReducted()).c_str());
    leMaReducted_->sigEditEnd.connect(slot(*this, &InjuriesDialog::handleEditCharacteristicReducted));
   
    lblAvReducted_ = new PG_Label(this, PG_Rect(20,130,150,20), gettext("Number of -1 AV (Max 2):"));
    leAvReducted_ = new PG_LineEdit(this,PG_Rect(175,130,26,26),"LineEdit",2);
    leAvReducted_->SetValidKeys("0123456789");
    leAvReducted_->SetText(to_string(player_->getAvReducted()).c_str());
    leAvReducted_->sigEditEnd.connect(slot(*this, &InjuriesDialog::handleEditCharacteristicReducted));

    lblAgReducted_ = new PG_Label(this, PG_Rect(20,160,150,20), gettext("Number of -1 AG (Max 2):"));
    leAgReducted_ = new PG_LineEdit(this,PG_Rect(175,160,26,26),"LineEdit",2);
    leAgReducted_->SetValidKeys("0123456789");
    leAgReducted_->SetText(to_string(player_->getAgReducted()).c_str());
    leAgReducted_->sigEditEnd.connect(slot(*this, &InjuriesDialog::handleEditCharacteristicReducted));

    lblStReducted_ = new PG_Label(this, PG_Rect(20,190,150,20), gettext("Number of -1 ST (Max 2):"));
    leStReducted_ = new PG_LineEdit(this,PG_Rect(175,190,26,26),"LineEdit",2);
    leStReducted_->SetValidKeys("0123456789");
    leStReducted_->SetText(to_string(player_->getStReducted()).c_str());
    leStReducted_->sigEditEnd.connect(slot(*this, &InjuriesDialog::handleEditCharacteristicReducted));

   
    Init(style);

    SetTransparency(0, false);
    SetFontColor(black, true);
    SetSimpleBackground(true);
    SetBackgroundColor(white);
    SetTitlebarColor(white);
    Update();
}


template<typename T>
std::string InjuriesDialog::to_string( const T & Value )
{
    // utiliser un flux de sortie pour créer la chaîne
    std::ostringstream oss;
    // écrire la valeur dans le flux
    oss << Value;
    // renvoyer une string
    return oss.str();
}

InjuriesDialog::~InjuriesDialog()
{
    delete btnOk_;
    delete btnCancel_;
    delete cbtnMissNextMatch_;
    delete lblNigglingInjuries_;
    delete lblMaReducted_;
    delete lblAvReducted_;
    delete lblAgReducted_;
    delete lblStReducted_;
    delete leNigglingInjuries_;
    delete leMaReducted_;
    delete leAvReducted_;
    delete leAgReducted_;
    delete leStReducted_;
}

void InjuriesDialog::Init(const char* style) {
    LoadThemeStyle(style);
}

void InjuriesDialog::LoadThemeStyle(const char* widgettype) {
    PG_Window::LoadThemeStyle(widgettype);

    btnOk_->LoadThemeStyle(widgettype, "Button1");
    if(btnCancel_) {
        btnCancel_->LoadThemeStyle(widgettype, "Button2");
    }
}

bool InjuriesDialog::handleButton(PG_Button* button) {
    //Set Buttonflag to ButtonID
    button->SetInputFocus();
    SetModalStatus(button->GetID());
    QuitModal();
    return true;
}

bool InjuriesDialog::handleEditCharacteristicReducted(PG_LineEdit* edit)
{
    if (atoi(edit->GetText()) > 2)
    {
       displayError(gettext("Maximum value is 2"));
       edit->SetText("0");
    }
    
    return true;
}

bool InjuriesDialog::getMissNextMatch() { return cbtnMissNextMatch_->GetPressed(); }
int InjuriesDialog::getNigglingInjuries() { return atoi(leNigglingInjuries_->GetText()); }
int InjuriesDialog::getMaReducted() { return atoi(leMaReducted_->GetText()); }
int InjuriesDialog::getAvReducted() { return atoi(leAvReducted_->GetText()); }
int InjuriesDialog::getAgReducted() { return atoi(leAgReducted_->GetText()); }
int InjuriesDialog::getStReducted() { return atoi(leStReducted_->GetText()); }


void InjuriesDialog::displayError(const char* msg)
{
      PG_MessageBox msgbox(parent_, 
      PG_Rect(200,50,240,100), gettext("Error"), msg, 
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

