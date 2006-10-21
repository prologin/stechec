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

#ifndef VIRTUALSURFACE_HH_
# define VIRTUALSURFACE_HH_

/*!
** @file sdl/VirtualSurface.hh
** @ingroup sdl_base
*/

# include "Surface.hh"

/*!
** @ingroup sdl_base
** @brief Container for Surface.
**
** Other surface (any class derived from Surface) may be attached to
** VirtualSurface, in a father-child relationship. Rendering is done
** automatically, using this VirtualSurface as an intermediate
** SDL_Surface, in a efficient way.
*/
class VirtualSurface : public Surface
{
public:
  VirtualSurface();
  VirtualSurface(const std::string& name, SDL_Surface* surf);
  VirtualSurface(const std::string& name, int width, int height);
  virtual ~VirtualSurface();

  //! @brief Add a child to this VirtualSurface.
  //! @param child Child to add.
  //! @note You can add the same child multiple time, so take care.
  void addChild(Surface* child);
  //! @brief Remove a child to this VirtualSurface.
  //! @param child Child to remove.
  void removeChild(Surface* child);
  //! @brief Get the number of child currently contained by this VirtualSurface.
  //! @return The current number of child this VS contains.
  int getNbChild() const;

  //! @brief Private method, you shouldn't need that.
  //!  Update child Z-order in children vector, when a child change its Z
  //!  attribute.
  void updateChildZOrder();

  virtual void enable();
  virtual void show();
  virtual void setZoom(double zoom);
  virtual void update();
  virtual void render();

  //! @brief Set a marker on a rectangle that must be redrawn next time. To
  //!  call when a visual change has occured in this rectangle (like adding,
  //!  moving or removing a sprite).
  //! @param zone A rectangle relative to this VirtualSurface position.
  void invalidate(const Rect& zone);
  //! @brief Set a marker on a rectangle that must be redrawn next time. To
  //!  call when a visual change has occured in this rectangle (like adding,
  //!  moving or removing a sprite).
  //! @param pos A top-left point relative to this VirtualSurface position.
  //! @param size The size of the invalidated rectangle.
  void invalidate(const Point& pos, const Point& size);
  
protected:

  typedef std::vector<Surface*> SurfaceList;
  SurfaceList   child_list_;

  typedef std::vector<Rect> RectList;
  RectList      invalidated_surf_;

  std::string   name_;
};

#endif /* !VIRTUALSURFACE_HH_ */
