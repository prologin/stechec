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

#ifndef API_HH_
# define API_HH_

# include "Contest.hh"
# include "Client.hh"
# include "GameData.hh"


# define TEST_POS(x, y)                                 \
  if (x >= g_->map_size_x || y >= g_->map_size_y)       \
    return BAD_ARGUMENT;

# define IF_FOG_OR_KNOW(x, y)                                   \
  if (g_->getUid() >= 0                                         \
       && (!(c_->player_fog[g_->getUid()][x][y] > 0             \
           || c_->player_know_map[g_->getUid()][x][y])))        \
    return UNKNOWN;

# define IF_FOG(x, y)                                                   \
  if (g_->getUid() >= 0 && !(c_->player_fog[g_->getUid()][x][y] > 0))   \
    return UNKNOWN;

# define TEST_ID(id) \
  if (id >= MAX_COLEOPTERE || g_->p->coleopteres[id].state == STATE_DEAD) \
    { LOG2("Forbidden move (bad id)"); return BAD_ARGUMENT; }

# define TEST_ID_ACTION(id) \
  if (id >= MAX_COLEOPTERE || g_->p->coleopteres[id].state == STATE_DEAD || g_->p->coleopteres[id].action) \
{ LOG2("Forbidden move (bad id or it has already moved this turn)"); return BAD_ARGUMENT; }


/*!
** Method of this call are called by the candidat, throught 'interface.cc'
*/
class Api: public StechecApi
{
public:

  Api(GameData *gameData, Client* c);
  virtual ~Api() { }

protected:
  virtual void teamSwitched();

public:
  // these functions are local, no need to query the server.

  int world_size_x()
  {
    return g_->map_size_y;
  }

  int world_size_y()
  {
    return g_->map_size_x;
  }

  int teams()
  {
    return g_->getNbPlayer();
  }

  int my_team()
  {
    return g_->getUid();
  }

  int land_value(unsigned int x, unsigned int y)
  {
    TEST_POS(x, y);
    IF_FOG_OR_KNOW(x, y);
    return (int)(g_->terrain_value[x][y] *
                 ((double)g_->terrain_value_mod[x][y] / 100));
  }

  int land_type(unsigned x, unsigned y)
  {
    TEST_POS(x, y);
    IF_FOG_OR_KNOW(x, y);
    return g_->terrain_type[x][y];
  }

  int current_turn()
  {
    return g_->getCurrentTurn();
  }
  int remaining_turns()
  {
    return g_->maxTurn - g_->getCurrentTurn();
  }
  int time_remaining()
  {
    // return _caller->getTimeRemaining();
    return 1;
  }
  int extra_time_remaining()
  {
    // return _caller->getExtraTimeRemaining();
    return 1;
  }
  int coleoptera_team(unsigned x, unsigned y)
  {
    TEST_POS(x, y);
    IF_FOG(x, y);
    if (g_->terrain_coleoptere[x][y])
      return g_->terrain_coleoptere[x][y]->get_player();
    return NO_UNIT;
  }

  int building_type(unsigned x, unsigned y)
  {
    TEST_POS(x, y);
    IF_FOG_OR_KNOW(x, y);
    if (g_->terrain_building[x][y])
        return g_->terrain_building[x][y]->type;
    return NO_UNIT;
  }
  int building_team(unsigned x, unsigned y)
  {
    TEST_POS(x, y);
    IF_FOG_OR_KNOW(x, y);
    if (g_->terrain_building[x][y])
      return g_->terrain_building[x][y]->get_player();
    return NO_UNIT;
  }
  int coleoptera_id(unsigned x, unsigned y)
  {
    TEST_POS(x, y);
    IF_FOG(x, y);
    if (g_->terrain_coleoptere[x][y])
      return g_->terrain_coleoptere[x][y]->get_id();
    else
      return NO_UNIT;
  }
  int factory_id(unsigned x, unsigned y)
  {
    TEST_POS(x, y);
    IF_FOG_OR_KNOW(x, y);
    if (g_->terrain_building[x][y] &&
        (g_->terrain_building[x][y]->type == FACTORY))
      return g_->terrain_building[x][y]->get_id();
    return NO_UNIT;
  }
  int hotel_id(unsigned x, unsigned y)
  {
    TEST_POS(x, y);
    IF_FOG_OR_KNOW(x, y);
    if (g_->terrain_building[x][y] != NULL && g_->terrain_building[x][y]->type == HOTEL)
      {
        LOG3("hotel id: %1", g_->terrain_building[x][y]->get_id());
        return g_->terrain_building[x][y]->get_id();
      }
    return NO_UNIT;
  }
  int central_id(unsigned x, unsigned y)
  {
    TEST_POS(x, y);
    IF_FOG_OR_KNOW(x, y);
    if (g_->terrain_building[x][y] &&
        g_->terrain_building[x][y]->type == CENTRAL)
      return g_->terrain_building[x][y]->get_id();
    return NO_UNIT;
  }
  int mine_id(unsigned x, unsigned y)
  {
    TEST_POS(x, y);
    IF_FOG_OR_KNOW(x, y);
    if (g_->terrain_building[x][y] &&
        g_->terrain_building[x][y]->type == MINE)
      return g_->terrain_building[x][y]->get_id();
    return NO_UNIT;
  }
  int coleoptera_x(unsigned id)
  {
    TEST_ID(id);
    return g_->p->coleopteres[id].get_x();
  }
  int coleoptera_y(unsigned id)
  {
    TEST_ID(id);
    return g_->p->coleopteres[id].get_y();
  }
  int coleoptera_ore(unsigned id)
  {
    TEST_ID(id);
    return g_->p->coleopteres[id].stock_ore;
  }
  int coleoptera_plasma(unsigned id)
  {
    TEST_ID(id);
    return g_->p->coleopteres[id].stock_plasma;
  }
  int coleoptera_damage(unsigned id)
  {
    TEST_ID(id);
    return g_->p->coleopteres[id].get_damage();
  }
  int coleoptera_state(unsigned id)
  {
    TEST_ID(id);
    return g_->p->coleopteres[id].state;
  }
  int factory_y(unsigned id)
  {
    if (id >= g_->p->factory_count)
      return BAD_ARGUMENT;
    if (g_->p->factories[id].state != b_normal)
      return BAD_ARGUMENT;
    return g_->p->factories[id].get_y();
  }
  int factory_x(unsigned id)
  {
    if (id >= g_->p->factory_count)
      return BAD_ARGUMENT;
    if (g_->p->factories[id].state != b_normal)
      return BAD_ARGUMENT;
    return g_->p->factories[id].get_x();
  }
  int factory_plasma(unsigned id)
  {
    if (id >= g_->p->factory_count)
      return BAD_ARGUMENT;
    if (g_->p->factories[id].state != b_normal)
      return BAD_ARGUMENT;
    return g_->p->factories[id].stock_plasma;
  }
  int factory_ore(unsigned id)
  {
    if (id >= g_->p->factory_count)
      return BAD_ARGUMENT;
    if (g_->p->factories[id].state != b_normal)
      return BAD_ARGUMENT;
    return g_->p->factories[id].stock_ore;
  }
  int factory_state(unsigned id)
  {
    if (id >= g_->p->factory_count)
      return BAD_ARGUMENT;
    if (g_->p->factories[id].state != b_normal)
      return BAD_ARGUMENT;
    if (g_->p->factories[id].buildlist == 0)
      return -1;
    return g_->p->factories[id].buildlist % MAKE_COLEOPTERA_TIME;
  }
  int mine_y(unsigned id)
  {
    if (id >= g_->p->mine_count)
      return BAD_ARGUMENT;
    if (g_->p->mines[id].state != b_normal)
      return BAD_ARGUMENT;
    return g_->p->mines[id].get_y();
  }
  int mine_x(unsigned id)
  {
    if (id >= g_->p->mine_count)
      return BAD_ARGUMENT;
    if (g_->p->mines[id].state != b_normal)
      return BAD_ARGUMENT;
    return g_->p->mines[id].get_x();
  }
  int central_y(unsigned id)
  {
    if (id >= g_->p->central_count)
      return BAD_ARGUMENT;
    if (g_->p->centrals[id].state != b_normal)
      return BAD_ARGUMENT;
    return g_->p->centrals[id].get_y();
  }
  int central_x(unsigned id)
  {
    if (id >= g_->p->central_count)
      return BAD_ARGUMENT;
    if (g_->p->centrals[id].state != b_normal)
      return BAD_ARGUMENT;
    return g_->p->centrals[id].get_x();
  }
  int hotel_y(unsigned id)
  {
    if (id >= g_->p->hotel_count)
      return BAD_ARGUMENT;
    if (g_->p->hotels[id].state != b_normal)
      return BAD_ARGUMENT;
    return g_->p->hotels[id].get_y();
  }
  int hotel_x(unsigned id)
  {
    if (id >= g_->p->hotel_count)
      return BAD_ARGUMENT;
    if (g_->p->hotels[id].state != b_normal)
      return BAD_ARGUMENT;
    return g_->p->hotels[id].get_x();
  }

  int hotel_value(unsigned id)
  {
    if (id >= g_->p->hotel_count)
      return BAD_ARGUMENT;
    if (g_->p->hotels[id].state != b_normal)
      return BAD_ARGUMENT;

    Hotel& h = g_->p->hotels[id];

    return (int)(g_->terrain_value[h.get_x()][h.get_y()] *
                 ((double)g_->terrain_value_mod[h.get_x()][h.get_y()] / 100));
  }

  int get_first_coleoptera()
  {
    int i;

    for (i = 0; i < MAX_COLEOPTERE && g_->p->coleopteres[i].state == STATE_DEAD; i++)
      ;
    if (i == MAX_COLEOPTERE)
      return -1;
    return i;
  }

  int get_next_coleoptera(int id)
  {
    if (id < 0)
      return INVALID_COMMAND;

    for (int i = id + 1; i < MAX_COLEOPTERE; i++)
      if (g_->p->coleopteres[i].state != STATE_DEAD)
        return i;
    return -1;
  }

  // les actions

  int move_coleoptera(unsigned id, int direction)
  {
    TEST_ID_ACTION(id);
    unsigned x, y;
    Coleoptere* col = &g_->p->coleopteres[id];

    x = col->get_x();
    y = col->get_y();

    switch (direction)
      {
      case DEC_Y: y--; break;
      case INC_Y: y++; break;
      case DEC_X: x--; break;
      case INC_X: x++; break;
      default: LOG2("Bad direction (%1)", direction);
      }
    TEST_POS(x, y);

    if (g_->terrain_type[x][y] == WATER)
      return INVALID_COMMAND;
    if (g_->p->coleopteres[id].state != STATE_NORMAL)
      return INVALID_COMMAND;

    StechecPkt com(MOVE_COLEOPTERA, -1);
    com.Push(3, id, x, y);
    SendToServer(com);

    col->action = true;
    return SUCCESS;
  }

  int dock_coleoptera_to_coleoptera(unsigned id1, unsigned id2)
  {
    TEST_ID_ACTION(id1);
    TEST_ID_ACTION(id2);

    Coleoptere* col1 = &g_->p->coleopteres[id1];
    Coleoptere* col2 = &g_->p->coleopteres[id2];

    if (col1->state != STATE_NORMAL && col2->state != STATE_NORMAL)
      return INVALID_COMMAND;
    if (abs(col1->get_x() - col2->get_x()) +
        abs(col1->get_y() - col2->get_y()) != 1)
      return INVALID_COMMAND;

    StechecPkt com(DOCK_TO_COLEOPTERA, -1);
    com.Push(2, id1, id2);
    SendToServer(com);

    col1->action = true;
    col2->action = true;
    return SUCCESS;
  }
  int dock_coleoptera_to_building(unsigned id)
  {
    TEST_ID_ACTION(id);

    Building* b = g_->terrain_building[g_->p->coleopteres[id].get_x()]
      [g_->p->coleopteres[id].get_y()];
    if (b == NULL || b->get_player() != g_->p->coleopteres[id].get_player() ||
        b->type == HOTEL)
      return INVALID_COMMAND;
    if (g_->p->coleopteres[id].state != STATE_NORMAL)
      return INVALID_COMMAND;
    StechecPkt com(DOCK_TO_BUILDING, -1);
    com.Push(1, id);
    SendToServer(com);

    g_->p->coleopteres[id].action = true;
    return SUCCESS;
  }

  int transfert(unsigned id, unsigned ore, unsigned plasma)
  {
    TEST_ID_ACTION(id);

    Coleoptere* col = &g_->p->coleopteres[id];

    if (ore > col->stock_ore)
      return INVALID_COMMAND;
    if (plasma > col->stock_plasma)
      return INVALID_COMMAND;

    if (col->state != STATE_DOCKING)
      return INVALID_COMMAND;

    assert(col->dock_col || col->dock_build);

    if (col->dock_col == NULL &&
        (col->dock_build == NULL || col->dock_build->type != FACTORY))
      {
        LOG2("You can only dock to a factory.");
        return INVALID_COMMAND;
      }

    StechecPkt com(TRANSFERT, -1);
    com.Push(3, id, ore, plasma);
    SendToServer(com);
    col->action = true;
    return SUCCESS;
  }

  int open_canal(unsigned id)
  {
    TEST_ID_ACTION(id);
    if (g_->p->coleopteres[id].state != STATE_NORMAL)
      return INVALID_COMMAND;
    StechecPkt com (COLEOPTERA_CHANGE_STATE, -1);
    com.Push(2, id, STATE_CANAL_OPEN);
    SendToServer(com);
    g_->p->coleopteres[id].action = true;
    return SUCCESS;
  }

  int close_canal(unsigned id)
  {
    TEST_ID_ACTION(id);
    if (g_->p->coleopteres[id].state != STATE_CANAL_OPEN)
      return INVALID_COMMAND;
    StechecPkt com (COLEOPTERA_CHANGE_STATE, -1);
    com.Push(2, id, STATE_NORMAL);
    SendToServer(com);
    g_->p->coleopteres[id].action = true;
    return SUCCESS;
  }

  int throw_plasma(unsigned id, unsigned direction, unsigned quantity)
  {
    TEST_ID_ACTION(id);
    if (quantity > g_->p->coleopteres[id].stock_plasma)
      return INVALID_COMMAND;
    if (g_->p->coleopteres[id].state != STATE_NORMAL)
      return INVALID_COMMAND;
    if (direction > 3)
      return INVALID_COMMAND;

    StechecPkt com(THROW_PLASMA, -1);
    switch (direction)
      {
      case DEC_Y: com.Push(4, id, quantity, 0, -1); break;
      case INC_Y: com.Push(4, id, quantity, 0, 1); break;
      case DEC_X: com.Push(4, id, quantity, -1, 0); break;
      case INC_X: com.Push(4, id, quantity, 1, 0); break;
      }
    SendToServer(com);
    g_->p->coleopteres[id].action = true;
    return SUCCESS;
  }

  int make_building(unsigned id, unsigned type)
  {
    TEST_ID_ACTION(id);
    int x = g_->p->coleopteres[id].get_x();
    int y = g_->p->coleopteres[id].get_y();

    if (type >= 4)
      return BAD_ARGUMENT;
    if (g_->p->coleopteres[id].stock_ore < ORE_BUILDING)
      return INVALID_COMMAND;
    if (g_->p->coleopteres[id].stock_plasma < PLASMA_BUILDING)
      return INVALID_COMMAND;
    if (g_->terrain_building[x][y])
      return INVALID_COMMAND;
    if (g_->p->coleopteres[id].state != STATE_NORMAL)
      return INVALID_COMMAND;
    if ((type == MINE && g_->terrain_type[x][y] != ORE) ||
        (type == CENTRAL && g_->terrain_type[x][y] != PLASMA))
      return INVALID_COMMAND;
    StechecPkt com(MAKE_BUILDING, -1);
    com.Push(2, id, type);
    SendToServer(com);
    g_->p->coleopteres[id].action = true;
    return SUCCESS;
  }

  int make_coleoptera(unsigned id, unsigned count)
  {
    if (id >= g_->p->central_count)
      return BAD_ARGUMENT;

    Factory* f = &g_->p->factories[id];

    if (f->stock_ore < count * ORE_COLEOPTERA)
      return INVALID_COMMAND;
    if (f->stock_plasma < count * PLASMA_COLEOPTERA)
      return INVALID_COMMAND;

    StechecPkt com(MAKE_COLEOPTERA, -1);
    com.Push(2, id, count);
    SendToServer(com);
    return SUCCESS;
  }

  int repair_coleoptera_by_coleoptera(int id1, int id2)
  {
    TEST_ID_ACTION(id1);
    TEST_ID_ACTION(id2);

    Coleoptere* col1 = &g_->p->coleopteres[id1];
    Coleoptere* col2 = &g_->p->coleopteres[id2];

    if (col1->state != STATE_NORMAL)
      return INVALID_COMMAND;
    if (abs(col1->get_x() - col2->get_x()) + abs(col1->get_y() - col2->get_y()) != 1)
      return INVALID_COMMAND;

    if (col1->stock_ore < 50 || col1->stock_plasma < 10)
      return INVALID_COMMAND;

    StechecPkt com(REPAIR_COLEOPTERA, -1);
    com.Push(2, id1, id2);
    SendToServer(com);

    col1->action = true;
    return SUCCESS;
  }

  int repair_coleoptera_by_factory(int id)
  {
    TEST_ID_ACTION(id);

    Coleoptere* col = &g_->p->coleopteres[id];
    int x = col->get_x();
    int y = col->get_y();

    if (g_->terrain_building[x][y] == NULL || g_->terrain_building[x][y]->type != FACTORY)
      return INVALID_COMMAND;
    if (g_->terrain_building[x][y]->get_player() != col->get_player())
      return INVALID_COMMAND;

    StechecPkt com(REPAIR_COLEOPTERA_BY_FACTORY, -1);
    com.Push(1, id);
    SendToServer(com);

    return SUCCESS;
  }
};

#endif /* !API_HH_ */
