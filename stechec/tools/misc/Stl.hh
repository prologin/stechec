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

#ifndef STL_HH_
# define STL_HH_

struct Deleter
{
  template<class T>
  T* operator()(T* x)
  {
    delete x;
    return 0;
  }
};

struct ArrayDeleter
{
  template<class T>
  T* operator()(T* x)
  {
    delete[] x;
    return 0;
  }
};

inline std::string trim(const std::string& s)
{
  if (s.length() == 0)
    return s;
  int b = s.find_first_not_of(" \t");
  int e = s.find_last_not_of(" \t");
  if (b == -1) // No non-spaces
    return "";
  return std::string(s, b, e - b + 1);
}

// std::for_each with less typing.
template <typename Collection, typename Function>
inline Function for_all(Collection &c, const Function &f) 
{
  return std::for_each(c.begin(), c.end(), f);
}

#endif /* !STL_HH_ */
