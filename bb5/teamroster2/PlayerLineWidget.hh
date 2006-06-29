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
#ifndef PLAYERLINEWIDGET_HH_
#define PLAYERLINEWIDGET_HH_

#include <vector>

#include "pgthemewidget.h"
#include "pglabel.h"
#include "pgdropdown.h"
#include "pglineedit.h"
#include "pglistboxbaseitem.h"

#include "TeamrosterApp.hh"
#include "Position.hh"
#include "Player.hh"

class PlayerLineWidget : public PG_ThemeWidget 
{
public:
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
	PlayerLineWidget(TeamrosterApp *app, PG_Widget *parent,PG_Rect rect, Player *player);
 	virtual ~PlayerLineWidget();

    void updatePositionsList(std::vector<Position> vPos);

    void updateModel(Player* player);
    void updateView();
   
private:
    void displayError(const char* msg);

    // -----------------------------------------------------------------------
    //  GUI Handlers
    // -----------------------------------------------------------------------
    bool handleEditName(PG_LineEdit* edit);
    bool handleSelectItemPosition(PG_ListBoxBaseItem* item);
    bool handleEditMa(PG_LineEdit* edit);
    bool handleEditSt(PG_LineEdit* edit);
    bool handleEditAg(PG_LineEdit* edit);
    bool handleEditAv(PG_LineEdit* edit);
 
    bool handleEditInj(PG_LineEdit* edit);

    bool handleEditComp(PG_LineEdit* edit);
    bool handleEditTd(PG_LineEdit* edit);
    bool handleEditInter(PG_LineEdit* edit);
    bool handleEditCasual(PG_LineEdit* edit);
    bool handleEditMvp(PG_LineEdit* edit);
    
    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    Player*      player_;
    
	PG_Widget*   parent_;
    
    PG_LineEdit* name_;
	PG_DropDown* position_;
	PG_LineEdit* movementAllowance_;
	PG_LineEdit* strength_;
	PG_LineEdit* agility_;
	PG_LineEdit* armourValue_;
	PG_LineEdit* skills_;
	PG_LineEdit* injuries_;
	PG_LineEdit* completions_;
	PG_LineEdit* touchdowns_;
	PG_LineEdit* interceptions_;
	PG_LineEdit* casualties_;
	PG_LineEdit* mostValuablePlayer_;
	PG_Label*    starPlayerPoints_;
	PG_Label*    value_;	
};

#endif /*PLAYERLINEWIDGET_HH_*/
