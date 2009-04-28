//
// surface.cpp for  in /home/lapie_t/prologin_gui
// 
// Made by Hazgar
// Login   <lapie_t@epitech.net>
// 
// Started on  Fri Feb 27 10:48:30 2009 Hazgar
// Last update Tue Apr 28 17:13:07 2009 user
//

#include <SDL_image.h>
#include "surface.h"

Surface::Surface(void *sfc)
{
  this->_sfc = static_cast<SDL_Surface*>(sfc);
  if (this->_sfc == NULL)
    throw "Surface load error";
  this->InitField();
}

Surface::Surface(const char *path)
{
  this->_sfc = IMG_Load(path);
  if (this->_sfc == NULL)
    throw "Surface load error";
  this->InitField();
}

Surface::~Surface(void)
{
  if (this->_sfc != NULL)
    SDL_FreeSurface(this->_sfc);
}

Surface::Surface(const Surface &right)
  : SfcField::SfcField()
{
  Surface::operator=(right);
}

void		Surface::InitField(void)
{
  this->_field.w = this->_sfc->w;
  this->_field.h = this->_sfc->h;
}

/* not used yet */
Surface		&Surface::operator=(const Surface &right)
{
  (void)right;
  return (*this);
}

void		*Surface::getSurface(void) const
{
  return (this->_sfc);
}

void		Surface::Blit(Surface &dst, SfcField &pos)
{
  SDL_Rect	*dst_pos;

  dst_pos = static_cast<SDL_Rect*>(pos.getField());
  if (SDL_BlitSurface(this->_sfc, &(this->_field), dst._sfc, dst_pos) == -1)
    throw "Surface blit error";
}
