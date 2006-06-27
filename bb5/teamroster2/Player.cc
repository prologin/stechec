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
#include "Race.hh"
#include "Team.hh"
#include "Player.hh"
#include "InvalidParameterException.hh"

#include <stdlib.h>
#include <vector.h>


Player::Player(Team *team)
{
    team_ = team;
    reset();
}

Player::~Player()
{
    delete &vSkills_;
} 

void Player::reset()
{
    name_ = "";
    positionTitle_ = "";
    movementAllowance_ = 0;
    strength_ = 0;
    agility_ = 0;
    armourValue_ = 0;
    skillsStr_ = "";
    vSkills_.clear();
    injuries_ = "";   
    completions_ = 0;
    touchdowns_ = 0;
    interceptions_ = 0;
    casualties_ = 0;
    mostValuablePlayer_ = 0;
    value_ = 0;       
}

// -----------------------------------------------------------------------
//  Getters & Setters
// -----------------------------------------------------------------------
Team* Player::getTeam() { return team_; }

const char* Player::getName() { return name_; }
    
void Player::setName(const char *name) { name_ = name; }

const char* Player::getPositionTitle() { return positionTitle_; }

Position* Player::getPosition()
{
    return team_->getRace()->getPosition(positionTitle_);
}

/*
 * Update position and player's caracteristics 
 */
void Player::setPosition(const char *selectedPosition) 
{     
    Race* race = team_->getRace();
    vector<Position> vPos = race->getPositions();
    
    bool posFound = false;
    for (unsigned int i=0; i< vPos.size(); i++)
    {
        if (strcmp(vPos[i].getTitle(), selectedPosition) == 0)
        {
            // The position has been found 
            posFound = true;
            
            positionTitle_ = selectedPosition; 
            
            // We need to update also current user attributes according to the new position 
            movementAllowance_ = vPos[i].getMovementAllowance();
            strength_ = vPos[i].getStrength();
            agility_ = vPos[i].getAgility();
            armourValue_ = vPos[i].getArmourValue();
            parseSkills(vPos[i].getSkillsAsString());
            value_ = vPos[i].getCost();

            break;
        }
    }
    
    if (posFound == false)
    {
        // selected position hasn't been found...
        InvalidParameterException e("Position not found");
        throw (e);    
    }
}

int Player::getMovementAllowance() { return movementAllowance_; }

void Player::setMovementAllowance(int ma) { 
    if (validateCharacteristicUpdate(ma, getPosition()->getMovementAllowance()))
    {     
        movementAllowance_ = ma; 
    }
}

int Player::getStrength() { return strength_; }

void Player::setStrength(int st) { 
    if (validateCharacteristicUpdate(st, getPosition()->getStrength()))
    {     
        strength_ = st; 
    }
}

int Player::getAgility() { return agility_; }

void Player::setAgility(int ag) { 
    if (validateCharacteristicUpdate(ag, getPosition()->getAgility()))
    {     
        agility_ = ag;
    }
}

int Player::getArmourValue() { return armourValue_; }

void Player::setArmourValue(int av) { 
    if (validateCharacteristicUpdate(av, getPosition()->getArmourValue()))
    {     
        armourValue_ = av;
    }
}

vector <char*> Player::getSkills() { return vSkills_; }

const char* Player::getSkillsAsString() { return skillsStr_; }

/*
 * Parse coma sperated skills list 
 * @param skills = "skill1,skill2,skill3,etc."
 */
void Player::parseSkills(const char* skills) 
{ 
    // Store skills as String
    skillsStr_ = skills;
    
    vSkills_.clear(); 
    
    char strTmp[strlen(skills)]; 
    sprintf(strTmp, "%s", skills);
    
    char *result = NULL;
    result = strtok(strTmp, ",");
    while(result != NULL) 
    {
       vSkills_.push_back(result);
       result = strtok( NULL, "," );
    }    
}

const char* Player::getInjuries() { return injuries_; }

void Player::setInjuries(const char* inj) { injuries_ = inj; }
 
int Player::getCompletions() { return completions_; }

void Player::setCompletions(int com) { completions_ = com; }

int Player::getTouchDowns() { return touchdowns_; }

void Player::setTouchDowns(int td) { touchdowns_ = td; }

int Player::getInterceptions() { return interceptions_; }

void Player::setInterceptions(int in) { interceptions_ = in; }

int Player::getCasualties() { return casualties_; }

void Player::setCasualties(int ca) { casualties_ = ca; }

int Player::getMostValuablePlayer() { return mostValuablePlayer_; }

void Player::setMostValuablePlayer(int mvp) { mostValuablePlayer_ = mvp; }

int Player::getStarPlayerPoints() 
{ 
  int spp = completions_ + 
            2*(casualties_ + interceptions_) +
            3*touchdowns_ + 
            5*mostValuablePlayer_;

  return spp;
}

long Player::getValue() 
{ 
    Position* pos;
    try
    {
        pos = getPosition();
    }
    catch (InvalidParameterException &ex)
    {
        // If no position is defined return 0
        return 0;
    }
    
    // Initialize player value with the position cost
    long playerValue = value_;
    
    // Eventually add value due to movement Allowance (see LBR5 p.34) 
    int dMa = movementAllowance_ - pos->getMovementAllowance();
    if (dMa > 0)
    {
       playerValue += dMa*30000;
    }
    
    // Eventually add value due to armour value (see LBR5 p.34) 
    int dAv = armourValue_ - pos->getArmourValue();
    if (dAv > 0)
    {
       playerValue += dAv*30000;
    }

    // Eventually add value due to agility (see LBR5 p.34) 
    int dAg = agility_ - pos->getAgility();
    if (dAg > 0)
    {
       playerValue += dAg*40000;
    }

    // Eventually add value due to strength (see LBR5 p.34) 
    int dSt = strength_ - pos->getStrength();
    if (dSt > 0)
    {
       playerValue += dSt*50000;
    }
    
    // Eventually add value due to new skill
//FIXME: to be added after skill management.

    // Eventually add value due to new skill with double
//FIXME: to be added after skill ñmanagement.

    return playerValue; 
}

bool Player::validateCharacteristicUpdate(int newVal, int positionVal)
{
    if (strcmp(positionTitle_,"") == 0)
    {
        InvalidParameterException e("Position must be fixed to update a charcateristic.");
        throw (e);            
    }
    else if (newVal > 10)
    {
        InvalidParameterException e("Characteristics can never be greater than 10.");
        throw (e);    
    } else if (abs(newVal - positionVal) > 2) {
        InvalidParameterException e("Characteristics can never be raised or reduce by more than 2 points.");
        throw (e);            
    } 
    
    return true; 
}
