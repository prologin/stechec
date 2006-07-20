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
#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;

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
    display_="";
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

const char* Player::getName() { return name_.c_str(); }
    
void Player::setName(const char *name) { name_ = name; }

const char* Player::getPositionTitle() { return positionTitle_.c_str(); }

Position* Player::getPosition()
{
    return team_->getRace()->getPosition(positionTitle_.c_str());
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
            value_ = vPos[i].getCost();
            
            // We need to remove any old player specific skills
            vSkills_.clear();

            display_ = vPos[i].getDisplay();
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

/**
 * Return all player's skills : normal + double
 */
vector<const char*> Player::getSkills() 
{ 
    vector<const char*>* vTmp = new vector<const char*>();
    vTmp->clear();
    
    // Add all skills coming with the Position
    vector<const char*> skills = getPosition()->getSkills();
    for (unsigned int j=0; j<skills.size(); j++)
    {
        vTmp->push_back(skills[j]);
    }

    // Add all additional skills for the player
    for (unsigned int i=0; i<vSkills_.size(); i++)
    {
        vTmp->push_back(vSkills_[i]);
    }

    // Add all additional skills double for the player
    for (unsigned int i=0; i<vSkillsDouble_.size(); i++)
    {
        vTmp->push_back(vSkillsDouble_[i]);
    }

    return *vTmp; 
}

vector<const char*> Player::getSkillsNormal() 
{ 
    vector<const char*>* vTmp = new vector<const char*>();
    vTmp->clear();
    
    // Add all skills coming with the Position
    vector<const char*> skills = getPosition()->getSkills();
    for (unsigned int j=0; j<skills.size(); j++)
    {
        vTmp->push_back(skills[j]);
    }

    // Add all additional skills for the player
    for (unsigned int i=0; i<vSkills_.size(); i++)
    {
        vTmp->push_back(vSkills_[i]);
    }

    return *vTmp; 
}

vector<const char*> Player::getSkillsDouble() 
{ 
    vector<const char*>* vTmp = new vector<const char*>();
    vTmp->clear();
    
    // Add all additional skills double for the player
    for (unsigned int i=0; i<vSkillsDouble_.size(); i++)
    {
        vTmp->push_back(vSkillsDouble_[i]);
    }

    return *vTmp; 
}

/**
 * Used to add a new skill for the player other than the skill from his position
 */
void Player::addSkillNormal(const char* skill)
{
    // Avoid double skills...
    bool found = false;
    
    // Search the skills in the additional skills of the player
    for (unsigned int i=0; i<vSkills_.size(); i++)
    {
        if (strcmp(vSkills_[i], skill) == 0)
        {
            found = true;
            break;
        }        
    }
    
    // Search in skill coming from the position 
    if (!found) 
    {
        vector<const char*> skills = getPosition()->getSkills();
        for (unsigned int j=0; j<skills.size(); j++)
        {
            if (strcmp(skills[j], skill) == 0)
            {
                found = true;
                break;
            }   
        }     
    }
    
    // If the skill is not already present, just add it as additional skill.
    if (!found)
    {
//        std::cout << skill << " added" << std::endl;
        vSkills_.push_back(skill);
    }
}

/**
 * Used to add a new skill for the player other than the skill from his position
 */
void Player::addSkillDouble(const char* skill)
{
    // Avoid double skills...
    bool found = false;
    
    // Search the skills in the additional skills of the player
    for (unsigned int i=0; i<vSkillsDouble_.size(); i++)
    {
        if (strcmp(vSkillsDouble_[i], skill) == 0)
        {
            found = true;
            break;
        }        
    }
    
    // Search in skill coming from the position 
    if (!found) 
    {
        vector<const char*> skills = getPosition()->getSkills();
        for (unsigned int j=0; j<skills.size(); j++)
        {
            if (strcmp(skills[j], skill) == 0)
            {
                found = true;
                break;
            }   
        }     
    }
    
    // If the skill is not already present, just add it as additional skill.
    if (!found)
    {
//        std::cout << skill << " added" << std::endl;
        vSkillsDouble_.push_back(skill);
    }
}

const char* Player::getSkillsAsString() 
{
    string skills = "";
    
    if (positionTitle_.compare("") != 0)
    {
        vector<const char*> vSkills = getSkills();
        for (unsigned int i=0; i<vSkills.size(); i++)
        {
            skills += vSkills[i];
            if (i<(vSkills.size() - 1))
            {
                skills += ", ";    
            }
        }
    } 
    return skills.c_str();
}

void Player::removeAllSkillsNormal() { vSkills_.clear(); }
void Player::removeAllSkillsDouble() { vSkillsDouble_.clear(); }

const char* Player::getDisplay() { return display_.c_str(); }

void Player::setDisplay(const char* display) { display_ = display; }

const char* Player::getInjuries() { return injuries_.c_str(); }

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
    playerValue += vSkills_.size()*20000;

    // Eventually add value due to new skill with double
    playerValue += vSkillsDouble_.size()*30000;

    return playerValue; 
}

bool Player::validateCharacteristicUpdate(int newVal, int positionVal)
{
    if (positionTitle_.compare("") == 0)
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
