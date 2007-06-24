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

#ifndef PANEL_HH_
# define PANEL_HH_

# include "Sprite.hh"
# include "TextSurface.hh"
# include "VirtualSurface.hh"

BEGIN_NS(sdlvisu);

class Game;

/*!
** @brief Game panel (on the right).
** @ingroup tbt_2d_gui
*/
class Panel : public VirtualSurface
{
public:
  Panel(Game& g);
  virtual ~Panel();

  //! @brief Called by VisuField, when the user move the mouse on a player.
  void displayPlayerInfo(int team_id, int player_id);

  //! @brief Hide player information.
  void hidePlayerInfo();
  
  void setTurn(int player_id, int cur_turn);
    
  virtual void update();

private:
  Game&         g_;
  
  Surface       bg_;
  Sprite        wheel_;

  // Player information
  bool          showing_player_info_;
  Surface       player_picture_;
  TextSurface   name_;
  TextSurface   position_;
  TextSurface   ma_;
  TextSurface   mar_;
  TextSurface   st_;
  TextSurface   ag_;
  TextSurface   av_;
  TextSurface   misc_;
  TextSurface   skills_;
  Sprite        digit_score_[2];
  Sprite        digit_reroll_[2];
  Sprite        digit_time_[3];

  Surface       turn_[2];
};

END_NS(sdlvisu);

#endif /* !PANEL_HH_ */
