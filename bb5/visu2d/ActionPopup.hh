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

class Api;

BEGIN_NS(sdlvisu);

#define POPUP_ITEM_HEIGHT 40
#define POPUP_ITEM_WIDTH 120

//! @see data/general/declarations_on.jpg
enum eDeclarationsImageIndex {
  DCLII_MOVE = 0,
  DCLII_BLITZ,
  DCLII_BLOCK,
  DCLII_PASS,

  DCLII_NB
};

//! @see data/general/actions_on.jpg
enum eActionsImageIndex {
  ACTII_MOVE = 0,
  ACTII_GOFORIT,
  ACTII_BLOCK,
  ACTII_GATHER,
  ACTII_THROW,
  ACTII_AGGRESS,
  ACTII_BLITZ,
  ACTII_ROLLOVER,
  ACTII_STANDUP,
  ACTII_TRANSMIT,
  ACTII_THROWTM,
  ACTII_LEAP,

  ACTII_NB
};

class VisuPlayer;
class Game;

/*!
** @brief Action Popup, used for both declaration and action.
*/

class ActionPopup : public VirtualSurface
{
public:
  ActionPopup(Game& g);
  virtual ~ActionPopup();

  void prepareDeclareMenu(VisuPlayer* vp);
  void prepareActionMenu(enum eDeclaredAction dcl);
  void dissociateFromPlayer();

  virtual void show();
  virtual void hide();
  
  virtual void update();

private:
  Api*      api_;
  Game&     game_;

  VisuPlayer*   vp_;
  Sprite    act_sprite_[ACTII_NB];    ///< Action normal sprites.
  Sprite    act_sprite_on_[ACTII_NB]; ///< Action sprites under mouse pointer.
  Sprite    dcl_sprite_[DCLII_NB];    ///< Declaration normal sprites.
  Sprite    dcl_sprite_on_[DCLII_NB]; ///< Declaration sprites under mouse pointer.
  int     displayed_items_[DCLII_NB + ACTII_NB]; ///< Displayed menu items.
  int     displayed_items_nb_;  ///< Menu length.
  bool    isDeclarationsMenu;   ///< Whether the menu displays declarations or actions.
};

END_NS(sdlvisu);

#endif /* !ACTIONPOPUP_HH_ */
