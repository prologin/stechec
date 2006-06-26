#include "Position.hh"
#include <vector.h>
#include <string>

using std::string;

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
    
vector<const char*> Position::getSkills()
{
    return vSkills_;
}

const char* Position::getSkillsAsString() 
{
    string skills = "";
    
    for (unsigned int i=0; i<vSkills_.size(); i++)
    {
        skills += vSkills_[i];
        if (i<(vSkills_.size() - 1))
        {
            skills += ", ";    
        }
    }
	return skills.c_str();
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
     
