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

#ifndef SURFACE_HH_
# define SURFACE_HH_

/*!
** @file sdl/Surface.hh
** @ingroup sdl_base
*/

# include <SDL.h>
# include "tools.hh"

class VirtualSurface;

/*!
** @ingroup sdl_base
** @brief Basic encapsulation of SDL_Surface structure.
**
** This is the base class to display something (in rectangular shape) on the screen.
**
** A simple example (assuming you already have a SDLWindow):
** @code
**  Surface surf("img/lala.png");
**  surf.setPos(40, 30);
**  win.getScreen.addChild(&surf);
** @endcode
*/
class Surface
{
public:
  Surface();
  Surface(SDL_Surface* surf, double zoom = 1., double angle = 0., const std::string& filename = "");
  Surface(const std::string filename, double zoom = 1., double angle = 0.);
  Surface(int width, int height);
  virtual ~Surface();

  // On-copy methods.
  Surface(const Surface& s);
  Surface& operator=(const Surface& s);

  //! @brief Get the relative position of the surface.
  Point         getPos() const;

  //! @brief Get the size of the surface (width-height).
  Point         getSize() const;

  //! @brief Get the relative surface position.
  //! @note This is equivalent to: Rect(getPos(), getSize());
  const Rect&   getRect() const;

  //! @brief Get the absolute surface from the point (0, 0)
  //!   on the screen.
  //! This is where the surface is render and visible by the user on the screen.
  //! This should only be used for events, ie. to compare mouse position
  //! (which has absolute position) with this surface position.
  //! @return Absolute surface Rect.
  virtual Rect  getScreenRect() const;

  
  double        getZoom() const;
  double        getAngle() const;
  int           getZ() const;
  SDL_Surface*  getSDLSurface();

  virtual void  setPos(const Point& pos);
  void          setPos(int x, int y);
  void          setSize(const Point& size);
  virtual void  setZoom(double zoom);
  virtual void  setAngle(double angle);
  virtual void  setZ(int z);

  //! @brief Show this surface on the screen.
  //! @note This is the default.
  void          show();
  //! @brief Don't display this surface on the screen.
  //! @note Function @c update() is still called, but not @c render().
  void          hide();
  // !@brief Whether it is displayed or not.
  bool          isShown() const;

  //! @brief Create an empty SDL Surface of the given size.
  //! Used to do manual rendering with SDL functions and, at the same time,
  //! have it in the surface hierarchy.
  void create(int width, int height);

  //! @brief Load a image into this surface (.png, .jpg, .bmp, ...).
  void load(const std::string filename, double zoom = 1., double angle = 0.);

  //! @brief In this method you can do all you want with your object.
  virtual void  update();
  
  //! @brief Don't care of this. Should only be overriden by VirtualSurface.
  virtual void  render() {}

  virtual void blit(Surface& to);
  virtual void blit(Surface& to, const Rect& to_rect, const Rect& from_rect);

  // Sort by Z component. Used by VirtualSurface.
  struct ZSort : public std::binary_function<const Surface*, const Surface*, bool>
  {
    bool operator()(const Surface* lhs, const Surface* rhs);
  };

  // Sort by filename-zoom-angle. Used by ResourceCenter
  struct ImgSort : public std::binary_function<const Surface*, const Surface*, bool>
  {
    bool operator()(const Surface& lhs, const Surface& rhs);    
  };

  friend std::ostream& operator<< (std::ostream& os, const Surface& s);
  
protected:
  //! @brief Get the relative zone to render on the screen.
  //!   getRect() can be larger than we really want to show, or off-screen.
  //!   By default, return getRect(), but can be overriden.
  //! @note Only used for rendering.
  //! @return Relative Rect to render.
  virtual Rect  getRenderRect() const;

  SDL_Surface*  surf_;

private:
  std::string   filename_;

  Rect          rect_;          ///< Position & Size of surface, relative to its parent.
  Rect          orig_rect_;     ///< Rect size with zoom_ == 1.
  double        zoom_;
  double        angle_;
  int           z_;
  bool          show_;

protected:
  bool            redraw_all_;
  VirtualSurface* parent_;

  friend class VirtualSurface;
  friend class ResourceCenter; // for stats
};

#endif /* !SURFACE_HH_ */
