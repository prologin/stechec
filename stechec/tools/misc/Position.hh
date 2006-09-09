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

#ifndef POSITION_HH_
# define POSITION_HH_

template <typename T> class TPoint;
typedef TPoint<int> Point;

/*! @brief Handy class to describe a position.
**
** It is just a point, with a set of methods that makes it useful.
**
** @author victor
** @date 10/02/2006
*/
class Position
{
public:
  Position();
  Position(int row, int col);
  Position(const Point& pos);

  int   getRow() const;
  int   getCol() const;
  void  setRow(int row);
  void  setCol(int col);
  
  int   getX() const;
  int   getY() const;
  void  setX(int x);
  void  setY(int y);

  //! Check if another point is in the neighbourhood.
  //! The distance is calculated using the absolute norm.
  //! @param from The other point to check for.
  //! @param dist Distance allowed, diagonal authorized.
  bool isNear(const Position& from, int dist = 1) const;

  //! @brief Return the distance between the two point.
  float distance(const Position& to) const;
  
  friend bool operator== (const Position& lhs, const Position& rhs);
  friend bool operator!= (const Position& lhs, const Position& rhs);
  friend Position operator+ (const Position& lhs, const Position& rhs);
  friend Position operator- (const Position& lhs, const Position& rhs);
  friend Position operator* (int factor, const Position& rhs);
  friend std::ostream& operator<< (std::ostream& os, const Position& p);
  Position& operator+= (const Position& rhs);

  int   row;  ///< Row position.
  int   col;  ///< Column position.
};

# include "misc/Point.hh"

typedef std::deque<Position>    PosList;
typedef PosList::const_iterator PosIter;

inline Position::Position()
  : row(-1),
    col(-1)
{
}

inline Position::Position(int row, int col)
  : row(row),
    col(col)
{
}


inline Position::Position(const Point& pos)
  : row(pos.y),
    col(pos.x)
{
}

inline int   Position::getRow() const
{
  return row;
}

inline int   Position::getCol() const
{
  return col;
}

inline void  Position::setRow(int row)
{
  this->row = row;
}

inline void  Position::setCol(int col)
{
  this->col = col;
}

inline int   Position::getX() const
{
  return col;
}

inline int   Position::getY() const
{
  return row;
}

inline void  Position::setX(int x)
{
  col = x;
}

inline void  Position::setY(int y)
{
  row = y;
}


inline bool Position::isNear(const Position& from, int dist) const
{
  return abs(from.row - row) <= dist && abs(from.col - col) <= dist;
}

inline float Position::distance(const Position& to) const
{
  return sqrtf((float)(abs((to.row * to.row - row * row)
			   + (to.col * to.col - col * col))));
}

inline bool operator== (const Position& lhs, const Position& rhs)
{
  return lhs.row == rhs.row && lhs.col == rhs.col;
}

inline bool operator!= (const Position& lhs, const Position& rhs)
{
  return lhs.row != rhs.row || lhs.col != rhs.col;
}

inline Position operator+ (const Position& lhs, const Position& rhs)
{
  return Position(lhs.row + rhs.row, lhs.col + rhs.col);
}

inline Position operator- (const Position& lhs, const Position& rhs)
{
  return Position(lhs.row - rhs.row, lhs.col - rhs.col);
}

inline Position operator* (int factor, const Position& rhs)
{
  return Position(rhs.row * factor, rhs.col * factor);
}

inline std::ostream& operator<< (std::ostream& os, const Position& p)
{
  os << "[row/y:" << p.row << " col/x:" << p.col << "]";
  return os;
}

inline Position& Position::operator+= (const Position& rhs)
{
  row += rhs.row;
  col += rhs.col;
  return *this;
}

#endif /* !POSITION_HH_ */
