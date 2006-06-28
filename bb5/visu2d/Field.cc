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
#include "Field.hh"

VisuField::VisuField(Game& g)
  : VirtualScrollableSurface("VVisuField", g.getInput(), Point(500, 600), Point(725, 1100)),
    g_(g),
    bg_("image/general/playground_0"),
    ball_("image/general/ball"),
    doing_kickoff_(false)
{
  addChild(&bg_);
  addChild(&ball_);
  ball_.setZ(5);
  ball_.hide();
}

VisuField::~VisuField()
{
}

void VisuField::playerDoingKickoff()
{
  doing_kickoff_ = true;
  ball_.setPos(g_.getInput().mouse_ - getAbsolutePos() - Point(30, 30));
  ball_.show();
}

void VisuField::setBallPos(const Point& pos)
{
  if (ball_.isShown())
    ball_.move(pos * 40, 25.);
  else
    {
      ball_.setPos(pos * 40);
      ball_.show();
    }
}

void VisuField::update()
{
  Input& inp = g_.getInput();

  if (doing_kickoff_)
    ball_.setPos(g_.getInput().mouse_ - getAbsolutePos() - Point(30, 30));
  
  if (doing_kickoff_ && inp.button_pressed_[1])
    {
      Point to((inp.mouse_ - getAbsolutePos()) / 40);
      ball_.setPos(to * 40);
      g_.getApi()->doPlaceBall(to);
      doing_kickoff_ = false;
    }

  VirtualScrollableSurface::update();
}
