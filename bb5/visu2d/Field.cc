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

# include "Field.hh"

VisuField::VisuField(Api* api, Input& input)
  : VirtualScrollableSurface("VVisuField", input, Point(500, 600), Point(725, 1100)),
    api_(api),
    inp_(input),
    bg_("image/general/playground_0"),
    ball_("image/general/ball")
{
  addChild(&bg_);
}

VisuField::~VisuField()
{
}

void VisuField::setBallPos(const Point& pos)
{
  ball_.setPos(pos * 40);
  ball_.setZ(4);
  addChild(&ball_);
}

void VisuField::update()
{
  VirtualScrollableSurface::update();
}
