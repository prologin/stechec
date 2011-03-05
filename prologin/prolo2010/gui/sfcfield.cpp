//
// sfcfield.cpp for  in /home/lapie_t/prologin_gui
// 
// Made by Hazgar
// Login   <lapie_t@epitech.net>
// 
// Started on  Fri Feb 27 11:00:43 2009 Hazgar
// Last update Fri Mar 13 15:58:37 2009 Hazgar
//

#include "surface.h"

SfcField::SfcField(void)
{
  this->_field.x = 0;
  this->_field.y = 0;
  this->_field.w = 0;
  this->_field.h = 0;
}

SfcField::~SfcField(void)
{
}

SfcField::SfcField(const SfcField &right)
{
  SfcField::operator=(right);
}

SfcField		&SfcField::operator=(const SfcField &right)
{
  this->_field.x = right._field.x;
  this->_field.y = right._field.y;
  this->_field.w = right._field.w;
  this->_field.h = right._field.h;
  return (*this);
}

void			SfcField::setPos(unsigned int x, unsigned int y)
{
  this->_field.x = x;
  this->_field.y = y;
}

void			SfcField::setSize(unsigned int width, unsigned int height)
{
  this->_field.w = width;
  this->_field.h = height;
}

void			*SfcField::getField(void)
{
  return (&(this->_field));
}

int			SfcField::getPosX(void) const
{
  return (this->_field.x);
}

int			SfcField::getPosY(void) const
{
  return (this->_field.y);
}

unsigned int		SfcField::getWidth(void) const
{
  return (this->_field.w);
}

unsigned int		SfcField::getHeight(void) const
{
  return (this->_field.h);
}
