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

BEGIN_NS(sdlvisu);

enum eAction {
  eActNone = -1,
  eActMove,
  eActMovePlus,
  eActBlock,
  eActGather,
  eActPass,
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

  void prepareDeclareMenu(VisuPlayer* vp, enum eStatus player_status);
  void prepareActionMenu(eAction decl_act);

  virtual void show();
  virtual void hide();
  
  virtual void update();

private:
  Game&                 g_;

  VisuPlayer*           vp_;
  Sprite                sprite_[12];
  Sprite                sprite_on_[12];
  enum eAction		display_act_[16];
  int			display_act_nb_;
};

END_NS(sdlvisu);

#endif /* !ACTIONPOPUP_HH_ */
