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

#include "Input.hh"
#include "InputTextSurface.hh"

InputTextSurface::InputTextSurface()
  : have_text_(false),
    index_(0)
{
}

InputTextSurface::InputTextSurface(const std::string& font_name, int surf_width)
  : TextSurface(font_name, surf_width, 25),
    have_text_(false),
    index_(0)
{
}

InputTextSurface::~InputTextSurface()
{
}

void InputTextSurface::acquireInput(const std::string& lock_id)
{
  Input& inp = *Input::getInst();

  lock_id_ = lock_id;
  inp.lockKeyboard(lock_id_);
}

bool InputTextSurface::isAcquireFinished() const
{
  return have_text_;
}

void InputTextSurface::resetAcquire()
{
  have_text_ = false;
}

void InputTextSurface::update()
{
  Input& inp = *Input::getInst();

  if (inp.isKeyboardLocked() && inp.unlockKeyboard(lock_id_))
    {
      // We have finished
      if (inp.key_pressed_[SDLK_ESCAPE] || inp.key_pressed_[SDLK_RETURN])
        {
          have_text_ = true;
          lock_id_ = "";
          TextSurface::update();
          return;
        }

      // Special keys
      if (inp.key_pressed_[SDLK_LEFT])
        if (--index_ < 0)
          index_ = 0;
      if (inp.key_pressed_[SDLK_RIGHT])
        if (++index_ >= (int)text_.size())
          index_--;
      if (inp.key_pressed_[SDLK_BACKSPACE] && index_ > 0)
        {
          std::string s(text_);
          LOG1("index: " << index_ << " text: " << text_ );
          setText(s.erase(index_ - 1, 1));
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
