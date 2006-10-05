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

/*!
** @defgroup sdl_base C++ wrapper for SDL.
**
** All these files are located in @c stechec/sdl
**
** There are some example of how to use it in @c stechec/sdl/test
**
** The reader is also invited to read documention about this SDL Engine
** in propal.pdf.
**
*/

/*!
** @file SDLWindow.hh
** @ingroup sdl_base
*/

# include "tools.hh"

# include "ResourceCenter.hh"
# include "Input.hh"
# include "VirtualSurface.hh"

/*!
** @ingroup sdl_base
** @brief Manage SDL Window.
**
** Manage creation of a SDL Window and collect its events into Input class.
**
** Here is a simple example of how to use it:
** @code
** int run_win(xml::XMLConfig* xml)
** {
**   SDLWindow win(xml);
**   win.init();
**   VirtualSurface& screen = win.getScreen();
**   // Do something with screen, like adding sprites.
**   while (true) {
**     Input& inp = win.getInput();
**     // Do something, this is your main loop!
**     // You can fetch mouse pos/keyboard state into 'inp'.
**     if (win.processOneFrame())
**       break;
**   }
** }
** @endcode
*/
class SDLWindow
{
public:
  //! @brief Contructor.
  //! @param xml Window configuration (size, fullscreen, ...).
  SDLWindow(xml::XMLConfig* xml);
  ~SDLWindow();

  //! @brief Get mouse and keyboard current state.
  //! @return Input instance.
  Input&                getInput();
  //! @brief Get the root surface, having the height and width of the window.
  VirtualSurface&       getScreen();

  //! @brief Check if the window is in fullscreen mode.
  //! @return true if the window is in fullscreen mode.
  bool isFullScreen() const;

  //! @brief Set the fullscreen mode.
  //! @param enable Enable/disable fullscreen mode.
  void setFullScreen(bool enable);

  //! @brief Get the number of frame per second (typically between 5 and 100 fps).
  //! @return The current number of frame per second.
  int getFps() const;
  
  //! @brief Initialize all the SDL stuff.
  void init();

  //! @brief Fill the screen with black.
  void clearScreen();

  //! @brief Update SDL events, update screen (and all his children),
  //!      and print one frame on the screen.
  //! @return true if the window is/was/want to be closed.
  bool processOneFrame();
  
private:
  xml::XMLConfig*       xml_;    ///< Window configuration file.

  Input                 input_;
  ResourceCenter        resource_;
  VirtualSurface        screen_;

  bool                  is_fullscreen_;
  unsigned              frame_drawed_;
  unsigned              frame_tick_fps_;
  unsigned              frame_previous_tick_;
  int                   fps_;
};

#endif /* !SDLWINDOW_HH_ */
