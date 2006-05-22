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

#ifndef INPUTEVENT_HH_
# define INPUTEVENT_HH_

# include "tools.hh"

/*
** Input (mouse, keyboard, ...) event base class.
**
** Derive from this class, then derive some of virtual methods
*/
class InputEvent
{
public:
  InputEvent();
  virtual ~InputEvent();

  //! @brief Set the object surface, absolute coordinate.
  void setObjectRect(const Rect& real_rect);
  //! @brief Set the object container, outside it, no events are reported.
  //!  (used to fake VirtualScrollableSurface madness).
  void setObjectContainer(const Rect& r);
  
protected:
  virtual void mouseMove(const Point& pos);
  virtual void mouseOut();

  bool has_focus_;

private:
  Rect obj_rect_;  ///< Object position, absolute.
  Rect container_; ///< Mouse is considered out outside of this surface.

  friend class Input;
  void privMouseMove(Point pos);
};

inline void InputEvent::mouseMove(const Point&) {}
inline void InputEvent::mouseOut() {}


#endif /* !INPUTEVENT_HH_ */
