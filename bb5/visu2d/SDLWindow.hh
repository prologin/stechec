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

#ifndef SDLWINDOW_HH_
# define SDLWINDOW_HH_

# include "tools.hh"
# include "client_cx.hh"

# include "ResourceCenter.hh"
# include "Input.hh"
# include "VirtualSurface.hh"

/*!
** Manage creation of a SDL Window, its events, and a separate
** thread for network events.
*/
class SDLWindow
{
public:
  SDLWindow(xml::XMLConfig* xml);
  ~SDLWindow();

  Input&                getInput();
  VirtualSurface&       getScreen();
  
  void setFullscreen(bool enable);

  //! @brief Initialize all the SDL stuff.
  void init();

  //! @brief Update SDL events, update screen (and all his children),
  //!  and print one frame on the screen.
  //! @return true if the window is/was/want to be closed.
  bool processOneFrame();
  
private:
  xml::XMLConfig*       xml_;    ///< Configuration file.

  Input                 input_;
  ResourceCenter        resource_;
  VirtualSurface        screen_;

  bool                  is_fullscreen_;
  unsigned              frame_drawed_;
  unsigned              frame_last_tick_;
};

#endif /* !SDLWINDOW_HH_ */
