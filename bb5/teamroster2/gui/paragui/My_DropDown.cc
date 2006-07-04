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
#include "pgdropdown.h"
#include "pglineedit.h"
#include "pgbutton.h"

#include "My_DropDown.hh"

My_DropDown::My_DropDown(PG_Widget* parent, const PG_Rect& r, int id, const char* style) : PG_DropDown(parent, r, id, style)
{
	PG_Rect rect(0, 0, r.my_width - 15, r.my_height);
	my_EditBox->MoveWidget(rect);

	PG_Rect rbutton(abs(r.my_width - 15), 0, 15, r.my_height);
	my_DropButton->MoveWidget(rbutton);
}

My_DropDown::~My_DropDown()
{
}
