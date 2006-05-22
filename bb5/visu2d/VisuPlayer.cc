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
    
  if (parent_ != NULL)
    {
      setObjectRect(Rect(parent_->getAbsolutePos() + pos, getSize()));
      setObjectContainer(parent_->getRealRect());
    }
}


void VisuPlayer::update()
{
  Input& inp = game_.getInput();
  if (inp.button_pressed_[1] && has_focus_ && !is_selected_)
    {
      field_.addChild(&circle_selected_);
      is_selected_ = true;
    }
  
  Sprite::update();
}

void VisuPlayer::mouseMove(const Point& pos)
{
  if (!has_focus_)
    {
      parent_->addChild(&circle_);
      panel_.displayPlayerInfo(p_->getTeamId(), p_->getId());
    }
}

void VisuPlayer::mouseOut()
{
  if (has_focus_)
    {
      parent_->removeChild(&circle_);
    }
}
