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

#include "Game.hh"
#include "VisuPlayer.hh"
#include "ActionPopup.hh"

ActionPopup::ActionPopup(VirtualSurface& screen, Input& input)
  : screen_(screen),
    input_(input),
    vp_(NULL),
    show_(false)
{
  // FIXME: recreate when needed.
  create(120, 160);

  for (int i = 0; i < 12; i++)
    {
      sprite_[i].load("image/general/actions_on");
      sprite_[i].splitNbFrame(1, 12);
      sprite_[i].setFrame(i + 1);
      sprite_[i].setZ(6);
      sprite_[i].setPos(2000, 2000); // outside
      addChild(&sprite_[i]);
    }
}

ActionPopup::~ActionPopup()
{
  if (show_)
    screen_.removeChild(this);
}

void ActionPopup::setVisuPlayer(VisuPlayer* vp)
{
  vp_ = vp;

  // FIXME: get player capabilities, to display revelant menu
  sprite_[0].setPos(0, 0);
  sprite_[2].setPos(0, 40);
  sprite_[4].setPos(0, 80);
  sprite_[5].setPos(0, 120);

}

void ActionPopup::show(const Point& pos)
{
  if (vp_ == NULL)
    return;

  if (!show_)
    {
      screen_.addChild(this);
      redraw_all_ = true;
    }
  if (pos != getPos())
    {
      screen_.invalidate(getRect());
      setPos(pos);
      redraw_all_ = true;
    }
  show_ = true;
}

void ActionPopup::hide()
{
  if (show_)
    {
      screen_.removeChild(this);
      screen_.invalidate(getRect());
    }
  show_ = false;
}

void ActionPopup::update()
{
  bool have_focus = getAbsoluteRect().inside(input_.mouse_);

  if (have_focus && show_ && vp_ != NULL)
    {
      if (input_.button_pressed_[1])
        {
          vp_->action(eActMove);
        }
    }

  VirtualSurface::update();
}
