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

Input* Input::inst_ = NULL;

Input::Input()
{
  // There should be only one instance.
  assert(inst_ == NULL);
  inst_ = this;

  reset();
}

Input::~Input()
{
  inst_ = NULL;
}

Input* Input::getInst()
{
  return inst_;
}

bool Input::isModDown(int mod) const
{
  return modifier_ & mod;
}

bool Input::isModPressed(int mod) const
{
  return modifier_pressed_ & mod;
}

void Input::reset()
{
  for (int i = 0; i < SDLK_LAST; i++)
    {
      key_pressed_[i] = false;
      key_pressed_lock_[i] = false;
    }
  string_ = "";
  string_lock_ = "";
  for (int i = 0; i < 10; i++)
    button_pressed_[i] = false;
  modifier_pressed_ = 0;
  mouse_delta_ = Point(0, 0);
}

void Input::lockKeyboard(const std::string& lock_id)
{
  if (lock_id == "")
    return;
  lock_id_ = lock_id;
  for (int i = 0; i < SDLK_LAST; i++)
    {
      key_pressed_lock_[i] = key_pressed_[i];
      key_pressed_[i] = false;
      key_lock_[i] = key_[i];
      key_[i] = false;
    }
  string_lock_ = string_;
  string_ = "";
}

bool Input::unlockKeyboard(const std::string& lock_id)
{
  if (lock_id_ == "" || lock_id != lock_id_)
    return false;
  for (int i = 0; i < SDLK_LAST; i++)
    {
      key_pressed_[i] = key_pressed_lock_[i];
      key_[i] = key_lock_[i];
    }
  string_ = string_lock_;
  lock_id_ = "";
  return true;
}

bool Input::isKeyboardLocked() const
{
  return lock_id_ != "";
}

const std::string& Input::getString() const
{
  return string_;
}

void Input::update(SDL_Event& event)
{
  switch (event.type)
    {
    case SDL_KEYDOWN:
      if (lock_id_ == "")
        {
          key_pressed_[event.key.keysym.sym] = true;
          key_[event.key.keysym.sym] = true;
          if (isascii(event.key.keysym.sym))
            string_ = string_ + (char)event.key.keysym.sym;
        }
      else
        {
          key_pressed_lock_[event.key.keysym.sym] = true;
          key_lock_[event.key.keysym.sym] = true;
          if (isascii(event.key.keysym.sym))
            string_lock_ = string_lock_ + (char)event.key.keysym.sym;
        }
      modifier_pressed_ = event.key.keysym.mod & ~modifier_;
      modifier_ = event.key.keysym.mod;
      break;

    case SDL_KEYUP:
      if (lock_id_ == "")
        key_[event.key.keysym.sym] = false;
      else
        key_lock_[event.key.keysym.sym] = false;
      modifier_ = event.key.keysym.mod;
      break;

    case SDL_MOUSEBUTTONDOWN:
      assert(event.button.button < 10);
      button_[event.button.button] = true;
      button_pressed_[event.button.button] = true;
      break;

    case SDL_MOUSEBUTTONUP:
      button_[event.button.button] = false;
      break;

    case SDL_MOUSEMOTION:
      mouse_ = Point(event.motion.x, event.motion.y);
      mouse_delta_ = Point(event.motion.xrel, event.motion.yrel);
      break;
    }
}
