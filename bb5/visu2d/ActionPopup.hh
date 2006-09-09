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

#ifndef ACTIONPOPUP_HH_
# define ACTIONPOPUP_HH_

# include "VirtualSurface.hh"
# include "Sprite.hh"

enum eVisuAction {
  eActNone,
  eActMove,
  eActBlock,
  eActGather,
  eActThrow,
  eActAggress,
  eActBlitz,
  eActRollOver,
  eActGetUp,
  eActTransmit,
  eActThrowTm,
  eActLeap
};

class VisuPlayer;
class Game;

class ActionPopup : public VirtualSurface
{
public:
  ActionPopup(Game& g);
  virtual ~ActionPopup();

  void setVisuPlayer(VisuPlayer* vp);
  VisuPlayer* getVisuPlayer() const;

  void show(const Point& pos);
  void hide();
  bool isVisible() const;

  virtual void update();

private:
  Game&                 g_;

  VisuPlayer*           vp_;
  Sprite                sprite_[12];
  bool                  show_;
};

#endif /* !ACTIONPOPUP_HH_ */
