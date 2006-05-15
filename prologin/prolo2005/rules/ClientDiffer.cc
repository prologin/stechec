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

#include "ClientDiffer.hh"

/*
** Dispatch le s_com
*/
void ClientDiffer::ApplyDiff(const StechecPkt *com)
{
  Player* p = NULL;
  if (com->client_id >= 0)
    p = &g_->players[com->client_id];

  switch (com->type)
    {
    case MAP_CONTENT:
      {
        int x = com->arg[0];
        int y = com->arg[1];
        g_->terrain_type[x][y] = com->arg[2];
        g_->terrain_value[x][y] = com->arg[3];
        break;
      }

    case UPDATE_LAND_VALUE:
      {
        int x = com->arg[0];
        int y = com->arg[1];
        int v = com->arg[2];
        g_->terrain_value_mod[x][y] = v;
      }
      break;

    case NEW_UNIT:
      {
        assert(p != NULL);
        int id = com->arg[0];
        int x = com->arg[1];
        int y = com->arg[2];

        LOG3("New unit, update fog " << x << ":" << y);
        g_->terrain_coleoptere[x][y] = &p->coleopteres[id];

        p->coleopteres[id].set_x(x);
        p->coleopteres[id].set_y(y);
        p->coleopteres[id].state = STATE_NORMAL;
        p->coleopteres[id].set_player(com->client_id);
        p->coleopteres[id].set_id(id);
        p->coleopteres[id].Reinit();

        c_->UpdateFogOfWar(com->client_id, INT_MAX, INT_MAX, x, y);
      }
      break;

    case MOVE_COLEOPTERA:
      {
        assert(p != NULL);
        int id = com->arg[0];
        int x = com->arg[1];
        int y = com->arg[2];
        Coleoptere* col = &p->coleopteres[id];

        LOG3("Differ: move coleoptere x: " << x << ", y: " << y);
        c_->UpdateFogOfWar(com->client_id, col->get_x(), col->get_y(), x, y);
        g_->terrain_coleoptere[col->get_x()][col->get_y()] = 0;
        g_->terrain_coleoptere[x][y] = col;
        col->set_x(x);
        col->set_y(y);
      }
      break;

    case MAKE_BUILDING:
      {
        make_building(com);
      }
      break;

    case MAKE_COLEOPTERA:
      {
        assert(p != NULL);
        int id = com->arg[0];
        int count = com->arg[1];
        Factory* f = &p->factories[id];

        f->buildlist += count * MAKE_COLEOPTERA_TIME;
      }
      break;

    case BUILDING_FINISHED:
      {
        int        x = com->arg[0];
        int        y = com->arg[1];

        LOG3("** BUILDING FINISHED ** client_id " << com->client_id << ", on " << x << ":" << y);
        assert(g_->terrain_coleoptere[x][y] != 0);
        g_->terrain_building[x][y] = g_->terrain_coleoptere[x][y]->dock_build;
        g_->terrain_building[x][y]->state = b_normal;
        g_->terrain_coleoptere[x][y]->dock_build = NULL;
        g_->terrain_coleoptere[x][y]->state = STATE_NORMAL;
        c_->UpdateFogOfWar(com->client_id, INT_MAX, INT_MAX, x, y);

      }
      break;

    case DOCK_TO_COLEOPTERA:
      {
        assert(p != NULL);
        int id1 = com->arg[0];
        int id2 = com->arg[1];
        Coleoptere* col1 = &p->coleopteres[id1];
        Coleoptere* col2 = &p->coleopteres[id2];

        col1->state = STATE_DOCKING;
        col1->dock_col = col2;
        col2->state = STATE_DOCKING;
        col2->dock_col = col1;
      }
      break;

    case DOCK_TO_BUILDING:
      {
        assert(p != NULL);
        int id = com->arg[0];
        Coleoptere* col = &p->coleopteres[id];

        col->state = STATE_DOCKING;
        col->dock_build = g_->terrain_building[col->get_x()][col->get_y()];
      }
      break;

    case UPDATE_PLASMA_STOCK:
      {
        int id_player = com->client_id;
        int id = com->arg[0];
        int new_stock = com->arg[1];
        int is_bat = com->arg[2];

        if (is_bat)
          g_->players[id_player].factories[id].stock_plasma = new_stock;
        else
          g_->players[id_player].coleopteres[id].stock_plasma = new_stock;
      }
      break;

    case UPDATE_ORE_STOCK:
      {
        int id_player = com->client_id;
        int id = com->arg[0];
        int new_stock = com->arg[1];
        int is_bat = com->arg[2];

        if (is_bat)
          g_->players[id_player].factories[id].stock_ore = new_stock;
        else
          g_->players[id_player].coleopteres[id].stock_ore = new_stock;
      }
      break;

    case COLEOPTERA_CHANGE_STATE:
      {
        int id_player = com->client_id;
        int id = com->arg[0];
        int state = com->arg[1];

        Coleoptere* col = &g_->players[id_player].coleopteres[id];
        col->state = state;
        if (state == STATE_DEAD)
          g_->terrain_coleoptere[col->get_x()][col->get_y()] = NULL;
      }
      break;

    case REPAIR_COLEOPTERA:
      {
        int id = com->arg[0]; // appareil a reparer
        int dom = com->arg[1];
        Coleoptere* col = &g_->p->coleopteres[id];
        col->set_damage(dom);
      }
      break;

    case REPAIR_COLEOPTERA_BY_FACTORY:
      {
        int id = com->arg[0]; // appareil a reparer, dans l'usine
        Coleoptere* col = &g_->p->coleopteres[id];
        col->state = STATE_REPAIRING;
        g_->terrain_coleoptere[col->get_x()][col->get_y()] = NULL;
      }
      break;
    }
}

void         ClientDiffer::make_building(const StechecPkt* elt)
{
  Player*        p = &g_->players[elt->client_id];
  int           id = elt->arg[0];
  int           type = elt->arg[1];

  Coleoptere*        col = &p->coleopteres[id];
  Building*        b = NULL;

  switch (type)
    {
    case HOTEL:
      assert(p->hotel_count < MAX_HOTEL);
      b = &p->hotels[p->hotel_count];
      b->set_id(p->hotel_count++);
      break;
    case MINE:
      assert(p->mine_count < MAX_MINE);
      b = &p->mines[p->mine_count];
      b->set_id(p->mine_count++);
      break;
    case CENTRAL:
      assert(p->central_count < MAX_CENTRAL);
      b = &p->centrals[p->central_count];
      b->set_id(p->central_count++);
      break;
    case FACTORY:
      assert(p->factory_count < MAX_FACTORY);
      b = &p->factories[p->factory_count];
      b->set_id(p->factory_count++);
      break;
    }

  col->dock_build = b;
  b->type = type;
  b->state = b_in_construction;
  b->set_x(col->get_x());
  b->set_y(col->get_y());
  b->set_player(col->get_player());
}
