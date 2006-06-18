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

#ifndef POINT_HH_
# define POINT_HH_

class Position;
template <typename T> class TPoint;

template <typename T> bool operator== (const TPoint<T>&, const TPoint<T>&);
template <typename T> bool operator!= (const TPoint<T>&, const TPoint<T>&);
template <typename T> TPoint<T> operator+ (const TPoint<T>&, const TPoint<T>&);
template <typename T> TPoint<T> operator- (const TPoint<T>&, const TPoint<T>&);
template <typename T> TPoint<T> operator* (const TPoint<T>&, double factor);
template <typename T> TPoint<T> operator/ (const TPoint<T>&, T factor);
template <typename T> std::ostream& operator<< (std::ostream&, const TPoint<T>&);


/*!
** Handy class to describe a point in x-y coordinate, for GUIs.
**
** Conversion from/to Position are made implicit, to avoid mistake.
*/
template <typename T>
class TPoint
{
public:

  TPoint();
  TPoint(T x, T y);
  template <typename U>
  TPoint(const TPoint<U>& pos);
  TPoint(const Position& pos);

  T     getRow() const;
  T     getCol() const;
  void  setRow(T row);
  void  setCol(T col);
  
  T     getX() const;
  T     getY() const;
  void  setX(T x);
  void  setY(T y);

  //! @brief Return the distance between the two points.
  double distance(const TPoint& to) const;

  friend bool operator== <> (const TPoint<T>& lhs, const TPoint<T>& rhs);
  friend bool operator!= <> (const TPoint<T>& lhs, const TPoint<T>& rhs);
  friend TPoint<T> operator+ <> (const TPoint<T>& lhs, const TPoint<T>& rhs);
  friend TPoint<T> operator- <> (const TPoint<T>& lhs, const TPoint<T>& rhs);
  friend TPoint<T> operator* <> (const TPoint<T>& lhs, double factor);
  friend TPoint<T> operator/ <> (const TPoint<T>& lhs, T factor);
  friend std::ostream& operator<< <> (std::ostream& os, const TPoint<T>& p);

  T x;  ///< X coordinate (horizontal).
  T y;  ///< Y coordinate (vertical).
};

typedef TPoint<int>     Point;
typedef TPoint<double>  DPoint;

# include "misc/Position.hh"

template <typename T>
inline TPoint<T>::TPoint()
  : x(-1),
    y(-1)
{
}

template <typename T>
inline TPoint<T>::TPoint(T x, T y)
  : x(x),
    y(y)
{
}

template <typename T>
template <typename U>
TPoint<T>::TPoint(const TPoint<U>& pos)
  : x(static_cast<T>(pos.x)),
    y(static_cast<T>(pos.y))
{
}

template <typename T>
inline TPoint<T>::TPoint(const Position& pos)
  : x(pos.col),
    y(pos.row)
{
}

template <typename T>
inline T   TPoint<T>::getRow() const
{
  return y;
}

template <typename T>
inline T   TPoint<T>::getCol() const
{
  return x;
}

template <typename T>
inline void  TPoint<T>::setRow(T row)
{
  y = row;
}

template <typename T>
inline void  TPoint<T>::setCol(T col)
{
  x = col;
}
  
template <typename T>
inline T   TPoint<T>::getX() const
{
  return x;
}

template <typename T>
inline T   TPoint<T>::getY() const
{
  return y;
}

template <typename T>
inline void  TPoint<T>::setX(T x)
{
  this->x = x;
}

template <typename T>
inline void  TPoint<T>::setY(T y)
{
  this->y = y;
}

template <typename T>
inline double TPoint<T>::distance(const TPoint& to) const
{
  return sqrt((double)((to.x - x) * (to.x - x) + (to.y - y) * (to.y - y)));
}

template <typename T>
inline bool operator== (const TPoint<T>& lhs, const TPoint<T>& rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <typename T>
inline bool operator!= (const TPoint<T>& lhs, const TPoint<T>& rhs)
{
  return !(lhs.x == rhs.x && lhs.y == rhs.y);
}

template <typename T>
inline TPoint<T> operator+ (const TPoint<T>& lhs, const TPoint<T>& rhs)
{
  return TPoint<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}

template <typename T>
inline TPoint<T> operator- (const TPoint<T>& lhs, const TPoint<T>& rhs)
{
  return TPoint<T>(lhs.x - rhs.x, lhs.y - rhs.y);
}

template <typename T>
inline TPoint<T> operator* (const TPoint<T>& lhs, double factor)
{
  return TPoint<T>((T)(lhs.x * factor), (T)(lhs.y * factor));
}

template <typename T>
inline TPoint<T> operator/ (const TPoint<T>& lhs, T factor)
{
  return TPoint<T>((T)(lhs.x / factor), (T)(lhs.y / factor));
}

template <typename T>
inline std::ostream& operator<< (std::ostream& os, const TPoint<T>& p)
{
  os << "[x:" << p.x << " y:" << p.y << "]";
  return os;
}

#endif /* !POINT_HH_ */
