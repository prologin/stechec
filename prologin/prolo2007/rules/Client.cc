/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2007 Prologin
*/

#include "GameData.hh"
#include "Client.hh"

void	Client::InitData ()
{
   ResetFogOfWar();
}

void	Client::FreeData ()
{
}


void	Client::change_fog(unsigned player_id,
			   struct Position& ref,
			   char sens)
{

   Position	pos;
   Position	stp;

   pos.row = ref.row - g_->players[player_id].competences[VISION];
   if (pos.row < 0)
      pos.row = 0;
   pos.col = ref.col - g_->players[player_id].competences[VISION];
   if (pos.col < 0)
      pos.col = 0;
   stp.row = ref.row + g_->players[player_id].competences[VISION] + 1;
   if (pos.col >= g_->map_size.row)
      pos.col = g_->map_size.row;
   stp.col = ref.col + g_->players[player_id].competences[VISION] + 1;
   if (pos.col >= g_->map_size.col)
      pos.col = g_->map_size.col;

   for (int i = pos.row; i < stp.row; ++i)
      for (int j = pos.col; j < stp.col; ++j)
	 if ( abs(ref.row - i) + abs(ref.col - j)
	     // (2 * std::max(abs(ref.row - i) , abs(ref.col - j)) +
// 	     std::max(abs(ref.row - i) , abs(ref.col - j)))
	     <=
	     g_->players[player_id].competences[VISION])
	 {
	    player_fog[player_id][i][j] += sens;
	    //	    LOG1("FOG : x : %1, y : %2, fog : %3",
	    // j, i, player_fog[i][j]);
	 }

}


void		Client::UpdateFogOfWar(unsigned player_id, struct Position& from,
				       struct Position& to, bool dec)
{
   if (dec)
      change_fog(player_id, from, -1);
   LOG5("FOG of war wil change (normaly)");
   change_fog(player_id, to, 1);
}

void		Client::ResetFogOfWar ()
{
   memset(player_fog, 0, sizeof (uint16_t) *  MAX_MAP_SIZE * MAX_MAP_SIZE * MAX_PLAYER * MAX_TEAM);
}
