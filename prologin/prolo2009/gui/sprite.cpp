//
// sprite.cpp<2> for  in /home/lapie_t/prologin_gui
// 
// Made by Hazgar
// Login   <lapie_t@epitech.net>
// 
// Started on  Fri Feb 27 11:25:41 2009 Hazgar
// Last update Sat Apr 25 11:09:08 2009 Hazgar
//

#include <cstring>
#include "sprite.h"

Sprite::Sprite(const SpriteCfg &cfg)
  : Surface(cfg.path)
{
  this->_frames			= cfg.frames;
  this->_frameRate		= cfg.frameRate;
  this->_direction		= cfg.direction;
  this->_repeat			= cfg.repeat;
  this->_defField.x		= cfg.pos[0];
  this->_defField.y		= cfg.pos[1];
  this->_defField.w		= cfg.width;
  this->_defField.h		= cfg.height;
  this->_currentFrame		= 0;
  this->_currentFrameDuration	= 0;
  this->_field.w		= this->_defField.w;
  this->_field.h		= this->_defField.h;
}

Sprite::~Sprite(void)
{
}

Sprite::Sprite(const Sprite &right)
  : Surface(right)
{
  Sprite::operator=(right);
}

/* not used yep */
Sprite			&Sprite::operator=(const Sprite &right)
{
  Surface::operator=(right);
  return (*this);
}

void			Sprite::action()
{
  int			current_time;
 
  current_time = SDL_GetTicks();
  if (current_time >= this->_currentFrameDuration)
    {
      if (this->_currentFrame < this->_frames)
	this->_field.x = this->_currentFrame * this->_defField.w;
      else if (this->_repeat)
	{
	  this->_currentFrame = 0;
	  this->_field.x = this->_defField.x;
	}
      this->_currentFrameDuration = current_time + this->_frameRate;
      this->_currentFrame++;
    }
}

void			Sprite::setFrameRate(int frate)
{
  this->_frameRate = frate;
}

void			Sprite::setCurrentFrameDuration(int fduration)
{
  this->_currentFrameDuration = fduration;
}

void			Sprite::setCurrentFrame(int frame)
{
  this->_currentFrame = frame;
}

void			Sprite::setDirection(int dir)
{
  this->_direction = dir;
}

void			Sprite::setRepeat(bool repeat)
{
  this->_repeat = repeat;
}
