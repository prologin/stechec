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

#include "Input.hh"
#include "Game.hh"
#include "VisuPlayer.hh"
#include "ActionPopup.hh"

BEGIN_NS(sdlvisu);

ActionPopup::ActionPopup(Game& g)
  : VirtualSurface("ActionPopup", 120, 40 * 12),
    g_(g),
    vp_(NULL),
    display_act_nb_(0)
{
  for (int i = 0; i < 12; i++)
    {
      sprite_[i].load("image/general/actions_on.jpg");
      sprite_[i].splitNbFrame(1, 12);
      sprite_[i].setFrame(i + 1);
      sprite_[i].setZ(6);
      sprite_[i].disable();
      addChild(&sprite_[i]);

      sprite_on_[i].load("image/general/actions_up.jpg");
      sprite_on_[i].splitNbFrame(1, 12);
      sprite_on_[i].setFrame(i + 1);
      sprite_on_[i].setZ(7);
      sprite_on_[i].disable();
      addChild(&sprite_on_[i]);
    }
  hide();
}

ActionPopup::~ActionPopup()
{
}

void ActionPopup::show()
{
  g_.setState(stPopupShow);
  VirtualSurface::show();
}

void ActionPopup::hide()
{
  g_.unsetState(stPopupShow);
  VirtualSurface::hide();
}

// Called when a player is selected, ready to choose its action declaration.
void ActionPopup::prepareDeclareMenu(VisuPlayer* vp, enum eStatus player_status)
{
  static enum eAction std_act[] = {eActMove, eActBlock, eActPass, eActBlitz};
  static enum eAction get_up_act[] = {eActGetUp};
  int i;

  for (i = 0; i < 12; i++)
    sprite_[i].disable();

  display_act_nb_ = 0;
  if (player_status == STA_PRONE)
    {
      display_act_nb_ = sizeof (get_up_act) / sizeof (enum eAction);
      for (int i = 0; i < display_act_nb_; i++)
	display_act_[i] = get_up_act[i];
    }
  else if (player_status == STA_STUNNED)
	  ;
  else if (player_status == STA_STANDING)
    {
      display_act_nb_ = sizeof (std_act) / sizeof (enum eAction);
      for (int i = 0; i < display_act_nb_; i++)
	display_act_[i] = std_act[i];
    }
  else
    return;

  vp_ = vp;
  for (i = 0; i < display_act_nb_; i++)
    {
      LOG6("prepare decl menu: %1 - %2", display_act_[i], i);
      sprite_[display_act_[i]].setPos(0, i * 40);
      sprite_[display_act_[i]].enable();
      sprite_on_[display_act_[i]].setPos(0, i * 40);
    }
  setSize(Point(120, 40 * display_act_nb_));
}

// Called after a declaration, show the revelant menu for all possible actions.
// Can only be called after:
//  - eActPass (move or pass)
//  - eActBlitz (move or block)
void ActionPopup::prepareActionMenu(enum eAction decl_act)
{
  for (int i = 0; i < 12; i++)
    sprite_[i].disable();

  display_act_nb_ = 0;
  if (decl_act == eActPass)
    {
      display_act_[0] = eActMove;
      display_act_[1] = eActPass;
      display_act_nb_ = 2;
    }
  else if (decl_act == eActBlitz)
    {
      display_act_[0] = eActMove;
      display_act_[1] = eActBlock;
      display_act_nb_ = 2;
    }

  for (int i = 0; i < display_act_nb_; i++)
    {
      LOG6("prepare action menu: %1 - %2", display_act_[i], i);
      sprite_[display_act_[i]].setPos(0, i * 40);
      sprite_[display_act_[i]].enable();
      sprite_on_[display_act_[i]].setPos(0, i * 40);
    }
  setSize(Point(120, 40 * display_act_nb_));
}

void ActionPopup::update()
{
  Input& input(g_.getInput());
  bool have_focus = getScreenRect().inside(input.mouse_);
  int item = (input.mouse_.y - getRect().y) / 40;

  // Really, nothing to do.
  if (vp_ == NULL)
    {
      VirtualSurface::update();
      return;
    }
  
  // Show popup menu.
  if ((g_.isStateSet(stWaitPlay) || g_.isStateSet(stPopupShow))
      && input.button_pressed_[3])
    {
      setPos(input.mouse_);
      show();
    }

  // Hide popup menu.
  if (!have_focus && g_.isStateSet(stPopupShow) && input.button_pressed_[1])
    hide();
  
  if (have_focus && g_.isStateSet(stPopupShow) &&
	   input.button_pressed_[1])
    {
      // Sanity check. Must not happen.
      if (item < 0 || item >= display_act_nb_)
	{
	  WARN("Selected item out of range (%1 - %2)!", item, display_act_nb_);
	  hide();
	  VirtualSurface::update();
	  return;
	}

      // Click on popup menu. VisuPlayer will declare the action.
      LOG2("Do an action! -> %1", item);
      enum eAction act = display_act_[item];
      vp_->selectAction(act);
       
      hide();
    }

  // Highligh of menu
  if (have_focus)
    {
      for (int i = 0; i < display_act_nb_; i++)
	if (i == item)
	  sprite_on_[display_act_[i]].enable();
	else
	  sprite_on_[display_act_[i]].disable();
    }
  else
    for (int i = 0; i < display_act_nb_; i++)
      sprite_on_[display_act_[i]].disable();

  VirtualSurface::update();
}

END_NS(sdlvisu);
