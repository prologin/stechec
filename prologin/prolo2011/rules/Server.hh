/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2011 Prologin
*/

#ifndef SERVER_HH_
# define SERVER_HH_

# include "Contest.hh"
# include "GameData.hh"

class Server : public StechecServer
{
public:
  Server(GameData* g);
  virtual ~Server() {}

};

#endif // !SERVER_HH_
