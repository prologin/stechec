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
#ifndef PLAYER_HH_
#define PLAYER_HH_

#include <vector>
#include <string>

#include "Position.hh"

class Team;

class Player
{
public:
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
    Player(Team* team);
	virtual ~Player();
    
    void reset();
    
    // -----------------------------------------------------------------------
    //  Getters & Setters
    // -----------------------------------------------------------------------
    
    Team* getTeam();
    const char* getName();
    void setName(const char* name);  
    const char* getPositionTitle();
    void setPosition(const char* posistionTitle);
    Position* getPosition();
    int getMovementAllowance();
    void setMovementAllowance(int ma);
    int getStrength();
    void setStrength(int st);
    int getAgility();
    void setAgility(int ag);
    int getArmourValue();
    void setArmourValue(int av);
    
    // Return all the player's skills (normal + double)
    std::vector <const char*> getSkills();
    
    std::vector <const char*> getSkillsNormal();
    std::vector <const char*> getSkillsDouble();
    
    void removeAllSkillsNormal();
    void removeAllSkillsDouble();
    void addSkillNormal(const char* skill);
    void addSkillDouble(const char* skill);
    const char* getSkillsAsString();    
    
    const char* getDisplay();
    void setDisplay(const char* display);
    
    bool getMissNextMatch();
    void setMissNextMatch(bool b);
    int getNigglingInjuries();
    void setNigglingInjuries(int ni);
    int getMaReducted();
    void setMaReducted(int n);
    int getAvReducted();
    void setAvReducted(int n);
    int getAgReducted();
    void setAgReducted(int n);
    int getStReducted();
    void setStReducted(int n);
    const char* getInjuriesAsString();
    
    int getCompletions();
    void setCompletions(int com);
    int getTouchDowns();
    void setTouchDowns(int td);
    int getInterceptions();
    void setInterceptions(int in);
    int getCasualties();
    void setCasualties(int ca);
    int getMostValuablePlayer();
    void setMostValuablePlayer(int mvp);
    int getStarPlayerPoints();
    long getValue();
    
private:
    // -----------------------------------------------------------------------
    //  Private methods
    // -----------------------------------------------------------------------
    bool validateCharacteristicUpdate(int newVal, int positionVal);

    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    Team* team_;
    std::string name_;
    std::string positionTitle_;
    int movementAllowance_;
    int strength_;
    int agility_;
    int armourValue_;
    std::vector<const char*> vSkills_;
    std::vector<const char*> vSkillsDouble_;
    
    bool missNextMatch_;
    int nigglingInjuries_;
    int maReducted_;
    int avReducted_;
    int agReducted_;
    int stReducted_;
     
    int completions_;
    int touchdowns_;
    int interceptions_;
    int casualties_;
    int mostValuablePlayer_;
    long value_;   
    std::string display_;
};
    

#endif /*PLAYER_HH_*/
