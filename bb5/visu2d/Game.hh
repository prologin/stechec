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

#ifndef GAME_HH_
# define GAME_HH_

# include <SDL.h>
 
# include "tools.hh"
# include "client_cx.hh"
# include "Api.hh"

# include "SDLWindow.hh"
# include "Input.hh"
# include "Surface.hh"
# include "VirtualSurface.hh"
# include "VirtualScrollableSurface.hh"
# include "Sprite.hh"
# include "Panel.hh"
# include "Field.hh"
# include "Event.hh"

class Game : public Event
{
public:
  Game(SDLWindow& win, xml::XMLConfig* xml, Api* api, ClientCx* ccx);
  ~Game();

  int run();
  
private:

  // Events
  virtual void evPlayerPos(int team_id, int player_id, const Point& pos);
  virtual void evBallPos(const Point& pos);

  SDLWindow&            win_;    ///< The SDL window.
  xml::XMLConfig*       xml_;    ///< Configuration file.
  Api*                  api_;    ///< Interface with client's rules.
  ClientCx*             ccx_;    ///< Connection with the server.

  Panel                 panel_;  ///< Game panel.
  VisuField             field_;  ///< Game field.
};

#endif /* !GAME_HH_ */
