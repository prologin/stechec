//
// gamecase.cpp for  in /home/lapie_t/prologin_gui
// 
// Made by Hazgar
// Login   <lapie_t@epitech.net>
// 
// Started on  Wed Apr 22 14:41:25 2009 Hazgar
// Last update Sat May  2 13:13:12 2009 user
//

#include "gamecase.h"

GameCase::GameCase(void)
{
  this->_type = LD_EMPTY;
}

GameCase::GameCase(t_landscape type)
{
  this->_type = type;
}

GameCase::~GameCase(void)
{
}

GameCase::GameCase(const GameCase &right)
{
  GameCase::operator=(right);
}

GameCase		&GameCase::operator=(const GameCase &right)
{
  this->_type = right._type;
  return (*this);
}

void			GameCase::setType(t_landscape type)
{
  this->_type = type;
}

t_landscape		GameCase::getType(void) const
{
  return (this->_type);
}
