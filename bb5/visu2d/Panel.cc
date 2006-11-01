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

#include "Api.hh"
#include "Game.hh"
#include "Panel.hh"

BEGIN_NS(sdlvisu);

Panel::Panel(Game& g)
  : VirtualSurface("VPanel", 300, 600),
    g_(g),
    bg_("image/panel/panel.jpg"),
    wheel_("image/panel/wheels.png"),
    showing_player_info_(false)
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
  if (showing_player_info_)
    hidePlayerInfo();

  g_.getApi()->selectTeam(team_id);
  const CPlayer* p = g_.getApi()->getPlayer(player_id);
  player_picture_ = Surface(std::string("image/figs/") + p->getPlayerPicture());
  player_picture_.setPos(45, 150);
  addChild(&player_picture_);
  showing_player_info_ = true;
}

void Panel::hidePlayerInfo()
{
  if (showing_player_info_)
    {
      removeChild(&player_picture_);
      showing_player_info_ = false;
    }
}

void Panel::update()
{
  VirtualSurface::update();
}

END_NS(sdlvisu);
