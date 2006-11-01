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
    vp_(NULL)
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

void ActionPopup::setVisuPlayer(VisuPlayer* vp)
{
  vp_ = vp;

  // FIXME: get player capabilities, to display revelant menu
  static int disp_act[] = {0, 2, 4, 6, -1};

  for (int i = 0; i < 12; i++)
    sprite_[i].disable();

  for (int i = 0; disp_act[i] != -1; i++)
    {
      sprite_[disp_act[i]].setPos(0, i * 40);
      sprite_[disp_act[i]].enable();
      sprite_on_[disp_act[i]].setPos(0, i * 40);
    }
  
  setSize(Point(120, 40 * 4));
}

VisuPlayer* ActionPopup::getVisuPlayer() const
{
  return vp_;
}

void ActionPopup::update()
{
  Input& input(g_.getInput());
  bool have_focus = getScreenRect().inside(input.mouse_);
  int item = (input.mouse_.y - getRect().y) / 40;

  if (vp_ == NULL)
    {
      VirtualSurface::update();
      return;
    }
  
  if ((g_.isStateSet(stWaitPlay) || g_.isStateSet(stPopupShow))
      && input.button_pressed_[3])
    {
      // Show popup menu.
      setPos(input.mouse_);
      show();
    }

  if (!have_focus && g_.isStateSet(stPopupShow)
      && input.button_pressed_[1])
    hide();
  
  if (have_focus && g_.isStateSet(stPopupShow) &&
	   input.button_pressed_[1])
    {
      // Click on popup menu.
      LOG2("Do an action! -> %1", item);

      // Send action order on VisuPlayer. It'll know what to do.
      switch (item)
	{
	case 0:
	  vp_->prepareAction(eActMove);
	  break;

	case 1:
	  vp_->prepareAction(eActBlock);
	  break;

	case 2:
	  vp_->prepareAction(eActThrow);
	  break;

	case 3:
	  vp_->prepareAction(eActBlitz);
	  break;

	default:
	  LOG2("action not handled.");
	  break;
	}
      hide();
    }

  // Highligh of menu
  static int disp_act[] = {0, 2, 4, 6, -1};
  if (have_focus)
    {
      for (int i = 0; disp_act[i] != -1; i++)
	if (i == item)
	  sprite_on_[disp_act[i]].enable();
	else
	  sprite_on_[disp_act[i]].disable();
    }
  else
    for (int i = 0; disp_act[i] != -1; i++)
      sprite_on_[disp_act[i]].disable();

  VirtualSurface::update();
}

END_NS(sdlvisu);
