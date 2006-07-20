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
#ifndef SKILLSDIALOG_HH_
#define SKILLSDIALOG_HH_

#include <vector>
#include <string>

#include "pglineedit.h"

#include "paragui.h"
#include "pglabel.h"
#include "pgwindow.h"
#include "pgbutton.h"
#include "pglistbox.h"

#include "../../common/Position.hh"
#include "../../common/Player.hh"

class DECLSPEC SkillsDialog : public PG_Window 
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
    SkillsDialog(PG_Widget* parent, 
                const PG_Rect& r, const char* windowtitle,
                Player* player,
                const char* style="MessageBox");

    virtual ~SkillsDialog();
    
    void LoadThemeStyle(const char* widgettype);

   /**
    * OBSOLETE - Waits for a button click and returns Button ID.
    * This method is obsolete. Please use the RunModal method.
    */
    inline int WaitForClick() {
        return RunModal();
    }
    
    // Return the list of the player's skills
    std::vector<std::string> getPlayerSkills();
    
    // Return the list of the player's skills double
    std::vector<std::string> getPlayerSkillsDouble();
    
protected:
   /** 
    * Checks if button is pressed
    * @param button pointer to PG_BUtton
    */
    virtual bool handleButton(PG_Button* button);
    
    bool handleButtonAddClick();
    bool handleButtonRemoveClick();
    bool handleButtonAddDoubleClick();
    bool handleButtonRemoveDoubleClick();
   
    PG_Button* btnOk_;
    PG_Button* btnCancel_;

    PG_Button* btnAdd_;
    PG_Button* btnRemove_;
    PG_Button* btnAddDouble_;
    PG_Button* btnRemoveDouble_;
    
private:
    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    Player*      player_;

    PG_ListBox* selectedSkills_;
    PG_ListBox* availableSkills_;
    
    PG_ListBox* selectedSkillsDouble_;
    PG_ListBox* availableSkillsDouble_;
    PG_Label*   l1_;
    PG_Label*   l2_;
    PG_Label*   l3_;
    PG_Label*   l4_;

    void Init(const char* style);
    void FillAvailableSkills();
    void InsertSkillsFromVector(std::vector<std::string> v, PG_ListBox* list);
    bool IsPlayerSkill(const char* s);
    
};
#endif /*SKILLSDIALOG_HH_*/
