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

#ifndef EXCEPTIONDEFINE_HH_
# define EXCEPTIONDEFINE_HH_

/*!
** @file ExceptionDefine.hh
** @brief Add few macro helpers for Exception.
** @ingroup tools_misc
**
** This file is included after logger, but before the rest of the world.
*/

//! @ingroup tools_misc
//! @{

# define THROW(Excpt, Msg)                      \
do {                                            \
  std::ostringstream os;                        \
  os << Msg;                                    \
  throw Excpt(os.rdbuf()->str());               \
} while (0)

# define PRINT_AND_THROW(Excpt, Msg)            \
do {                                            \
  std::ostringstream os;                        \
  os << Msg;                                    \
  Excpt e(os.rdbuf()->str());                   \
  ERR("Throwing " #Excpt ": %1", e.what());     \
  throw e;                                      \
} while (0)

//! @}

#endif /* !EXCEPTIONDEFINE_HH_ */
