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
#include "BackgroundDialog.hh"

#include <libintl.h>


BackgroundDialog::BackgroundDialog(PG_Widget* parent, const PG_Rect& r, const char* windowtitle, const char* background, const char* style) : 
PG_Window(parent, r, windowtitle, MODAL) 
{ 
    parent_ = parent;
    
    PG_Color white(255,255,255);  
    PG_Color black(0,0,0);
    
    btnOk_ = new PG_Button(this, PG_Rect(100, 235, 30, 20), "OK");
    btnOk_->SetID(1);
    btnOk_->sigClick.connect(slot(*this, &BackgroundDialog::handleButton));
    
    btnCancel_ = new PG_Button(this, PG_Rect(170, 235, 60, 20), gettext("CANCEL"));
    btnCancel_->SetID(2);
    btnCancel_->sigClick.connect(slot(*this, &BackgroundDialog::handleButton));

    multiLEdit_ = new PG_MultiLineEdit(this, PG_Rect(10, 50, 290, 150));
    multiLEdit_->SetText(background);

    Init(style);

    SetTransparency(0, false);
    SetFontColor(black, true);
    SetSimpleBackground(true);
    SetBackgroundColor(white);
    SetTitlebarColor(white);
    Update();
}

BackgroundDialog::~BackgroundDialog()
{
    delete btnOk_;
    delete btnCancel_;
}

void BackgroundDialog::Init(const char* style) {
    LoadThemeStyle(style);
}

void BackgroundDialog::LoadThemeStyle(const char* widgettype) {
    PG_Window::LoadThemeStyle(widgettype);

    btnOk_->LoadThemeStyle(widgettype, "Button1");
    if(btnCancel_) {
        btnCancel_->LoadThemeStyle(widgettype, "Button2");
    }
}

bool BackgroundDialog::handleButton(PG_Button* button) {
    //Set Buttonflag to ButtonID
    button->SetInputFocus();
    SetModalStatus(button->GetID());
    QuitModal();
    return true;
}

const char* BackgroundDialog::getBackground()
{
    return multiLEdit_->GetText();
}
