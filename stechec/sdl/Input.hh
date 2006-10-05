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

#ifndef INPUT_HH_
# define INPUT_HH_

/*!
** @file sdl/Input.hh
** @ingroup sdl_base
*/

# include <SDL.h>
# include "tools.hh"

/*!
** Hold keyboard and mouse state.
** @ingroup sdl_base
**
** * For mouse absolute position and delta motion, this should be
**   obvious.
** * For mouse button, use constants SDL_BUTTON_LEFT, ..., see
**   SDL_MouseButtonEvent(5).
** * For keyboard, two informations are available. Keys currently
**   pressed, and keys being pressed since the last updat.e occured.
**   Access key using ascii character, eg:
**    if (key_['b']) {...} 
** * For keyboard modifier, use class convenience function.
**
** It also provides a easy way to gain exclusive access to the keyboard,
** by locking and unlocking it.
*/
class Input
{
public:
  Input();
  ~Input();

  static Input* getInst();

  //! @brief To call before a serie of update();
  void reset();
  
  //! @brief Update Input structure. To use in the main loop only.
  void update(SDL_Event& event);

  //! @brief See if keyboard modifier @c mod is down.
  //! Use constants KMOD_CTRL, ... as specified in SDL_GetModState(5).
  bool isModDown(int mod) const;
  //! @brief Same as isModDown, but since last update.
  bool isModPressed(int mod) const;

  //! @brief Locks the keyboard, so that key_ and key_pressed_
  //!  will always be empty, until it is unlocked with the same lock_id.
  void lockKeyboard(const std::string& lock_id);
  //! @brief Unlock the keyboard.
  //! @return true if unlock successful (same lock_id), else false.
  bool unlockKeyboard(const std::string& lock_id);
  //! @brief Check if the keyboard is locked.
  bool isKeyboardLocked() const;

  const std::string& getString() const;

  Point mouse_delta_;       ///< Mouse motion since last update.
  Point mouse_;             ///< Mouse position.
  bool button_[10];         ///< Mouse button currently down.
  bool button_pressed_[10]; ///< Mouse button pressed since last update.
  bool key_[SDLK_LAST];           ///< Keys currently pressed.
  bool key_pressed_[SDLK_LAST];   ///< Keys pressed between last updat and now.

private:
  int modifier_;
  int modifier_pressed_;

  std::string lock_id_;
  bool key_lock_[SDLK_LAST];
  bool key_pressed_lock_[SDLK_LAST];
  std::string string_;
  std::string string_lock_;

  static Input* inst_;
};

#endif /* !INPUT_HH_ */
