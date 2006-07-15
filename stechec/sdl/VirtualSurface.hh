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

# include "Surface.hh"

/*!
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

  void addChild(Surface* child);
  void removeChild(Surface* child);

  void updateChildZOrder();

  //! @brief Get the surface position wrt the screen (real (0,0)),
  //! not relative to its parent.
  virtual Point getAbsolutePos() const;

  virtual void setZoom(double zoom);

  virtual void update();
  virtual void render();

  void invalidate(const Rect& zone);
  void invalidate(const Point& pos, const Point& size);
  
protected:
  typedef std::vector<Surface*> SurfaceList;
  SurfaceList   child_list_;

  typedef std::vector<Rect> RectList;
  RectList      invalidated_surf_;

  std::string   name_;
};

#endif /* !VIRTUALSURFACE_HH_ */
