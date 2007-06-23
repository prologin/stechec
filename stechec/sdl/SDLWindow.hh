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

# include "ResourceCenter.hh"
# include "Input.hh"
# include "VirtualSurface.hh"

namespace xml { class XMLConfig; }

/*!
** @brief Manage SDL Window.
** @ingroup sdl_base
**
** Manage creation of a SDL Window and collect its events into Input class.
**
** There could only be one instance of this class at one time. It
** contains (single across the program) instances of:
** - @b resources: keep one instance of this class. This is a
**   singleton, so it can be retrived using its own functions. see
**   ResourceCenter.
** - @b input: also keep one instance of this class, containing SDL
**   key/mouse status. You can spread a reference to this class all
**   around the program.
** - @b screen: the surface representing the screen.
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
  SDLWindow();
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

  //! @brief Check if the window was already initialized.
  bool isInitialized();

  //! @brief Initialize all the SDL stuff, or reintialize windows with a
  //!    new set of values.
  //! @param xml Window configuration (size, fullscreen, ...).
  void init(xml::XMLConfig* xml);

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
