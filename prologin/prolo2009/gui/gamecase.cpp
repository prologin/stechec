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
  this->_price = 0;
  this->_owner = OWN_TOWNHALL;
}

GameCase::GameCase(t_landscape type, int price, t_owner owner)
{
  this->_type = type;
  this->_price = price;
  this->_owner = owner;
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
  this->_price = right._price;
  this->_owner = right._owner;
  return (*this);
}

void			GameCase::setType(t_landscape type)
{
  this->_type = type;
}

void			GameCase::setPrice(int price)
{
  this->_price = price;
}

void			GameCase::setOwner(t_owner owner)
{
  this->_owner = owner;
}

t_landscape		GameCase::getType(void) const
{
  return (this->_type);
}

int			GameCase::getPrice(void) const
{
  return (this->_price);
}

t_owner			GameCase::getOwner(void) const
{
  return (this->_owner);
}
