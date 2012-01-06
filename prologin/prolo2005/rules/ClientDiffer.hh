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

#ifndef DIFFER_HH_
# define DIFFER_HH_

# include "Contest.hh"
# include "GameData.hh"
# include "Client.hh"

/*!
** This class is called *only on client* (player or visio) side.
** It receives any changes made by the server after each turn.
**
** You must:
**  - handle all or parts of messages that are given to ApplyDiff
**
** You can:
**  - access to the GameData class: _g
*/
class ClientDiffer: public StechecClientDiffer
{
public:

  ClientDiffer(GameData* game, Client* client) :
    StechecClientDiffer(game, client)
  { }
  
  // Dispatcheur de s_com
  virtual void ApplyDiff(const StechecPkt *com);

  bool in_map(unsigned x, unsigned y)
  {
    return x < g_->map_size_x && y < g_->map_size_y;
  }
  
private:
  void make_building(const StechecPkt* elt);
};

#endif // !DIFFER_HH_ 
