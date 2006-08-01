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
#ifndef BACKGROUNDDIALOG_HH_
#define BACKGROUNDDIALOG_HH_

#include "paragui.h"
#include "pgwindow.h"
#include "pgbutton.h"
#include "pgmultilineedit.h"

class DECLSPEC BackgroundDialog  : public PG_Window 
{
public:
   /**
    * Creates a dialog box to manage skills for a specific player.
    *     
    * @param parent Parent widget
    * @param r rectangle of PopUp
    * @param windowtitle Title of window
    * @param player Player 
    * @param style widgetstyle to use (default "MessageBox")
    */
    BackgroundDialog(PG_Widget* parent, 
                const PG_Rect& r, const char* windowtitle,
                const char* background,
                const char* style="MessageBox");
    
    virtual ~BackgroundDialog();
    
    void LoadThemeStyle(const char* widgettype);

   /**
    * OBSOLETE - Waits for a button click and returns Button ID.
    * This method is obsolete. Please use the RunModal method.
    */
    inline int WaitForClick() {
        return RunModal();
    }
    
    const char* getBackground();

private:  
    /**
     * Handle OK/Cancel buttons 
     */
    bool handleButton(PG_Button* button);    

    void Init(const char* style);

    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    PG_Widget*         parent_;

    PG_Button*         btnOk_;
    PG_Button*         btnCancel_;
    
    PG_MultiLineEdit*  multiLEdit_;
};

#endif /*BACKGROUNDDIALOG_HH_*/