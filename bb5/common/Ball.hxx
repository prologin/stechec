/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2007 The TBT Team.
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

template <typename T>
inline Ball<T>::Ball()
  : owner_(NULL)
{
  pos_ = Position(-1,-1);
}

template <typename T>
inline Ball<T>::~Ball()
{
}

template <typename T>
inline void Ball<T>::setPosition(const Position& pt)
{
  pos_ = pt;
}

template <typename T>
inline const Position& Ball<T>::getPosition() const
{
  return pos_;
}

template <typename T>
inline T* Ball<T>::getOwner() const
{
  return owner_;
}

template <typename T>
inline void Ball<T>::setOwner(T* player)
{
  owner_ = player;
}
