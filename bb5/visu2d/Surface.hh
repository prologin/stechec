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

# include <SDL.h>
# include "tools.hh"

class VirtualSurface;

/*!
** @brief Basic encapsulation of SDL_Surface structure.
**
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

  Point         getPos() const;
  Point         getSize() const;
  const Rect&   getRect() const;
  double        getZoom() const;
  double        getAngle() const;
  int           getZ() const;
  SDL_Surface*  getSDLSurface();

  void          setPos(const Point& pos);
  void          setPos(int x, int y);
  void          setSize(const Point& size);
  virtual void  setZoom(double zoom);
  virtual void  setAngle(double angle);
  virtual void  setZ(int z);


  //! @brief In this method you can do all you want with your object.
  virtual void  update();

  //! @brief Get zone to render.
  //! rect_ can be larger than we really want to show. Used only on render().
  virtual Rect  getRenderRect() const;
  
  //! @brief Don't care of this. Should only be overriden by VirtualSurface.
  virtual void  render() {}

  virtual void blit(Surface& to);
  virtual void blit(Surface& to, const Rect& to_rect, const Rect& from_rect);

  class ZSort : public std::binary_function<const Surface*, const Surface*, bool>
  {
  public:
    bool operator()(const Surface* lhs, const Surface* rhs)
    { return lhs->z_ < rhs->z_; }
  };

  friend bool operator< (const Surface& lhs, const Surface& rhs);
  friend std::ostream& operator<< (std::ostream& os, const Surface& s);
  
protected:
  SDL_Surface*  surf_;

private:
  std::string   filename_;

  Rect          rect_;
  double        zoom_;
  double        angle_;
  int           z_;

protected:
  bool            redraw_all_;
  VirtualSurface* parent_;

  friend class VirtualSurface;
  friend class ResourceCenter; // for stats
};

#endif /* !SURFACE_HH_ */
