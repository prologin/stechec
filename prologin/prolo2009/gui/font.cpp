//
// font.cpp for  in /home/user/prolo_stechec/prologin/prolo2009/gui
// 
// Made by user
// Login   <user@epitech.net>
// 
// Started on  Tue Apr 28 14:25:18 2009 user
// Last update Thu Apr 30 14:29:00 2009 user
//

#include <cstring>
#include "font.h"

Font::Font(const char *path, int size)
  : Text("")
{
  this->_font = TTF_OpenFont(path, size);
  if (this->_font == NULL)
    throw "Error while loading TTF font";
  this->_sfc = NULL;
  memset(&(this->_color), 0, sizeof(this->_color));
}

Font::~Font(void)
{
}

Font::Font(const Font &right)
{
  Font::operator=(right);
}

Font			&Font::operator=(const Font &right)
{
  delete this->_sfc;

  this->_sfc = right._sfc;
  this->_font = right._font;
  this->_color.r = right._color.r;
  this->_color.g = right._color.g;
  this->_color.b = right._color.b;
  this->_color.unused = right._color.unused;
  return (*this);
}

void			Font::setText(std::string str)
{
  this->Text << str;
}

/* endianness not checked */
void			Font::setColor(unsigned int color)
{
  this->_color.unused = (color >> 24) & 0xFF;
  this->_color.r = (color >> 16) & 0xFF;
  this->_color.g = (color >> 8) & 0xFF;
  this->_color.b = color & 0xFF;
}

std::string		Font::getText(void) const
{
  return (this->Text.str());
}

/* endianness not checked */
unsigned int		Font::getColor(void) const
{
  unsigned int		color = 0;

  color |= this->_color.unused << 24;
  color |= this->_color.r << 16;
  color |= this->_color.g << 8;
  color |= this->_color.b;
  return (color);
}

void			*Font::getSurface(void) const
{
  if (this->_sfc != NULL)
    return (this->_sfc->getSurface());
  return (NULL);
}

void			Font::Blit(Surface &dst, SfcField &pos)
{
  SDL_Surface		*sfc;

  sfc = TTF_RenderText_Blended(this->_font, this->Text.str().c_str(), this->_color);
  if (sfc)
    {
      delete this->_sfc;
      //sfc->flags &= ~SDL_SRCALPHA;
      //sfc->flags |= SDL_SRCCOLORKEY;
      this->_sfc = new Surface(sfc);
    }
  if (this->_sfc != NULL)
    {
      pos.setPos(pos.getPosX() - (this->_sfc->getWidth() >> 1), pos.getPosY());
      this->_sfc->Blit(dst, pos);
    }
}

int			Font::getWidth(void) const
{
  if (this->_sfc)
    return (this->_sfc->getWidth());
  return (0);
}

int			Font::getHeight(void) const
{
  if (this->_sfc)
    return (this->_sfc->getHeight());
  return (0);
}
