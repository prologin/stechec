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
#include <iostream.h>
#include <string.h>
#include <vector.h>
#include "Race.hh"
#include "Position.hh"
#include "InvalidParameterException.hh"

Race::Race()
{
}

Race::~Race()
{
}

const char* Race::getName()
{
	return name_;
}

void Race::setName(char* name)
{
	
	name_ = name;
}

const char* Race::getEmblem()
{
    return emblem_;
}

void Race::setEmblem(char* emblem)
{
    emblem_  = emblem;
}

bool Race::getApothecaryUse()
{
	return apothecaryUse_;
}

void Race::setApothecaryUse(bool use)
{
	apothecaryUse_ = use;
}

long Race::getRerollCost()
{
	return rerollCost_;
}

void Race::setRerollCost(long cost)
{
	rerollCost_ = cost;
}

int Race::getRerollQuantity()
{
	return rerollQuantity_;
}

void Race::setRerollQuantity(int qty)
{
	rerollQuantity_ = qty;
}

const char* Race::getBackground()
{
	return background_;
}

void Race::setBackground(char* bg)
{
	background_ = bg;
}
	
vector<Position> Race::getPositions()
{
	return vPos_;
}

Position* Race::getPosition(const char *title)
{
    for (unsigned int i=0; i<vPos_.size(); i++)
    {
        if (strcmp(title, vPos_[i].getTitle()) == 0)
        {
            return &vPos_[i];
        }
    }
    
    // selected position hasn't been found...
    InvalidParameterException e("Position not found");
    throw (e);    
}

void Race::addPosition(Position pos)
{
	vPos_.push_back(pos);
}
	