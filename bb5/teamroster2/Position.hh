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

#ifndef POSITION_HH_
#define POSITION_HH_
#include <vector.h>

class Position
{
public:
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
	Position();
	virtual ~Position();
	
    // -----------------------------------------------------------------------
    //  Getters & Setters
    // -----------------------------------------------------------------------
    int getQuantity();
    void setQuantity(int qty);
    const char* getTitle();
    void setTitle(const char* title);
    long getCost();
    void setCost(long cost);
    int getMovementAllowance();
    void setMovementAllowance(int ma);
    int getStrength();
    void setStrength(int st);
    int getAgility();
    void setAgility(int ag);
    int getArmourValue();
    void setArmourValue(int av);
    
    vector<const char*> getSkills();
    void addSkill(const char* skill);
    
    const char* getNormalSkills();
    void setNormalSkills(const char* normalSkills);
    const char* getDoubleSkills();
    void setDoubleSkills(const char* doubleSkills);
     
private: 
    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
 	int quantity_;
	const char* title_;
	long cost_;
	int movementAllowance_;
	int strength_;
	int agility_;
	int armourValue_;
    vector <const char*> vSkills_;
	const char* skills_;  // Skills list as string
	const char* normalSkills_;
	const char* doubleSkills_;

};

#endif /*POSITION_HH_*/
