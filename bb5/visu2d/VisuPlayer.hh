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
class CPlayer;

BEGIN_NS(sdlvisu);

class Game;
class ActionPopup;

/*!
** @brief Player class for visu.
** @ingroup tbt_2d_gui
**
** Player caracteristics are not stored in this class, they are
** fetched from Api.
*/
class VisuPlayer : public Sprite
{
public:
  VisuPlayer(Game& game, ActionPopup* act_popup, const CPlayer* p);
  virtual ~VisuPlayer();

  //! @brief Get CPlayer associated with.
  const CPlayer* getPlayer() const;

  void unselect();

  //! @brief Called at each new turn. Reset some variables.
  void beginTurn();

  //! @brief Called at each end turn. Reset some variables.
  void finishTurn();
  
  //! @brief Declare an action.
  //! @note Called on coach decision, via e.g. popup menu.
  //! @param declaration Declaration selected.
  void declareAction(enum eDeclaredAction declaration);

  //! @brief Handles action declaration event.
  //! @note Called on API event.
  //! @param declaration Action declared.
  void onEventDeclare(enum eDeclaredAction declaration);

  //! @brief Select a real action.
  //! @brief Called on coach decision, via e.g. popup menu.
  //! @param action Real action selected.
  void selectAction(enum eRealAction action);

  //! @brief Update status/ball mini-icon.
  void updateStatus();

  //! @brief Enable player number, status, etc. at the same time.
  virtual void enable();
  //! @brief Disable player number, status, etc. at the same time.
  virtual void disable();
  //! @brief Set position of player number, status, etc. at the same time.
  virtual void setPos(const Point& pos);

  virtual void update();

private:

  //! @brief Really do an action, after coach has chosen his target.
  void targetAction(enum eRealAction act);

  //! @brief To call when this player has finished its action.
  void finishAction();

  void drawPath();
  
  Api*            api_;
  Game&           game_;
  ActionPopup*    act_popup_;
  const CPlayer*  p_;

  bool          has_focus_;
  bool          is_selected_;
  bool          has_played_;
  
  Sprite    circle_;
  Sprite    circle_selected_;
  Sprite    player_num_;
  Sprite    status_;

  enum eRealAction      target_action_;
  enum eStatus          old_status_; ///< Kludge spotted, better to have consistent events.

  // for pathway.
  Sprite                move_sprite_;
  std::vector<Sprite>   path_;
  Point                 prev_dst_;
};

END_NS(sdlvisu);

#endif /* !VISUPLAYER_HH_ */
