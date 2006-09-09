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

VisuPlayer::VisuPlayer(Api* api, Game& game, const CPlayer* p)
  : api_(api),
    game_(game),
    p_(p),
    has_focus_(false),
    is_selected_(false),
    circle_("image/general/circle.png"),
    circle_selected_("image/general/circle_select.png"),
    player_num_("image/general/player_num.png")
{
  circle_.setZ(5);
  circle_.splitNbFrame(3, 1);
  circle_.anim(200);
  circle_.hide();
  game_.getField().addChild(&circle_);

  circle_selected_.setZ(6);
  circle_selected_.hide();
  game_.getField().addChild(&circle_selected_);
  
  player_num_.setZ(4);
  player_num_.splitNbFrame(16, 1);
  player_num_.setFrame(p->getId());
  game_.getField().addChild(&player_num_);
}

VisuPlayer::~VisuPlayer()
{
}

void VisuPlayer::unselect()
{
  if (is_selected_)
    {
      is_selected_ = false;
      circle_selected_.hide();
    }
}

void VisuPlayer::action(eAction item)
{
  Point to((game_.getInput().mouse_ + game_.getField().getRealAbsoluteRect().getPos()) / 40);
  int player_id = p_->getId();
  
  switch (item)
    {
    case eActMove:
      LOG2("MOVE to " << to << " - send to api.");
      api_->doMovePlayer(player_id, to);
      break;
    case eActBlock:
      LOG2("BLOCK to " << to << " - not implemented yet");
      break;
    case eActThrow:
      LOG2("THROW to " << to << " - not implemented yet");
      break;
    case eActAggress:
      LOG2("AGGRESS to " << to << " - not implemented yet");
      break;
    default:
      LOG2("not implemented yet...");
      break;
    }
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
      circle_.show();
      game_.getPanel().displayPlayerInfo(p_->getTeamId(), p_->getId());
    }

  // Mouse moves out of player.
  if (has_focus_ && !now_focus)
    {
      circle_.hide();
    }

  // Click on player. Select him.
  if (now_focus && !is_selected_ && inp.button_pressed_[1])
    {
      game_.unselectAllPlayer();
      game_.selectPlayer(this);
      circle_selected_.show();
      is_selected_ = true;
    }

  // Debug
  if (now_focus && inp.key_pressed_[(unsigned char)'d'])
    LOG3(*p_);
  
  has_focus_ = now_focus;
  Sprite::update();
}
