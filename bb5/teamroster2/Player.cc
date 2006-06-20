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
const char* Player::getName() { return name_; }
    
void Player::setName(const char *name) { name_ = name; }

const char* Player::getPositionTitle() { return positionTitle_; }

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
            parseSkills(vPos[i].getSkills());
            value_ = vPos[i].getCost();

            break;
        }
    }
    
    if (posFound == false)
    {
        // selected position hasn't been found...
        //FIXME: Generate Exception here...
    }
}

int Player::getMovementAllowance() { return movementAllowance_; }

void Player::setMovementAllowance(int ma) { movementAllowance_ = ma; }

int Player::getStrength() { return strength_; }

void Player::setStrength(int st) { strength_ = st; }

int Player::getAgility() { return agility_; }

void Player::setAgility(int ag) { agility_ = ag;}

int Player::getArmourValue() { return armourValue_; }

void Player::setArmourValue(int av) { armourValue_ = av;}

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
    return value_; 
}

