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

/*
** Player class for visu.
**
** Player caracteristics are not stored in this class, they are
** fetched from Api.
*/
class VisuPlayer : public Sprite
{
public:
  VisuPlayer(Game& game, ActionPopup* act_popup, const CPlayer* p);
  virtual ~VisuPlayer();

  void unselect();

  //! @brief Called at each new turn. Reset some variables.
  void newTurn();

  //! @brief Called at each end turn. Reset some variables.
  void finishedTurn();
  
  //! @brief An action was selected by the popup menu or an other source.
  //! @param item Action selected.
  void selectAction(enum eAction item);

  virtual void setPos(const Point& pos);
  virtual void update();

private:

  //! @brief Really do an action, after coach has chosen his target.
  void targetAction(enum eAction item);

  //! @brief To call when this player has finished its action.
  void actionFinished();
    
  void drawPath();
  
  Api*          api_;
  Game&         game_;
  ActionPopup*  act_popup_;
  const CPlayer* p_;

  bool          has_focus_;
  bool          is_selected_;
  bool		is_second_action_;
  bool		has_played_;
  enum eStatus	last_player_status_;
  
  Sprite        circle_;
  Sprite        circle_selected_;
  Sprite        player_num_;
  Sprite	status_;

  eAction	target_action_;

  // for pathway.
  Sprite	move_sprite_;
  std::vector<Sprite> path_;
  Point		prev_dst_;
};

END_NS(sdlvisu);

#endif /* !VISUPLAYER_HH_ */
