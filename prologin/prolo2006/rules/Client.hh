/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#ifndef CLIENT_HH_
# define CLIENT_HH_

# include "Contest.hh"
# include "Constant.hh"

# define IS_OPAQUE(X,Y) ((g_->terrain_type[X][Y] == WALL || g_->terrain_type[X][Y] == BET_PLACE))

class		Client : public StechecClient
{
public:
  Client(GameData *g) : StechecClient(g) {}
  virtual ~Client() {}

  uint16_t	player_fog[MAX_TEAM][MAX_MAP_SIZE][MAX_MAP_SIZE];


  void		InitData();
  void		FreeData();
  void		UpdateFogOfWar(unsigned player_id, struct Position& from,
			       struct Position& to, bool dec);
private:
  void		change_fog(unsigned player_id,
			   struct Position& ref,
			   char sens);
  void		update_fog(unsigned player_id,
			   struct Position& ref,
			   struct Position& pos,
			   char sens);
};

#endif // !CLIENT_HH_
