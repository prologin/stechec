/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
**
** Copyright (C) 2006, 2007 The TBT Team.
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

#include <SDL_ttf.h>
#include <SDL_gfxPrimitives.h>

#include "Input.hh"
#include "InputTextSurface.hh"

InputTextSurface::InputTextSurface()
  : have_text_(false),
    show_cursor_(false),
    index_(0)
{
}

InputTextSurface::InputTextSurface(const std::string& font_name, int surf_width)
  : TextSurface(font_name, 16, surf_width, 35),
    have_text_(false),
    show_cursor_(false),
    index_(0)
{
}

InputTextSurface::~InputTextSurface()
{
}

void InputTextSurface::acquireInput(const std::string& lock_id)
{
  Input& inp = *Input::getInst();

  if (show_cursor_)
    return;

  if (!inp.lockKeyboard(lock_id))
    return;

  lock_id_ = lock_id;
  index_ = text_.size();
  content_changed_ = true;
}

bool InputTextSurface::isAcquireFinished() const
{
  return have_text_;
}

void InputTextSurface::resetAcquire()
{
  have_text_ = false;
}

// Draw cursor
void InputTextSurface::customTextRender(SDL_Surface* su, int line)
{
  if (!show_cursor_)
    return;

  std::string chunck;
  char curs[2] = {0};
  int width_before = 0;
  int width_cursor = 0;
  int height_cursor = 10;

  if (!lines_.empty())
    {
      chunck = lines_[0].substr(0, index_);
      curs[0] = lines_[0][index_];
      if (TTF_SizeText(font_, chunck.c_str(), &width_before, NULL) != 0)
	PRINT_AND_THROW(TTFError, "TTF_SizeText");
      if (TTF_SizeText(font_, curs, &width_cursor, &height_cursor) != 0)
	PRINT_AND_THROW(TTFError, "TTF_SizeText");
    }
  if (width_cursor < 1)
    width_cursor = 8;

  SDL_PixelFormat* dst_fmt = surf_->format;
  unsigned dst_x = std::min(width_before + 4, surf_->w);
  unsigned dst_y = 4;
  unsigned width = std::min(width_before + width_cursor + 4, surf_->w);
  unsigned height = std::min(25, surf_->h);
  unsigned* p_dst;
  unsigned src_alpha;
  unsigned x, y;

  SDL_LockSurface(surf_);
  for (y = dst_y; y < height; y++)
    {
      p_dst = (unsigned*)surf_->pixels + surf_->w * y + dst_x;
      for (x = dst_x; x < width; x++)
	{
	  src_alpha = (*p_dst & dst_fmt->Amask) >> dst_fmt->Ashift;
	  if (src_alpha < 150 || lines_.empty())
	    {
	      *p_dst = (255 << dst_fmt->Rshift) | (0 << dst_fmt->Gshift) |
		(255 << dst_fmt->Bshift) | (150 << dst_fmt->Ashift);
	    }
	  p_dst++;
	}
    }
  SDL_UnlockSurface(surf_);
}

void InputTextSurface::update()
{
  Input& inp = *Input::getInst();

  if (inp.isKeyboardLocked() && inp.unlockKeyboard(lock_id_))
    {
      show_cursor_ = true;

      // We have finished
      if (inp.key_pressed_[SDLK_ESCAPE] || inp.key_pressed_[SDLK_RETURN])
        {
          have_text_ = true;
	  show_cursor_ = false;
	  content_changed_ = true;
          lock_id_ = "";
          TextSurface::update();
          return;
        }

      // Special keys
      if (inp.key_pressed_[SDLK_LEFT])
	{
	  if (--index_ < 0)
	    index_ = 0;
	  else
	    content_changed_ = true;
	}
      else if (inp.key_pressed_[SDLK_RIGHT])
	{
	  if (++index_ > (int)text_.size())
	    index_--;
	  else
	    content_changed_ = true;
	}
      if (inp.key_pressed_[SDLK_BACKSPACE] && index_ > 0)
        {
          std::string s(text_);
          setText(s.erase(index_ - 1, 1));
          index_--;
        }
      else if (inp.key_pressed_[SDLK_DELETE] && index_ < text_.size())
	{
	  std::string s(text_);
          setText(s.erase(index_, 1));
	  if (index_ > (int)text_.size())
	    index_--;
	}
      else
        {
          // Add text
          const std::string& new_text = inp.getString();
          if (new_text != "")
            {
              std::string s(text_);
              setText(s.insert(index_, new_text));
              index_ += new_text.size();
            }
        }

      // Relock keyboard
      inp.lockKeyboard(lock_id_);
    }

  TextSurface::update();
}
