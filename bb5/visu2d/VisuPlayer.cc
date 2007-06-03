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
    circle_("image/general/circle.png"),
    circle_selected_("image/general/circle_select.png"),
    player_num_("image/general/player_num.png"),
    status_("image/general/status.png"),
    target_action_(ACT_UNASSIGNED)
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

void VisuPlayer::beginTurn()
{
  unselect();
  circle_selected_.setFrame(1);
  if (p_->getStatus() == STA_STANDING
      || p_->getStatus() == STA_PRONE)
    {
      has_played_ = false;
      circle_.show();
    }
  else
    {
      has_played_ = true;
      circle_.hide();
    }
}

void VisuPlayer::finishTurn()
{
  unselect();
  has_played_ = true;
  target_action_ = ACT_UNASSIGNED;
  circle_.hide();
}

void VisuPlayer::finishAction()
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

// Called when the coach click on the declarations popup menu
void VisuPlayer::declareAction(enum eDeclaredAction dcl)
{
  assert(DCL_UNASSIGNED < dcl && dcl <= DCL_LAST);

  api_->selectTeam(p_->getTeamId());
  api_->selectPlayer(p_->getId());

  LOG2("Select declaration: %1.", Player::stringify(dcl));
  api_->doDeclare(dcl);
}

// Called on API event.
void VisuPlayer::onEventDeclare(enum eDeclaredAction dcl)
{
  if (p_->getTeamId() == api_->myTeamId())
    {
      act_popup_->prepareActionMenu(dcl);
      circle_selected_.setFrame(2);
      circle_selected_.show();
      game_.setState(stDoAction);
      if (dcl == DCL_BLOCK)
        target_action_ = ACT_BLOCK;
      else
        target_action_ = ACT_MOVE;
    }
}

// Called when the coach click on the actions popup menu
void VisuPlayer::selectAction(enum eRealAction act)
{
  assert(ACT_UNASSIGNED < act && act <= ACT_LAST);

  api_->selectTeam(p_->getTeamId());
  api_->selectPlayer(p_->getId());

  LOG2("Select action: %1.", Player::stringify(act));
  game_.setState(stDoAction);
  
  switch (act)
  {
    case ACT_THROW:
    case ACT_MOVE:
    case ACT_BLOCK:
      target_action_ = act;
      break;
    default:
      LOG2("Invalid action selection: %1.", act);
      game_.unsetState(stDoAction);
      break;
    }
}

void VisuPlayer::targetAction(enum eRealAction act)
{
  VisuField& field = game_.getField();
  Point to(field.mouseToSquare());

  api_->selectTeam(p_->getTeamId());
  api_->selectPlayer(p_->getId());

  switch (act)
  {
    case ACT_MOVE:
      LOG2("MOVE to %1 - send to api.", to);
      api_->doMovePlayer(to);
      break;
    case ACT_BLOCK:
      LOG2("BLOCK to %1 - send to api", to);
      api_->doBlockPlayer(api_->playerId(to));
      break;
    case ACT_THROW:
      LOG2("THROW to %1 - send to api", to);
      api_->doPassPlayer(to);
      break;
    default:
      LOG2("Action (%1) not implemented yet...", act);
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

  // Some action to do ?
  if (target_action_ != ACT_UNASSIGNED && game_.isStateSet(stDoAction))
    {
      if (field.mouseInsideField())
        {
          if (target_action_ != ACT_MOVE)
            field.setMarker(field.mouseToSquare(), 0);
          if (inp.button_pressed_[1])
            {
              targetAction(target_action_);
              target_action_ = ACT_UNASSIGNED;
              game_.unsetState(stDoAction);
              field.removeMarker();
            }
        }
      else
        field.removeMarker();
    }

  // Update focus.
  // FIXME: It doesn't count action popup and dialog box, which may be overlaying the player.
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
      if (p_->getAction() == DCL_UNASSIGNED)
        act_popup_->prepareDeclareMenu(this);
      else
        act_popup_->prepareActionMenu(p_->getAction());
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
    finishAction();
  
  // Draw path
  if (game_.isStateSet(stDoAction)
      && is_selected_
      && target_action_ == ACT_MOVE)
    drawPath();
  else
    path_.clear();
  
  // Debug
  if (now_focus && inp.key_pressed_[SDLK_d])
    LOG3("%1", *p_);

  has_focus_ = now_focus;
  Sprite::update();
}

void VisuPlayer::updateStatus()
{
  api_->selectTeam(p_->getTeamId());
  api_->selectPlayer(p_->getId());
  
  LOG4("Switch status for player %1 of team %2 to %3.",
      p_->getId(), p_->getTeamId(), p_->getStatus());
  switch (p_->getStatus())
      {
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
        case STA_INJURED:
          status_.setFrame(4);
          status_.show();
          break;
        case STA_SENTOFF:
          status_.setFrame(5);
          status_.show();
          break;
        case STA_STANDING:
          if (api_->getBallOwner() == p_)
            {
              game_.getField().removeBall();
              status_.setFrame(6);
              status_.show();
            }
          else
            status_.hide();
          break;
        case STA_RESERVE:
        default:
          status_.hide();
          break;
      }
}

END_NS(sdlvisu);
