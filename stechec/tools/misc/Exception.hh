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

#ifndef EXCEPTION_HH_
# define EXCEPTION_HH_

class Exception
{
public:
  Exception();
  Exception(const std::string& msg);
  virtual ~Exception();

  std::string what();
  friend std::ostream& operator<< (const Exception& e, std::ostream& os);
  
private:
  std::string msg_;
};


inline Exception::Exception()
  : msg_("Undefined exception")
{
}

inline Exception::Exception(const std::string& msg)
  : msg_(msg)
{
}

inline Exception::~Exception()
{
}

inline std::string Exception::what()
{
  return msg_;
}

inline std::ostream& operator<< (const Exception& e, std::ostream& os)
{
  os << e.msg_;
  return os;
}

#endif /* !EXCEPTION_HH_ */
