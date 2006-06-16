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
#ifndef BBOWLWIDGET_HH_
#define BBOWLWIDGET_HH_

#include "pgthemewidget.h"
#include "pglabel.h"
#include "pglineedit.h"
#include "pglistbox.h"

#include "TeamrosterApp.hh"
#include "PlayerLineWidget.hh"

class BBowlWidget : public PG_ThemeWidget
{
public:
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
	BBowlWidget(TeamrosterApp *app, PG_Widget *parent,PG_Rect rect);
	virtual ~BBowlWidget();

    void evaluateTotalTeamCost();
    
private:
    // -----------------------------------------------------------------------
    //  GUI Handlers
    // -----------------------------------------------------------------------
    bool handleSelectItemRace(PG_ListBoxBaseItem* item);
    bool handleEditApothecary(PG_LineEdit* edit);
    bool handleEditReroll(PG_LineEdit* edit);
    bool handleEditFanFactor(PG_LineEdit* edit);
    bool handleEditAssistantCoach(PG_LineEdit* edit);
    bool handleEditCheerleader(PG_LineEdit* edit);

    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
	PlayerLineWidget* mywidget[16];

	PG_LineEdit* teamName_;
	PG_ListBox* race_;
	PG_Label* teamRating_;	
	PG_LineEdit* bank_;	
	PG_LineEdit* headCoach_;
	PG_LineEdit* reroll_;
	PG_Label* rerollCost_;
	PG_Label* rerollTotalCost_;
	PG_LineEdit* fanFactor_;
	PG_LineEdit* assistantCoach_;
	PG_LineEdit* cheerleader_;
	PG_LineEdit* apothecary_;
	PG_Label* fanFactorCost_;
	PG_Label* assistantCoachCost_;	
	PG_Label* cheerleaderCost_;	
	PG_Label* apothecaryCost_;	
	PG_Label* totalTeamValueCost_;	
	
};

#endif /*BBOWLWIDGET_HH_*/
