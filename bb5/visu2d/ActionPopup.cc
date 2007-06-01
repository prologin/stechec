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
#include "Input.hh"
#include "Game.hh"
#include "VisuPlayer.hh"
#include "ActionPopup.hh"

BEGIN_NS(sdlvisu);

ActionPopup::ActionPopup(Game& g)
  : VirtualSurface("ActionPopup", POPUP_ITEM_WIDTH,
      POPUP_ITEM_HEIGHT * (DCLII_NB + ACTII_NB)),
    api_(g.getApi()),
    game_(g),
    vp_(NULL),
    displayed_items_nb_(0)
{
  for (int i = 0; i < DCLII_NB; i++)
    {
      dcl_sprite_[i].load("image/general/declarations_on.jpg");
      dcl_sprite_[i].splitNbFrame(1, DCLII_NB);
      dcl_sprite_[i].setFrame(i + 1);
      dcl_sprite_[i].setZ(6);
      dcl_sprite_[i].disable();
      addChild(&dcl_sprite_[i]);

      dcl_sprite_on_[i].load("image/general/declarations_up.jpg");
      dcl_sprite_on_[i].splitNbFrame(1, DCLII_NB);
      dcl_sprite_on_[i].setFrame(i + 1);
      dcl_sprite_on_[i].setZ(7);
      dcl_sprite_on_[i].disable();
      addChild(&dcl_sprite_on_[i]);
    }
  for (int i = 0; i < ACTII_NB; i++)
    {
      act_sprite_[i].load("image/general/actions_on.jpg");
      act_sprite_[i].splitNbFrame(1, ACTII_NB);
      act_sprite_[i].setFrame(i + 1);
      act_sprite_[i].setZ(6);
      act_sprite_[i].disable();
      addChild(&act_sprite_[i]);

      act_sprite_on_[i].load("image/general/actions_up.jpg");
      act_sprite_on_[i].splitNbFrame(1, ACTII_NB);
      act_sprite_on_[i].setFrame(i + 1);
      act_sprite_on_[i].setZ(7);
      act_sprite_on_[i].disable();
      addChild(&act_sprite_on_[i]);
    }
  hide();
}

ActionPopup::~ActionPopup()
{
}

void ActionPopup::show()
{
  game_.setState(stPopupShow);
  VirtualSurface::show();
}

void ActionPopup::hide()
{
  game_.unsetState(stPopupShow);
  VirtualSurface::hide();
}

// Called when a player is selected, ready to choose its action declaration.
void ActionPopup::prepareDeclareMenu(VisuPlayer* vp)
{
  enum eDeclaredAction declaration;

  for (int i = 0; i < ACTII_NB; i++)
    {
      act_sprite_[i].disable();
      act_sprite_on_[i].disable();
    }
  for (int i = 0; i < DCLII_NB; i++)
    {
      dcl_sprite_[i].disable();
      dcl_sprite_on_[i].disable();
    }

  displayed_items_nb_ = api_->declarationPossibleNumber();
  if (displayed_items_nb_ <= 0) return;

  vp_ = vp;
  isDeclarationsMenu = true;

  for (int i = 0; i < displayed_items_nb_; i ++)
    {
      declaration = (enum eDeclaredAction) api_->declarationPossible(i);
      if (declaration == DCL_MOVE)        displayed_items_[i] = DCLII_MOVE;
      else if (declaration == DCL_BLITZ)  displayed_items_[i] = DCLII_BLITZ;
      else if (declaration == DCL_BLOCK)  displayed_items_[i] = DCLII_BLOCK;
      else                                displayed_items_[i] = DCLII_PASS;
      LOG6("Prepare declarations menu: Item #%1: %2.", i, displayed_items_[i]);
      dcl_sprite_[displayed_items_[i]].setPos(0, i * POPUP_ITEM_HEIGHT);
      dcl_sprite_[displayed_items_[i]].enable();
      dcl_sprite_on_[displayed_items_[i]].setPos(0, i * POPUP_ITEM_HEIGHT);
    }
  setSize(Point(POPUP_ITEM_WIDTH, POPUP_ITEM_HEIGHT * displayed_items_nb_));
}

// Called after a declaration, show the revelant menu for all possible actions.
void ActionPopup::prepareActionMenu(enum eDeclaredAction dcl)
{
  for (int i = 0; i < ACTII_NB; i++)
    {
      act_sprite_[i].disable();
      act_sprite_on_[i].disable();
    }
  for (int i = 0; i < DCLII_NB; i++)
    {
      dcl_sprite_[i].disable();
      dcl_sprite_on_[i].disable();
    }

  displayed_items_nb_ = 0;
  isDeclarationsMenu = false;

  if (dcl == DCL_PASS)
    {
      displayed_items_[0] = ACTII_MOVE;
      displayed_items_[1] = ACTII_THROW;
      displayed_items_nb_ = 2;
    }
  else if (dcl == DCL_BLITZ)
    {
      displayed_items_[0] = ACTII_MOVE;
      displayed_items_[1] = ACTII_BLOCK;
      displayed_items_nb_ = 2;
    }
  else if (dcl == DCL_BLOCK) // Just a reminder.
    {
      displayed_items_[0] = ACTII_BLOCK;
      displayed_items_nb_ = 1;
    }
  else if (dcl == DCL_MOVE) // Just a reminder.
    {
      displayed_items_[0] = ACTII_MOVE;
      displayed_items_nb_ = 1;
    }

  for (int i = 0; i < displayed_items_nb_; i++)
    {
      LOG6("Prepare actions menu: Item #%1: %2.", i, displayed_items_[i]);
      act_sprite_[displayed_items_[i]].setPos(0, i * POPUP_ITEM_HEIGHT);
      act_sprite_[displayed_items_[i]].enable();
      act_sprite_on_[displayed_items_[i]].setPos(0, i * POPUP_ITEM_HEIGHT);
    }
  setSize(Point(POPUP_ITEM_WIDTH, POPUP_ITEM_HEIGHT * displayed_items_nb_));
}

void ActionPopup::dissociateFromPlayer()
{
  vp_ = NULL;
}

void ActionPopup::update()
{
  Input& input(game_.getInput());
  bool have_focus = getScreenRect().inside(input.mouse_);
  int index = (input.mouse_.y - getRect().y) / POPUP_ITEM_HEIGHT;
  Point max_pos = game_.getField().getRect().getPos()
                + game_.getField().getScreenRect().getSize()
                - getSize();

  // If there isn't any player associated.
  if (vp_ == NULL)
    {
      VirtualSurface::update();
      return;
    }
  
  // If right-click anywhere.
  if ((game_.isStateSet(stWaitPlay) || game_.isStateSet(stPopupShow))
      && input.button_pressed_[3])
    {
      setPos(std::min(input.mouse_.x, max_pos.x), std::min(input.mouse_.y, max_pos.y));
      show();
    }

  // If left-click outside of popup menu.
  if (!have_focus && game_.isStateSet(stPopupShow) && input.button_pressed_[1])
    hide();
  
  // If left-click on popup menu.
  if (have_focus && game_.isStateSet(stPopupShow) && input.button_pressed_[1])
    {
      // Sanity check. Must not happen.
      if (index < 0 || index >= displayed_items_nb_)
        {
          WARN("Selected item index (%1) is out of range (%2)!", index, displayed_items_nb_);
          hide();
          VirtualSurface::update();
          return;
        }
      // VisuPlayer will declare/do the action.
      if (isDeclarationsMenu)
        {
          enum eDeclaredAction declaration;
          if (displayed_items_[index] == DCLII_MOVE)        declaration = DCL_MOVE;
          else if (displayed_items_[index] == DCLII_BLOCK)  declaration = DCL_BLOCK;
          else if (displayed_items_[index] == DCLII_BLITZ)  declaration = DCL_BLITZ;
          else                                              declaration = DCL_PASS;
          vp_->declareAction(declaration);
        }
      else
        {
          enum eRealAction action;
          if (displayed_items_[index] == ACTII_MOVE)        action = ACT_MOVE;
          else if (displayed_items_[index] == ACTII_BLOCK)  action = ACT_BLOCK;
          else                                              action = ACT_THROW;
          vp_->selectAction(action);
        }
      hide();
    }

  // Highlight of menu.
  if (have_focus)
    {
      for (int i = 0; i < displayed_items_nb_; i++)
        if (i == index)
          if (isDeclarationsMenu)
            dcl_sprite_on_[displayed_items_[i]].enable();
          else
            act_sprite_on_[displayed_items_[i]].enable();
        else
          if (isDeclarationsMenu)
            dcl_sprite_on_[displayed_items_[i]].disable();
          else
            act_sprite_on_[displayed_items_[i]].disable();
    }
  else
    for (int i = 0; i < displayed_items_nb_; i++)
      if (isDeclarationsMenu)
        dcl_sprite_on_[displayed_items_[i]].disable();
      else
        act_sprite_on_[displayed_items_[i]].disable();

  VirtualSurface::update();
}

END_NS(sdlvisu);
