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
#include "InputEvent.hh"

InputEvent::InputEvent()
  : has_focus_(false),
    obj_rect_(-1, -1, 0, 0),
    container_(0, 0, 99999, 99999)  
{
  Input::addObject(this);
}

InputEvent::~InputEvent()
{
  Input::removeObject(this);
}

void InputEvent::setObjectRect(const Rect& real_rect)
{
  obj_rect_ = real_rect;
}

void InputEvent::setObjectContainer(const Rect& r)
{
  container_ = r;
}

void InputEvent::privMouseMove(Point pos)
{
  if (!container_.inside(pos))
    return;

  if (obj_rect_.inside(pos))
    {
      mouseMove(pos);
      has_focus_ = true;
    }
  else if (has_focus_)
    {
      mouseOut();
      has_focus_ = false;
    }
}
