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

#include "ClientDiffer.hh"

ClientDiffer::ClientDiffer(GameData* game, Client* client)
  : StechecClientDiffer(game, client)
{
}

void ClientDiffer::ApplyDiff(const StechecPkt *pkt)
{
  switch (pkt->type)
    {
      case INIT_DATA :
	switch (pkt->arg[0]) {
	case INIT_MAP_SIZE :
	  {
	    int size_x = pkt->arg[2];
	    int size_y = pkt->arg[1];
	    assert(size_x >= 4 && size_y >= 4 && size_x <= MAP_MAX_X && size_y <= MAP_MAX_Y);
	    g_->_map_size_x = size_x;
	    g_->_map_size_y = size_y;
	    LOG4("Receiving map size %1 %2", size_y, size_x);
	    break;
	  }
	}
	break;
    case MAP_CONTENT :
      {
	int y = pkt->arg[0];
	int x = pkt->arg[1];
	int c = pkt->arg[2];
	assert( g_->_map_size_y != -1);
	assert( y >= 0 && x >= 0 && x < g_->_map_size_x && y < g_->_map_size_y );
	LOG4("Received map content in %1 %2 : %3", y,x,c);
	g_->_map[y][x] = (unsigned char) c;
	break;
      }
    default:
      {
        LOG2("Differ get unexpected message, id: %1", pkt->type);
        break;
      }
    }
}
