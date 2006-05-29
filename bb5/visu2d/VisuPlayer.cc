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
#include "Panel.hh"
#include "Field.hh"
#include "Game.hh"
#include "VisuPlayer.hh"

VisuPlayer::VisuPlayer(Api* api, Panel& panel, VisuField& field, Game& game, const CPlayer* p)
  : api_(api),
    panel_(panel),
    field_(field),
    game_(game),
    p_(p),
    has_focus_(false),
    is_selected_(false),
    circle_("image/general/circle"),
    circle_selected_("image/general/circle_select"),
    player_num_("image/general/player_num")
{
  circle_.setZ(5);
  circle_.splitNbFrame(3, 1);
  circle_.anim(200);
  circle_selected_.setZ(6);
  player_num_.setZ(4);
  player_num_.splitNbFrame(16, 1);
  player_num_.setFrame(p->getId());
  field_.addChild(&player_num_);
}

VisuPlayer::~VisuPlayer()
{
}

void VisuPlayer::setPos(const Point& pos)
{
  Sprite::setPos(pos);
  circle_.setPos(pos);
  circle_selected_.setPos(pos);
  player_num_.setPos(pos + Point(3, 18));
}


void VisuPlayer::update()
{
  Input& inp = game_.getInput();

  // Update focus.
  bool now_focus = getAbsoluteRect().inside(inp.mouse_);

  // Mouse moves on player.
  if (!has_focus_ && now_focus)
    {
      parent_->addChild(&circle_);
      panel_.displayPlayerInfo(p_->getTeamId(), p_->getId());
    }

  // Mouse moves out of player.
  if (has_focus_ && !now_focus)
    {
      parent_->removeChild(&circle_);
    }

  // Click on player. Select him.
  if (has_focus_ && !is_selected_ && inp.button_pressed_[1])
    {
      field_.addChild(&circle_selected_);
      is_selected_ = true;
    }

  has_focus_ = now_focus;
  Sprite::update();
}
