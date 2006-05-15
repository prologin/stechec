/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#include "Server.hh"

void Server::ChangeState(Coleoptere* col, int state)
{
  if (state == STATE_DEAD)
    {
      g_->terrain_coleoptere[col->get_x()][col->get_y()] = NULL;

      //If the coleoptere was in the middle of constructing a building,
      //the building is considered as dead.
      if (col->build_time > 0)
        col->dock_build->state = b_dead;
    }

  col->state = state;
  SendToAll(COLEOPTERA_CHANGE_STATE, col->get_player(), 2, col->get_id(), STATE_BUILDING);
}

void Server::UpdatePlasma(Coleoptere* col, int qte)
{
  col->stock_plasma = qte;
  SendToAll(UPDATE_PLASMA_STOCK, col->get_player(), 3, col->get_id(), col->stock_plasma, 0);
}

void Server::UpdatePlasma(Factory* build, int qte)
{
  build->stock_plasma = qte;
  SendToAll(UPDATE_PLASMA_STOCK, build->get_player(), 3, build->get_id(), build->stock_plasma, 1);
}

void Server::UpdateOre(Coleoptere* col, int qte)
{
  col->stock_ore = qte;
  SendToAll(UPDATE_ORE_STOCK, col->get_player(), 3, col->get_id(), col->stock_ore, 0);
}

void Server::UpdateOre(Factory* build, int qte)
{
  build->stock_ore = qte;
  SendToAll(UPDATE_ORE_STOCK, build->get_player(), 3, build->get_id(), build->stock_ore, 1);
}
