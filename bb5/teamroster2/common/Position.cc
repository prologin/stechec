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
#include "Position.hh"
#include <vector>

Position::Position()
{
}

Position::~Position()
{
}

int Position::getQuantity()
{
	return quantity_;
}

void Position::setQuantity(int qty)
{ 
	quantity_ = qty;
}

const char* Position::getTitle()
{
	return title_;
}

void Position::setTitle(const char* title)
{
	title_ = title;
}
    
long Position::getCost()
{
	return cost_;
}

void Position::setCost(long cost)
{
	cost_ = cost;
}

int Position::getMovementAllowance()
{
	return movementAllowance_;
}

void Position::setMovementAllowance(int ma)
{
	movementAllowance_ = ma;
}

int Position::getStrength()
{
	return strength_;
}

void Position::setStrength(int st)
{
	strength_ = st;
}

int Position::getAgility() 
{
	return agility_;
}

void Position::setAgility(int ag)
{
	agility_ = ag;
}

int Position::getArmourValue()
{
	return armourValue_;
}

void Position::setArmourValue(int av)
{
	armourValue_ = av;
}
    
std::vector<const char*> Position::getSkills()
{
    return vSkills_;
}

void Position::addSkill(const char* skill)
{
	vSkills_.push_back(skill);
}

const char* Position::getNormalSkills() 
{
	return normalSkills_;
}

void Position::setNormalSkills(const char* normalSkills)
{
	normalSkills_ = normalSkills;
}

const char* Position::getDoubleSkills() 
{
	return doubleSkills_;
}

void Position::setDoubleSkills(const char* doubleSkills)
{
	doubleSkills_ = doubleSkills;
}
     
