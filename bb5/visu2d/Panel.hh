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

# include "VirtualSurface.hh"
# include "Sprite.hh"

class Game;

/*
** Game panel (on the right).
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
  
  virtual void update();

private:
  Game&         g_;
  
  Surface       bg_;
  Sprite        wheel_;

  // Player information
  bool          showing_player_info_;
  Surface       player_picture_;
};

#endif /* !PANEL_HH_ */
