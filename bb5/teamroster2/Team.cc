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
#include "Race.hh"
#include "Player.hh"
#include "Team.hh"

Team::Team(Race* race)
{
    teamName_ = "";
    bank_ = 0; 
    headCoach_ = "";
    reroll_ = 0;
    fanFactor_ = 0;
    assistantCoach_ = 0;
    cheerleader_ = 0;
    apothecary_ = 0;

    setRace(race);
    
    for (int i=0; i<TEAM_SIZE; i++)
    {
        players_[i] = new Player(this);
    }        
}

Team::~Team()
{
}


Player* Team::getPlayer(unsigned int number) { 
   if (number <= TEAM_SIZE)
   {
      return players_[number-1];
   }
   else 
   {
    // FIXME: Generate an exception here...
    return NULL;
   }
}

const char* Team::getName(){ return teamName_; }
void Team::setName(const char *name){ teamName_ = name; }
Race* Team::getRace(){ return race_; } 

void Team::setRace(Race *race) { 
  
  race_ = race;
 
   // Reset apothecary if the current race can't have apothecary
   if (race_->getApothecaryUse() == false)
   {
        apothecary_ = 0; 
   }
}

long Team::getBank() { return bank_; }
void Team::setBank(long bank) { bank_ = bank; }
const char* Team::getHeadCoach() { return headCoach_; }
void Team::setHeadCoach(const char *coach) { headCoach_ = coach; }
int Team::getReroll() { return reroll_; }
void Team::setReroll(int reroll) { reroll_ = reroll; }
int Team::getFanFactor() { return fanFactor_; }
void Team::setFanFactor(int fanFactor) { fanFactor_ = fanFactor; }
int Team::getAssistantCoach() { return assistantCoach_; }
void Team::setAssistantCoach(int assistantCoach) { assistantCoach_ = assistantCoach; }
int Team::getCheerleader() { return cheerleader_; }
void Team::setCheerleader(int cheerleader) { cheerleader_ = cheerleader; }
int Team::getApothecary() { return apothecary_; }

void Team::setApothecary(int apothecary) 
{
   if (race_->getApothecaryUse() == false)
   {
      // FIXME: Generate Exception
      apothecary_ = 0;
   }
   else 
   {
      apothecary_ = apothecary; 
   }
}   
 
long Team::getRerollUnitaryCost() { return race_->getRerollCost(); }
long Team::getRerollTotalCost() { return (reroll_ * race_->getRerollCost()); }
long Team::getFanFactorCost() { return (FAN_FACTOR_COST * fanFactor_); }
long Team::getAssistantCoachCost() { return (ASSISTANT_COACH_COST * assistantCoach_); }
long Team::getCheerleaderCost() { return (CHEERLEADER_COST * cheerleader_); }
long Team::getApothecaryCost() { return (APOTHECARY_COST * apothecary_); }

long Team::getTotalValueCost() 
{ 
  long cost = 0;
  
  // Evaluate the players costs
  for (int i=0; i<TEAM_SIZE; i++)
  {
    cost += players_[i]->getValue();  
  }
  
  // add additional costs
  cost += getRerollTotalCost();
  cost += getFanFactorCost();
  cost += getAssistantCoachCost();
  cost += getCheerleaderCost();
  cost += getApothecaryCost();
  
  return cost;
}
