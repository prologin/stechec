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

#ifndef EXCEPTIONDEFINE_HH_
# define EXCEPTIONDEFINE_HH_

/*!
** @brief Add few #define helper for exception.
**
** This file is included after logger, but before the rest of the world.
*/

# define THROW(Excpt, Msg)                      \
{                                               \
  std::ostringstream os;                        \
  os << Msg;                                    \
  throw Excpt(os.rdbuf()->str());               \
}

# define PRINT_AND_THROW(Excpt, Msg)            \
{                                               \
  std::ostringstream os;                        \
  os << Msg;                                    \
  Excpt e(os.rdbuf()->str());                   \
  ERR("Throwing " #Excpt ": %1",e);             \
  throw e;                                      \
}

#endif /* !EXCEPTIONDEFINE_HH_ */
