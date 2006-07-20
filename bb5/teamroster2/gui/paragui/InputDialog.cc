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
#include "InputDialog.hh"

#include "pglog.h"
#include "pgwindow.h"
#include "pgrichedit.h"

InputDialog::InputDialog(PG_Widget* parent, const PG_Rect& r, const char* windowtitle, const char* windowtext, const PG_Rect& btn1, const char* btn1text, const PG_Rect& btn2, const char* btn2text, const PG_Rect& ledit, const char* lEditText, PG_Label::TextAlign textalign, const char* style) :
PG_Window(parent, r, windowtitle, MODAL) {

    lineEdit_ = new PG_LineEdit(this, ledit, lEditText);
    lineEdit_->SetText(lEditText);

    btnok_ = new PG_Button(this, btn1, btn1text);
    btnok_->SetID(1);
    btnok_->sigClick.connect(slot(*this, &InputDialog::handleButton));
    
    btncancel_ = new PG_Button(this, btn2, btn2text);
    btncancel_->SetID(2);
    btncancel_->sigClick.connect(slot(*this, &InputDialog::handleButton));

    Init(windowtext, textalign, style);

    PG_Color white(255,255,255);  
    PG_Color black(0,0,0);
    SetTransparency(0, false);
    SetFontColor(black, true);
    SetSimpleBackground(true);
    SetBackgroundColor(white);
    SetTitlebarColor(white);
    Update();
}
    
InputDialog::~InputDialog()
{
    delete btnok_;
    delete btncancel_;
    delete lineEdit_;
}


const char* InputDialog::getText() {
    return lineEdit_->GetText();
}

void InputDialog::Init(const char* windowtext, int textalign, const char* style) {
    textbox_ = new PG_RichEdit(this, PG_Rect(10, 40, my_width-20, my_height-50));
    textbox_->SendToBack();
    textbox_->SetTransparency(255);
    textbox_->SetText(windowtext);

    msgalign_ = textalign;

    LoadThemeStyle(style);
}

void InputDialog::LoadThemeStyle(const char* widgettype) {
    PG_Window::LoadThemeStyle(widgettype);

    btnok_->LoadThemeStyle(widgettype, "Button1");
    if(btncancel_) {
        btncancel_->LoadThemeStyle(widgettype, "Button2");
    }
}

//Event
bool InputDialog::handleButton(PG_Button* button) {
    //Set Buttonflag to ButtonID
    button->SetInputFocus();
    SetModalStatus(button->GetID());
    QuitModal();
    return true;
}
