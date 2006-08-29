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

#ifndef INPUTTEXTSURFACE_HH_
# define INPUTTEXTSURFACE_HH_

# include "TextSurface.hh"

/*
** @brief Implements a TextBox, to let user type text.
** @ingroup sdl_base
**
** This is implemented on top of TextSurface, and isn't really nice
** to render custom text, ie a cursor.
** Moreover, some problems may exists when user mix special keys
** and text, especially when the framerate is low (because special
** keys are not treated in the order they were typed).
*/
class InputTextSurface: public TextSurface
{
public:
  InputTextSurface();
  InputTextSurface(const std::string& font_name, int surf_width);
  virtual ~InputTextSurface();

  //! @brief Get exclusive usage of the keyboard, and begin to grab text.
  void acquireInput(const std::string& lock_id);
  //! @brief User has finished to type text.
  bool isAcquireFinished() const;
  //! @brief Reset the IsAcquireFinished function.
  void resetAcquire();

  virtual void update();

private:
  std::string lock_id_;
  bool have_text_;       ///< User has finished typing text.
  int index_;
};

#endif /* !INPUTTEXTSURFACE_HH_ */
