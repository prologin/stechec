/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#ifndef OUTAGUI_HH_
# define OUTAGUI_HH_

# include <SDL.h>
# include <SDL_image.h>

# include "tools.hh"
# include "client_cx.hh"
# include "Api.hh"
# include "Event.hh"

# include "Square.hh"
# include "Object.hh"
# include "VirtualScreen.hh"
# include "ResourceCenter.hh"
# include "InfoBox.hh"

class OutAGui : public OutAnEvent
{
public:
  OutAGui(Api* api, ClientCx* ccx);
  ~OutAGui();
  
  int run();

private:

  /*!
  ** Events from rules.
  */
  virtual void endOfTurn() {}
  virtual void moveGoodman(int player_id, int unit_id, const Position& pos);

  Api*          api_;
  ClientCx*     ccx_;

  int           map_x_;
  int           map_y_;

  // SDL specifics functions and variables.
  bool          initSDL();
  void          init();
  void          initMapSquare(int x, int y);
  void          setZoom(int zoom);
  bool          drawBG();
  void          draw();

  ResourceCenter resource_;
  VirtualScreen* vscreen_;
  InfoBox*       info_box_;
  SDL_Surface*   screen_;

  Square        map_[MAX_MAP_SIZE][MAX_MAP_SIZE];
  SDLObject     delorean_;
  SDLObject     almanach_;
  SDLObject     unit_[MAX_TEAM][MAX_GOODMEN];
};

#endif /* !OUTAGUI_HH_ */
