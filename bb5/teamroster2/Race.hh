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
#ifndef RACE_HH_
#define RACE_HH_

#include <vector.h>
#include "Position.hh"

class Race
{
public:
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
	Race();
	virtual ~Race();
	
    // -----------------------------------------------------------------------
    //  Getters & Setters
    // -----------------------------------------------------------------------
	const char* getName();
	void setName(char* name);
    const char* getEmblem();
    void setEmblem(char* emblem);
	bool getApothecaryUse();
	void setApothecaryUse(bool use);
    long getRerollCost();
    void setRerollCost(long cost);
    int getRerollQuantity();
    void setRerollQuantity(int qty);
    const char* getBackground();
	void setBackground(char* bg);
	vector<Position> getPositions();
    Position* getPosition(const char* title);
	void addPosition(Position pos);
	
private:
    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    char* name_;
    char* emblem_;
    bool apothecaryUse_;
    long rerollCost_;
    int rerollQuantity_;
    char* background_;
    vector<Position> vPos_;
};


#endif /*RACE_HH_*/
