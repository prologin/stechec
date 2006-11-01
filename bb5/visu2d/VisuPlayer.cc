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

BEGIN_NS(sdlvisu);

VisuPlayer::VisuPlayer(Api* api, Game& game, const CPlayer* p)
  : api_(api),
    game_(game),
    p_(p),
    has_focus_(false),
    is_selected_(false),
    circle_("image/general/circle.png"),
    circle_selected_("image/general/circle_select.png"),
    player_num_("image/general/player_num.png"),
    status_("image/general/status.png"),
    next_action_(eActNone)
{
  VisuField& field = game_.getField();

  circle_.setZ(5);
  circle_.splitNbFrame(3, 1);
  circle_.anim(200);
  circle_.hide();
  field.addChild(&circle_);

  circle_selected_.setZ(6);
  circle_selected_.hide();
  field.addChild(&circle_selected_);

  player_num_.setZ(4);
  player_num_.splitNbFrame(16, 1);
  player_num_.setFrame(p->getId() + 1);
  field.addChild(&player_num_);

  status_.setZ(4);
  status_.splitNbFrame(6, 1);
  status_.hide();
  field.addChild(&status_);

  move_sprite_.load("image/general/map-move.png");
  move_sprite_.splitNbFrame(3, 2);
  move_sprite_.setFrame(3);
  move_sprite_.setZ(3);

  // Set its property
  load("image/figs/amazon.png"); // FIXME: team picture.
  splitSizeFrame(40, 40);
  setZ(3);
  setFrame(p->getPlayerPosition() * 2 + p->getTeamId() + 1);
  last_player_status_ = api_->playerStatus();
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

void VisuPlayer::drawPath()
{
  Point to((game_.getInput().mouse_ - Point(game_.getField().getScreenRect())) / 40);
  if (prev_dst_ == to)
    return;

  path_.clear();
  int len = api_->moveLength(to.x, to.y);
  for (int i = 0; i < len; i++)
    {
      Point pt(api_->movePathX(i), api_->movePathY(i));
      path_.push_back(move_sprite_);
      Sprite& sp = path_.back();
      sp.setPos(pt * 40);
      game_.getField().addChild(&sp);
    }
  prev_dst_ = to;
}

void VisuPlayer::prepareAction(enum eAction item)
{
  int player_id = p_->getId();

  game_.setState(stDoAction);
  switch (item)
    {
    case eActMove:
      api_->doDeclareMove(player_id);
      next_action_ = item;
      break;

    case eActBlock:
      api_->doDeclareBlock(player_id);
      next_action_ = item;
      break;

    case eActThrow:
    case eActAggress:

    default:
      LOG2("not implemented yet...");
      game_.unsetState(stDoAction);
      break;
    }
}

void VisuPlayer::action(enum eAction item)
{
  Point to((game_.getInput().mouse_ - Point(game_.getField().getScreenRect())) / 40);
  int player_id = p_->getId();
  
  switch (item)
    {
    case eActMove:
      LOG2("MOVE to %1 - send to api.", to);
      api_->doMovePlayer(player_id, to);
      break;
    case eActBlock:
      LOG2("BLOCK to %1 - send to api", to);
      api_->doBlockPlayer(player_id, api_->playerId(to.x, to.y));
      break;
    case eActThrow:
      LOG2("THROW to %1 - not implemented yet", to);
      break;
    case eActAggress:
      LOG2("AGGRESS to %1 - not implemented yet", to);
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
  status_.setPos(pos + Point(20, 18));
}


void VisuPlayer::update()
{
  Input& inp = game_.getInput();

  api_->selectTeam(p_->getTeamId());
  api_->selectPlayer(p_->getId());

  // Update player status sprite
  if (api_->playerStatus() != last_player_status_)
    {
      last_player_status_ = api_->playerStatus();
      LOG4("Switch status for player %1 to %2.", p_->getId(), last_player_status_);
      switch (last_player_status_)
	{
	case 3:
	case 4:
	  status_.setFrame(1);
	  status_.show();
	  break;
	case 5:
	  status_.setFrame(2);
	  status_.show();
	  break;
	case 6:
	  status_.setFrame(3);
	  status_.show();
	  break;
	case 7:
	  status_.setFrame(5);
	  status_.show();
	  break;
	default:
	  status_.hide();
	  break;
	}
    }
  
  // Some action to do ?
  if (next_action_ != eActNone && game_.isStateSet(stDoAction) && inp.button_pressed_[1])
    {
      action(next_action_);
      next_action_ = eActNone;
      game_.unsetState(stDoAction);
    }
  
  // Update focus.
  bool now_focus = getScreenRect().inside(inp.mouse_);

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

  // Click on player (of _my_ team). Select him.
  if (now_focus && !is_selected_ && inp.button_pressed_[1]
      && api_->myTeamId() == p_->getTeamId())
    {
      game_.unselectAllPlayer();
      game_.selectPlayer(this);
      circle_selected_.show();
      is_selected_ = true;
    }

  // Draw path
  if (game_.isStateSet(stDoAction) && next_action_ == eActMove)
    drawPath();
  else
    path_.clear();
  
  // Debug
  if (now_focus && inp.key_pressed_[(unsigned char)'d'])
    LOG3("%1", *p_);

  has_focus_ = now_focus;
  Sprite::update();
}

END_NS(sdlvisu);
