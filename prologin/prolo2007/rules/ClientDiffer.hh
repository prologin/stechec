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

#ifndef CLIENTDIFFER_HH_
# define CLIENTDIFFER_HH_

# include "Contest.hh"
# include "GameData.hh"
# include "Client.hh"

class	ClientDiffer : public StechecClientDiffer
{
public:
  ClientDiffer(GameData* game, Client* client) :
    StechecClientDiffer(game, client)
  { }

  // Dispatcheur de s_com
  virtual void ApplyDiff(const StechecPkt *com);

  bool in_map(int row, int col)
  {
    return row < g_->map_size.row && col < g_->map_size.col;
  }

private:
};

#endif // !CLIENTDIFFER_HH_
