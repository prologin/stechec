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
  : VirtualScrollableSurface("VVisuField", input, Point(500, 600), Point(725, 1050)),
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

void VisuField::addPlayer(int team_id, int player_id, const Point& pos)
{
  api_->switchTeam(team_id);  
  const CTeam* team = api_->getTeam();
  const CPlayer* p = team->getPlayerConst(player_id);
  Sprite& s = player_[team_id][player_id];
  s = Sprite("image/figs/amazon");
  s.splitSizeFrame(40, 40);
  s.setZ(3);
  s.setFrame(p->getPlayerPosition() * 2 + 1);
  s.setPos(pos * 40);
  LOG3("add player " << player_id << " at: " << s.getRect() << " pos: " << pos);
  addChild(&s);
}

void VisuField::setBallPos(const Point& pos)
{
  ball_.setPos(pos * 40);
  ball_.setZ(4);
  addChild(&ball_);
  LOG3("set ball at" << ball_.getRect());
}

void VisuField::update()
{
  VirtualScrollableSurface::update();
}
