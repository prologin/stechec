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
#include "ActionPopup.hh"
#include "VisuPlayer.hh"

BEGIN_NS(sdlvisu);

VisuPlayer::VisuPlayer(Game& game, ActionPopup* act_popup, const CPlayer* p)
  : api_(game.getApi()),
    game_(game),
    act_popup_(act_popup),
    p_(p),
    has_focus_(false),
    is_selected_(false),
    has_played_(true),
    last_player_status_(STA_UNASSIGNED),
    circle_("image/general/circle.png"),
    circle_selected_("image/general/circle_select.png"),
    player_num_("image/general/player_num.png"),
    status_("image/general/status.png"),
    target_action_(eActNone)
{
  VisuField& field = game_.getField();

  circle_.setZ(5);
  circle_.splitNbFrame(3, 1);
  circle_.hide();
  field.addChild(&circle_);

  circle_selected_.setZ(6);
  circle_selected_.splitNbFrame(2, 1);
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

void VisuPlayer::newTurn()
{
  is_second_action_ = false;
  circle_selected_.setFrame(1);
  has_played_ = false;
  if (p_->getStatus() == STA_STANDING
      || p_->getStatus() == STA_PRONE
      || p_->getStatus() == STA_STUNNED)
    circle_.show();
  else
    circle_.hide();
}

void VisuPlayer::finishedTurn()
{
  has_played_ = true;
  circle_.hide();
}

void VisuPlayer::actionFinished()
{
  circle_.hide();
  circle_selected_.hide();
  has_played_ = true;
}

void VisuPlayer::drawPath()
{
  VisuField& field = game_.getField();
  Point to(field.mouseToSquare());
  
  if (prev_dst_ == to)
    return;

  path_.clear();
  int len = api_->moveLength(to);

  // Move is not possible.
  if (len == 0)
    field.setMarker(field.mouseToSquare(), 1);
  else
    field.removeMarker();

  for (int i = 0; i < len; i++)
    {
      Point pt(api_->movePath(i));
      path_.push_back(move_sprite_);
      Sprite& sp = path_.back();
      sp.setPos(field.squareToField(pt, Point(5, 5)));
      game_.getField().addChild(&sp);
    }
  prev_dst_ = to;
}

// Called when the coach click on the popup menu
void VisuPlayer::selectAction(enum eAction item)
{
  api_->selectTeam(p_->getTeamId());
  api_->selectPlayer(p_->getId());

  LOG2("Select action for action %1", item);
  game_.setState(stDoAction);
  
  // Depending on action:
  //  - require another popup menu to display
  //  - declare the action to the api and waits that coach select target
  //  - do this action immediately (try to stand up, in case we were prone)
  switch (item)
    {
    case eActBlitz:
      api_->doDeclare(BLITZ);
      api_->doStandUpPlayer();
      act_popup_->prepareActionMenu(eActBlitz);
      circle_selected_.setFrame(2);
      game_.unsetState(stDoAction);
      is_second_action_ = true;
      break;

    case eActPass:
      if (!is_second_action_)
	{
	  api_->doDeclare(PASS);
	  api_->doStandUpPlayer();
	  act_popup_->prepareActionMenu(eActPass);
	  circle_selected_.setFrame(2);
	  game_.unsetState(stDoAction);
	  is_second_action_ = true;
	}
      else
	{
	  target_action_ = eActPass;
	}
      break;

    case eActMove:
      if (!is_second_action_)
	{
	  api_->doDeclare(MOVE);
	  api_->doStandUpPlayer();
	}
      target_action_ = eActMove;
      break;

    case eActBlock:
      api_->doDeclare(BLOCK);
      api_->doStandUpPlayer();
      target_action_ = eActBlock;
      break;

    case eActAggress:
      target_action_ = eActAggress;
      break;

    default:
      LOG2("Invalid action selection: %1", item);
      game_.unsetState(stDoAction);
      break;
    }
}

void VisuPlayer::targetAction(enum eAction item)
{
  VisuField& field = game_.getField();
  Point to(field.mouseToSquare());

  switch (item)
    {
    case eActMove:
      LOG2("MOVE to %1 - send to api.", to);
      api_->doMovePlayer(to);
      break;
    case eActBlock:
      LOG2("BLOCK to %1 - send to api", to);
      api_->doBlockPlayer(api_->playerId(to));
      break;
    case eActPass:
      LOG2("THROW to %1 - send to api", to);
      api_->doPassPlayer(to);
      break;
    case eActAggress:
      LOG2("AGGRESS to %1 - not implemented yet", to);
      actionFinished();
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
  VisuField& field = game_.getField();

  api_->selectTeam(p_->getTeamId());
  api_->selectPlayer(p_->getId());

  // Update player status sprite
  if (p_->getStatus() != last_player_status_)
    {
      last_player_status_ = p_->getStatus();
      LOG4("Switch status for player %1 to %2.", p_->getId(), last_player_status_);
      switch (last_player_status_)
	{
	case STA_RESERVE:
	  status_.setFrame(5);
	  status_.show();
	  break;
	case STA_PRONE:
	  status_.setFrame(1);
	  status_.show();
	  break;
	case STA_STUNNED:
	  status_.setFrame(2);
	  status_.show();
	  break;
	case STA_KO:
	  status_.setFrame(3);
	  status_.show();
	  break;
	case STA_STANDING:
	default:
	  status_.hide();
	  break;
	}
    }

  // Some action to do ?
  if (target_action_ != eActNone && game_.isStateSet(stDoAction))
    {
      if (field.mouseInsideField())
	{
	  if (target_action_ != eActMove)
	    field.setMarker(field.mouseToSquare(), 0);
	  if (inp.button_pressed_[1])
	    {
	      targetAction(target_action_);
	      target_action_ = eActNone;
	      game_.unsetState(stDoAction);
	      field.removeMarker();
	    }
	}
      else
	field.removeMarker();
    }

  // Update focus.
  bool now_focus = getScreenRect().inside(inp.mouse_);

  // Mouse moves on player.
  if (!has_focus_ && now_focus)
    {
      if (p_->getTeamId() == api_->myTeamId())
	circle_.anim(200);
      game_.getPanel().displayPlayerInfo(p_->getTeamId(), p_->getId());
    }

  // Mouse moves out of player.
  if (has_focus_ && !now_focus)
    {
      if (p_->getTeamId() == api_->myTeamId())
	circle_.stopAnim();
    }

  // Click on player (of _my_ team, on my turn). Select him.
  if (now_focus && !is_selected_ && !has_played_
      && !game_.isStateSet(stPopupShow) && !game_.isStateSet(stDoAction)
      && api_->myTeamId() == p_->getTeamId()
      && (inp.button_pressed_[1] || inp.button_pressed_[3]))
    {
      game_.unselectAllPlayer();
      act_popup_->prepareDeclareMenu(this, p_->getStatus());
      circle_selected_.show();
      is_selected_ = true;
      // Left button will show it now
      if (inp.button_pressed_[3])
	{
	  act_popup_->setPos(inp.mouse_);
	  act_popup_->show();
	}
    }

  // Player has finished its action
  if (circle_.isShown() && p_->hasPlayed())
    actionFinished();
  
  // Draw path
  if (game_.isStateSet(stDoAction) && target_action_ == eActMove)
    drawPath();
  else
    path_.clear();
  
  // Debug
  if (now_focus && inp.key_pressed_[SDLK_d])
    LOG3("%1", *p_);

  has_focus_ = now_focus;
  Sprite::update();
}

END_NS(sdlvisu);
