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
** `COPYING' file in the root directory.
** 
** The TBT Team consists of people listed in the `AUTHORS' file.
*/

#ifndef STL_HH_
# define STL_HH_

//! @ingroup tools_misc
struct Deleter
{
  template<class T>
  T* operator()(T* x)
  {
    delete x;
    return 0;
  }
};

//! @ingroup tools_misc
struct ArrayDeleter
{
  template<class T>
  T* operator()(T* x)
  {
    delete[] x;
    return 0;
  }
};

//! @brief Remove white spaces and tabulations from the beginning and the end of the string.
//! @param s String to trim.
//! @ingroup tools_misc
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

//! @brief Remove all white spaces from the string.
//! @param s String to trim.
//! @ingroup tools_misc
inline void trimAll(std::string& s)
{
  s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
}

struct ToLower
{
  char operator() (char c) const
  {
    return std::tolower(static_cast<unsigned char>(c));
  }
};

//! @brief Convert all characters to lower if possible.
//! @param s String to lower.
//! @ingroup tools_misc
inline void toLower(std::string& s)
{
  std::transform(s.begin(), s.end(), s.begin(), ToLower());
}

//! @brief Implements std::for_each with less typing.
//! @ingroup tools_misc
template <typename Collection, typename Function>
inline Function for_all(Collection &c, const Function &f) 
{
  return std::for_each(c.begin(), c.end(), f);
}

#endif /* !STL_HH_ */
