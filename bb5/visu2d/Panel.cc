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

# include "Panel.hh"

Panel::Panel(Api* api, Input& input)
  : VirtualSurface("VPanel", 300, 600),
    api_(api),
    inp_(input),
    bg_("image/panel/panel"),
    wheel_("image/panel/wheels")
{
  setPos(Point(500, 0));
  addChild(&bg_);

  wheel_.setZ(1);
  wheel_.setPos(113, 506);
  wheel_.splitNbFrame(13, 1);
  wheel_.anim(100);
  addChild(&wheel_);
}

Panel::~Panel()
{
}

void Panel::displayPlayerInfo(int team_id, int player_id)
{
  api_->switchTeam(team_id);
  const CPlayer* p = api_->getPlayer(player_id);
  player_picture_ = Surface(p->getPlayerPicture());
  addChild(&player_picture_);
}

void Panel::hidePlayerInfo()
{
  
}

void Panel::update()
{
  VirtualSurface::update();
}
