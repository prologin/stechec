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

#ifndef VIRTUALSCROLLABLESURFACE_HH_
# define VIRTUALSCROLLABLESURFACE_HH_

# include "Input.hh"
# include "VirtualSurface.hh"

/*!
** @brief Scrollable kind of VirtualSurface.
** @ingroup sdl_base
** @attention It must be directly attached to a SDLWindow.
*/
class VirtualScrollableSurface : public VirtualSurface
{
public:
  VirtualScrollableSurface(const std::string& name,
                           Input& input,
                           const Point& real_size,
                           const Point& virtual_size);
  VirtualScrollableSurface(const std::string& name,
                           Input& input,
                           SDL_Surface* surf,
                           const Point& virtual_size);
  ~VirtualScrollableSurface();

  //! @brief Automatically center visible surface from its position/size
  //   only if the visible/printed surface is larger than logical surface.
  //! @param enable Switch on/off.
  //! @note Default disabled.
  void setAutomaticAdjust(bool enable);
  
  //! @brief Get the absolute surface from the point (0, 0) on the screen.
  //! This is where (on the screen) the visible surface is rendered and visible by the user.
  //! This should only be used for events, ie. to compare mouse position
  //! (which has absolute position) with this surface position.
  //! @return Absolute surface Rect.
  virtual Rect getRenderRect() const;

  //! @brief Get the absolute surface from the point (0, 0) on the logical surface.
  //! This is where (on the logical surface) the visible surface is taken from.
  //! This looks to be also used for events... ie. to compare mouse position
  //! (which has absolute position) with this surface position.
  //! (I guess that I still need an explanation... ^^)
  //! @return Visible surface Rect, relative to logical surface.
  virtual Rect getScreenRect() const;

  virtual void setPos(const Point& to);
  
  virtual void update();

  virtual void blit(Surface& to);
  virtual void blit(Surface& to, const Rect& to_rect, const Rect& from_rect);

private:
  void adjustSize(const Rect& rect);

  Input&        inp_;

  Point         real_size_;     ///< Real surface size. The printed one.
  Point         vpos_;          ///< Current position to show.
  Point         dec_;           ///< Screen motion direction.
  unsigned      last_updated_;  ///< Ticks (ms).

  //! @brief Adjust to the center of getRect() if real_size_ < getRect()
  //!  (note that in this case there is not need to scroll).
  bool		adjust_to_center_;
  bool		adjust_x_;        ///< Currently adjusting x pos.
  bool		adjust_y_;        ///< Currently adjusting y pos.
  Point		adjust_orig_pos_; ///< Original position before adjustment.
};

#endif /* !VIRTUALSCROLLABLESURFACE_HH_ */
