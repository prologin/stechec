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

  virtual Rect getRealRect() const;
  virtual Point getAbsolutePos() const;

  virtual void update();

  virtual void blit(Surface& to);
  virtual void blit(Surface& to, const Rect& to_rect, const Rect& from_rect);
  
private:
  Input&        inp_;

  Point         real_size_;     ///< Real surface size. The printed one.
  Point         vpos_;          ///< Current position to show.
  Point         dec_;           ///< Screen motion direction.
  unsigned      last_updated_;  ///< Ticks (ms).
};

#endif /* !VIRTUALSCROLLABLESURFACE_HH_ */
