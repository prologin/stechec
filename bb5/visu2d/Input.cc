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

const Input* Input::getInst()
{
  return inst_;
}

void Input::addObject(InputEvent* obj)
{
  assert(inst_ != NULL);
  inst_->object_.push_back(obj);
}

void Input::removeObject(InputEvent* obj)
{
  assert(inst_ != NULL);
  inst_->object_.erase(std::remove(inst_->object_.begin(), inst_->object_.end(), obj),
                       inst_->object_.end());
}

void Input::updateObjects()
{
  if (mouse_delta_ != Point(0, 0))
    std::for_each(object_.begin(), object_.end(),
                  std::bind2nd(std::mem_fun(&InputEvent::privMouseMove),
                               mouse_));
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
  for (int i = 0; i < 256; i++)
    key_pressed_[i] = false;
  for (int i = 0; i < 10; i++)
    button_pressed_[i] = false;
  modifier_pressed_ = 0;
  mouse_delta_ = Point(0, 0);
}

void Input::update(SDL_Event& event)
{
  switch (event.type)
    {
    case SDL_KEYDOWN:
      key_pressed_[event.key.keysym.sym] = true;
      key_[event.key.keysym.sym] = true;
      modifier_pressed_ = event.key.keysym.mod & ~modifier_;
      modifier_ = event.key.keysym.mod;
      break;

    case SDL_KEYUP:
      key_[event.key.keysym.sym] = false;
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
