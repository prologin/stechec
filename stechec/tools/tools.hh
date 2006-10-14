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

#ifndef TOOLS_HH_
# define TOOLS_HH_

/*!
** @defgroup tools Stechec tools library.
**
** Provide some useful tools.
**
** All these files are located in @c stechec/tools
**
*/

# include <sstream>
# include <iostream>
# include <string>
# include <map>
# include <vector>
# include <queue>
# include <list>
# include <set>
# include <utility>
# include <algorithm>

# include <cstdio>
# include <cstdlib>
# include <cerrno>
# include <cassert>
# include <ctime>
# include <cmath>

# define BEGIN_NS(Ns) namespace Ns {
# define END_NS(Ns) }

# include "misc/Exception.hh"
# include "misc/os_wrapper.hh"
# include "misc/Stl.hh"
# include "misc/Position.hh"
# include "misc/Point.hh"
# include "misc/Rect.hh"

# include "logger/log.hh"
# include "misc/ExceptionDefine.hh"
# include "datatfs/Packet.hh"
# include "misc/Timer.hh"
# include "misc/Library.hh"

#endif /* !TOOLS_HH_ */
