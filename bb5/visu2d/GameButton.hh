/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
**
** Copyright (C) 2007 The TBT Team.
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

#ifndef GAMEBUTTON_HH_
# define GAMEBUTTON_HH_

# include "Sprite.hh"

BEGIN_NS(sdlvisu);

class Game;

/*!
** @brief Base class for GameButton handler.
** @ingroup tbt_2d_gui
*/
class GameButtonCallback
{
public:
  GameButtonCallback() {}
  virtual ~GameButtonCallback() {}

  virtual void clicked() = 0;
};

/*!
** @brief Simple GameButton class for visu.
** @ingroup tbt_2d_gui
*/
class GameButton : public Sprite
{
public:
  GameButton(Game& game, const std::string filename, double zoom = 1., double angle = 0.);
  virtual ~GameButton();

  void setActionHandler(GameButtonCallback* handler);
  GameButtonCallback* getActionHandler() const;

  virtual void update();

private:
  Game& game_;
  GameButtonCallback* handler_;
  bool is_being_pressed_;
};

END_NS(sdlvisu);

#endif /* !GAMEBUTTON_HH_ */
