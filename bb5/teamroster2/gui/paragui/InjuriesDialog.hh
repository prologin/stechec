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
#ifndef INJURIESDIALOG_HH_
#define INJURIESDIALOG_HH_

#include "paragui.h"
#include "pgwindow.h"
#include "pgbutton.h"
#include "pglabel.h"
#include "pglineedit.h"
#include "pgcheckbutton.h"

#include "../../common/Player.hh"

class DECLSPEC InjuriesDialog : public PG_Window 
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
    InjuriesDialog(PG_Widget* parent, 
                const PG_Rect& r, const char* windowtitle,
                Player* player,
                const char* style="MessageBox");
    
	virtual ~InjuriesDialog();
    
    void LoadThemeStyle(const char* widgettype);

   /**
    * OBSOLETE - Waits for a button click and returns Button ID.
    * This method is obsolete. Please use the RunModal method.
    */
    inline int WaitForClick() {
        return RunModal();
    }
    
    bool getMissNextMatch();
    int getNigglingInjuries();
    int getMaReducted();
    int getAvReducted();
    int getAgReducted();
    int getStReducted();
    
private:
    template<typename T>
    std::string to_string( const T & Value );

    /**
     * Handle OK/Cancel buttons 
     */
    bool handleButton(PG_Button* button);    
    bool handleEditCharacteristicReducted(PG_LineEdit* edit);

    void displayError(const char* msg);
    void Init(const char* style);

    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    PG_Widget*      parent_;

    Player*         player_;

    PG_Button*      btnOk_;
    PG_Button*      btnCancel_;
    PG_CheckButton* cbtnMissNextMatch_;

    PG_Label*       lblNigglingInjuries_;
    PG_Label*       lblMaReducted_;
    PG_Label*       lblAvReducted_;
    PG_Label*       lblAgReducted_;
    PG_Label*       lblStReducted_;

    PG_LineEdit*    leNigglingInjuries_;
    PG_LineEdit*    leMaReducted_;
    PG_LineEdit*    leAvReducted_;
    PG_LineEdit*    leAgReducted_;
    PG_LineEdit*    leStReducted_;
};

#endif /*INJURIESDIALOG_HH_*/
