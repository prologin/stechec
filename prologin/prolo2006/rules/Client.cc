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

#include "GameData.hh"
#include "Client.hh"

void	Client::InitData ()
{
  memset(player_fog, 0, sizeof (uint16_t) * MAX_TEAM * MAX_MAP_SIZE * MAX_MAP_SIZE);
}

void	Client::FreeData ()
{
}


void	Client::update_fog(unsigned player_id,
			   struct Position& ref,
			   struct Position& pos,
			   char sens)
{
   float	sqr;

   float	row;
   float	col;

   float	row1;
   float	col1;

   sqr = sqrt(((pos.row - ref.row) * (pos.row - ref.row))
	      + ((pos.col - ref.col) * (pos.col - ref.col))) * 1.3;
   if (sqr == 0)
      return;
   row = (float)(pos.row - ref.row) / sqr;
   col = (float)(pos.col - ref.col) / sqr;
   row1 = ref.row + 0.5;
   col1 = ref.col + 0.5;
   while ((row < 0 ? (int)row1 >= pos.row : (int)row1 <= pos.row)
	  && (col < 0 ? (int)col1 >= pos.col : (int)col1 <= pos.col))
   {
      if (sens < 0)
	 assert(player_fog[player_id][(int)row1][(int)col1] > 0);
      player_fog[player_id][(int)row1][(int)col1] += sens;
      if (IS_OPAQUE((int)row1, (int)col1))
	 break;
      row1 += row;
      col1 += col;
   }
}

void	Client::change_fog(unsigned player_id,
		   struct Position& ref,
		   char sens)
{

   Position	pos;

   for (pos.row = 0, pos.col = 0; pos.col < g_->map_size.col; pos.col++)
      update_fog(player_id, ref, pos, sens);

   for (pos.row = g_->map_size.row - 1, pos.col = 0;
	pos.col < g_->map_size.col; pos.col++)
      update_fog(player_id, ref, pos, sens);

   for (pos.row = 0, pos.col = 0; pos.row < g_->map_size.row; pos.row++)
      update_fog(player_id, ref, pos, sens);

   for (pos.row = 0, pos.col = g_->map_size.col - 1;
	pos.row < g_->map_size.row; pos.row++)
      update_fog(player_id, ref, pos, sens);

}

// Mode Vision qui passe pas a travers les murs

void		Client::UpdateFogOfWar(unsigned player_id, struct Position& from,
				       struct Position& to, bool dec)
{
   if (dec)
      change_fog(player_id, from, -1);
   change_fog(player_id, to, 1);
}
