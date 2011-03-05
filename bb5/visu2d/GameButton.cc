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

#include "GameButton.hh"

#include "Game.hh"
#include "Input.hh"

BEGIN_NS(sdlvisu);

GameButton::GameButton(Game& game, const std::string filename, double zoom, double angle)
  : Sprite(filename, zoom, angle),
    game_(game),
    is_being_pressed_(false)
{
  disable();
}

GameButton::~GameButton()
{
}

void GameButton::setActionHandler(GameButtonCallback* handler)
{
  handler_ = handler;
}

GameButtonCallback* GameButton::getActionHandler() const
{
  return handler_;
}

void GameButton::update()
{
  Input& inp = game_.getInput();

  // FIXME: It doesn't take count of other surfaces, which may be overlaying the button.
  if (getScreenRect().inside(inp.mouse_))
    {
      if (inp.button_pressed_[1])
        is_being_pressed_ = true;
      else if (is_being_pressed_)
        {
          if (handler_ != NULL)
            handler_->clicked();
          is_being_pressed_ = false;
        }
    }
  else
    if (is_being_pressed_ && !inp.button_pressed_[1])
      is_being_pressed_ = false;

  Sprite::update();
}

END_NS(sdlvisu);
