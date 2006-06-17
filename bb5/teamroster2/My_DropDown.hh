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
#ifndef MY_DROPDOWN_HH_
#define MY_DROPDOWN_HH_

#include "pgdropdown.h"

class My_DropDown : public PG_DropDown
{
public:
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
	My_DropDown(PG_Widget* parent, const PG_Rect& r = PG_Rect::null, int id = -1, const char* style="DropDown");
	virtual ~My_DropDown();
};

#endif /*MY_DROPDOWN_HH_*/
