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

#ifndef RECT_HH_
# define RECT_HH_

# include "misc/Point.hh"

/*!
** @brief A rectangle class.
**
** Warning: @c w and @c h should _never_ be negative.
*/
class Rect
{
public:
  Rect();
  Rect(int x, int y, int w, int h);
  Rect(const Point& pos, const Point& size);

  bool inside(const Point& pos) const;
  Point getPos() const;
  Point getSize() const;

  friend bool operator== (const Rect& lhs, const Rect& rhs);
  friend bool operator!= (const Rect& lhs, const Rect& rhs);
  Rect& operator+= (const Rect& rhs);
  friend Rect operator+ (const Rect& lhs, const Rect& rhs);
  Rect& operator+= (const Point& rhs);
  friend Rect operator+ (const Rect& lhs, const Point& rhs);
  Rect& operator-= (const Point& rhs);
  friend Rect operator- (const Rect& lhs, const Point& rhs);
  Rect& operator|= (const Rect& rhs);
  Rect& operator&= (const Rect& rhs);
  friend Rect operator| (const Rect& lhs, const Rect& rhs);
  friend Rect operator& (const Rect& lhs, const Rect& rhs);
  friend std::ostream& operator<< (std::ostream& os, const Rect& p);

public:
  int x;
  int y;
  int w;
  int h;
};

inline Rect::Rect()
  : x(0),
    y(0),
    w(0),
    h(0)
{
}

inline Rect::Rect(int x, int y, int w, int h)
  : x(x),
    y(y),
    w(w),
    h(h)
{
}

inline Rect::Rect(const Point& pos, const Point& size)
  : x(pos.x),
    y(pos.y),
    w(size.x),
    h(size.y)
{
}


inline bool Rect::inside(const Point& pos) const
{
  return pos.x >= x && pos.x <= x + w && pos.y >= y && pos.y <= y + h;
}

inline Point Rect::getPos() const
{
  return Point(x, y);
}

inline Point Rect::getSize() const
{
  return Point(w, h);
}

inline bool operator== (const Rect& lhs, const Rect& rhs)
{
  return (lhs.x == rhs.x && lhs.y == rhs.y
          && lhs.w == rhs.w && lhs.h == rhs.h);
}

inline bool operator!= (const Rect& lhs, const Rect& rhs)
{
  return !(lhs.x == rhs.x && lhs.y == rhs.y
           && lhs.w == rhs.w && lhs.h == rhs.h);

}

inline Rect& Rect::operator+= (const Rect& rhs)
{
  x += rhs.x;
  y += rhs.y;
  w += rhs.w;
  h += rhs.h;
  return *this;
}

inline Rect operator+ (const Rect& lhs, const Rect& rhs)
{
  return Rect(lhs.x + rhs.x, lhs.y + rhs.y, lhs.w + rhs.w, lhs.h + rhs.w);
}

inline Rect& Rect::operator+= (const Point& rhs)
{
  x += rhs.x;
  y += rhs.y;
  return *this;
}

inline Rect operator+ (const Rect& lhs, const Point& rhs)
{
  return Rect(lhs.x + rhs.x, lhs.y + rhs.y, lhs.w, lhs.h);
}

inline Rect& Rect::operator-= (const Point& rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}

inline Rect operator- (const Rect& lhs, const Point& rhs)
{
  return Rect(lhs.x - rhs.x, lhs.y - rhs.y, lhs.w, lhs.h);
}

inline Rect& Rect::operator|= (const Rect& rhs)
{
  int x2 = std::max(x + w, rhs.x + rhs.w);
  int y2 = std::max(y + h, rhs.y + rhs.h);
  x = std::min(x, rhs.x);
  y = std::min(y, rhs.y);
  w = x2 - x;
  h = y2 - y;
  return *this;
}

inline Rect& Rect::operator&= (const Rect& rhs)
{
  int x2 = std::min(x + w, rhs.x + rhs.w);
  int y2 = std::min(y + h, rhs.y + rhs.h);
  x = std::max(x, rhs.x);
  y = std::max(y, rhs.y);
  w = std::max(x2 - x, 0);
  h = std::max(y2 - y, 0);
  return *this;
}

inline Rect operator| (const Rect& lhs, const Rect& rhs)
{
  return Rect(lhs) |= rhs;
}

inline Rect operator& (const Rect& lhs, const Rect& rhs)
{
  return Rect(lhs) &= rhs;
}

inline std::ostream& operator<< (std::ostream& os, const Rect& p)
{
  os << "[x:" << p.x << ", y:" << p.y << ", w:" << p.w << ", h:" << p.h << "]";
  return os;
}


#endif /* !RECT_HH_ */
