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
#ifndef INPUTDIALOG_HH_
#define INPUTDIALOG_HH_

#include "pglineedit.h"

#include "paragui.h"
#include "pglabel.h"
#include "pgwindow.h"
#include "pgrichedit.h"
#include "pgbutton.h"

class DECLSPEC InputDialog : public PG_Window 
{
public:
   /**
    * Creates a PopUp with 2 Buttons
    *     
    * @param parent Parent widget
    * @param r rectangle of PopUp
    * @param windowtitle Title of window
    * @param windowtext Text to appear in window
    * @param btn1 Struct PG_Rect to create Button 1
    * @param btn1text Text to appear in Button 1
    * @param btn2 Struct PG_Rect to create Button 2
    * @param btn2text Text to appear in Button 2
    * @param textalign Alignment for windowtext
    * @param style widgetstyle to use (default "MessageBox")
    */
    InputDialog(PG_Widget* parent, 
                const PG_Rect& r, const char* windowtitle, 
                const char* windowtext, 
                const PG_Rect& btn1, const char* btn1text, 
                const PG_Rect& btn2, const char* btn2text, 
                const PG_Rect& ledit, const char* lEditText, 
                PG_Label::TextAlign textalign = PG_Label::CENTER, 
                const char* style="MessageBox");

	virtual ~InputDialog();
    
    void LoadThemeStyle(const char* widgettype);

   /**
    * OBSOLETE - Waits for a button click and returns Button ID.
    * This method is obsolete. Please use the RunModal method.
    */
    inline int WaitForClick() {
        return RunModal();
    }
    
    const char* getText();
    
protected:
   /** 
    * Checks if button is pressed
    * @param button pointer to PG_BUtton
    */
    virtual bool handleButton(PG_Button* button);

    PG_Button* btnok_;
    PG_Button* btncancel_;

private:
    PG_LineEdit* lineEdit_;
    PG_RichEdit* textbox_;
    int msgalign_;

    void Init(const char* windowtext, int textalign, const char* style) ;
};

#endif /*INPUTDIALOG_HH_*/
