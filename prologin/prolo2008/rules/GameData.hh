/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2007 Prologin
*/

#ifndef GAMEDATA_HH_
# define GAMEDATA_HH_

# include <list>
# include <map>
# include <algorithm>
# include "Contest.hh"
# include "Constant.hh"

/*!
** This class is meant to contain all data, accessible from
** everywhere.
*/
class GameData: public StechecGameData
{
public:
  GameData();

};

#endif // !GAMEDATA_HH_
