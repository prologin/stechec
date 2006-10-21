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
** This module basicly defines a wrapper in C++, but it turns out that
** this is an (almost) complete SDL engine.  It provides:
** - resouces (picture, fonts) loading and caching
** - encapsulation of all basic SDL functions in C++ classes
** - creation of a SDL Window described by an xml file
** - hierachical handling of surfaces
** - handling IOLoop, updating and rendering surfaces are done automatically
** - provides more high-level functions (like sprite animation)
**
** Thus, the module's user doesn't have to care with SDL low-level
** details. The downside is that does things quite differently than
** SDL (is does it the way this module writer wanted), and may
** contains lots of bugs. It is mainly targeted to do what is needed,
** but with little adaptation it can be used everywhere SDL drawing is
** required.
** 
** A more complete documentation is available (in french) in in
** propal.pdf. All sources and headers files are located in @c
** stechec/sdl
**
** Some examples are available through this documentation, and
** there are some more working examples in @c stechec/sdl/test
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

namespace xml { class XMLConfig; }

/*!
** @ingroup sdl_base
** @brief Manage SDL Window.
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
