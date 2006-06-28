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
#ifndef TEAM_HH_
#define TEAM_HH_

#include "Race.hh"
#include "Player.hh"
#include <string>

#define TEAM_SIZE             16

#define APOTHECARY_COST       50000
#define FAN_FACTOR_COST       10000
#define CHEERLEADER_COST      10000
#define ASSISTANT_COACH_COST  10000

class Team
{
public:
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
	Team(Race *race);
	virtual ~Team();
    
    void validateTeam();
    
    // -----------------------------------------------------------------------
    //  Getters & Setters
    // -----------------------------------------------------------------------
    Player* getPlayer(unsigned int number);
    const char* getName();
    void setName(const char *name);
    const char* getEmblem();
    void setEmblem(const char *emblem);
    Race* getRace(); 
    void setRace(Race *race);
    long getBank();
    void setBank(long bank);
    const char* getHeadCoach();
    void setHeadCoach(const char *coach);
    int getReroll();
    void setReroll(int reroll);
    int getFanFactor();
    void setFanFactor(int fanFactor);
    int getAssistantCoach();
    void setAssistantCoach(int assistantCoach);
    int getCheerleader();
    void setCheerleader(int cheerleader);
    int getApothecary();
    void setApothecary(int apothecary);
    
    long getRerollUnitaryCost();
    long getRerollTotalCost();
    long getFanFactorCost();
    long getAssistantCoachCost();
    long getCheerleaderCost();
    long getApothecaryCost();
    long getTotalValueCost();
    
private:
    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    Player* players_[TEAM_SIZE];
    
    std::string teamName_;
    std::string emblem_;
    Race *race_;
    long bank_; 
    std::string headCoach_;
    int reroll_;
    int fanFactor_;
    int assistantCoach_;
    int cheerleader_;
    int apothecary_;
};

#endif /*TEAM_HH_*/
