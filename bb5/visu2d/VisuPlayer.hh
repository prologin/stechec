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

#ifndef VISUPLAYER_HH_
# define VISUPLAYER_HH_

# include "ActionPopup.hh"
# include "Sprite.hh"

class Api;
class Game;
class CPlayer;

/*
** Player class for visu.
**
** Player caracteristics are not stored in this class, they are
** fetched from Api.
*/
class VisuPlayer : public Sprite
{
public:
  VisuPlayer(Api* api, Game& game, const CPlayer* p);
  virtual ~VisuPlayer();

  void unselect();

  //! @brief Do an action.
  void action(eAction item);

  virtual void setPos(const Point& pos);
  virtual void update();

private:

  Api*          api_;
  Game&         game_;
  const CPlayer* p_;

  bool          has_focus_;
  bool          is_selected_;
  Sprite        circle_;
  Sprite        circle_selected_;
  Sprite        player_num_;
  Sprite        action_popup_;
};

#endif /* !VISUPLAYER_HH_ */
