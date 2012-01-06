/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#ifndef CLIENT_HH_
# define CLIENT_HH_

# include "Contest.hh"

class Client: public StechecClient
{
public:
  Client(GameData *g);
  virtual ~Client();

  uint16_t     ***player_fog;
  bool         ***player_know_map;

  void         InitData();
  void         UpdateFogOfWar(unsigned player_id, unsigned from_x, unsigned from_y,
			      unsigned to_x, unsigned to_y);
};

#endif /* !CLIENT_HH_ */
