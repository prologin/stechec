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

ActionPopup::ActionPopup(Game& g)
  : VirtualSurface("ActionPopup", 120, 40 * 12),
    g_(g),
    vp_(NULL),
    show_(false)
{
  for (int i = 0; i < 12; i++)
    {
      sprite_[i].load("image/general/actions_on.jpg");
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
    g_.getScreen().removeChild(this);
}

void ActionPopup::setVisuPlayer(VisuPlayer* vp)
{
  vp_ = vp;

  // FIXME: get player capabilities, to display revelant menu
  sprite_[0].setPos(0, 0);
  sprite_[2].setPos(0, 40);
  sprite_[4].setPos(0, 80);
  sprite_[5].setPos(0, 120);
  setSize(Point(120, 40 * 4));
}

VisuPlayer* ActionPopup::getVisuPlayer() const
{
  return vp_;
}

void ActionPopup::show(const Point& pos)
{
  if (vp_ == NULL)
    return;

  if (!show_)
    {
      g_.getScreen().addChild(this);
      redraw_all_ = true;
    }
  if (pos != getPos())
    {
      g_.getScreen().invalidate(getRect());
      setPos(pos);
      redraw_all_ = true;
    }
  show_ = true;
}

void ActionPopup::hide()
{
  if (show_)
    {
      g_.getScreen().removeChild(this);
      g_.getScreen().invalidate(getRect());
    }
  show_ = false;
}

bool ActionPopup::isVisible() const
{
  return show_;
}

void ActionPopup::update()
{
  Input& input(g_.getInput());
  bool have_focus = getAbsoluteRect().inside(input.mouse_);

  // A player
  if (have_focus && show_ && vp_ != NULL)
    {
      if (input.button_pressed_[1])
        {
          int item = (input.mouse_.y - getRect().y) / 40;
          g_.addAction(eActMove);
        }
    }

  VirtualSurface::update();
}
