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


#include "Api.hh"

// from Api.cc
extern Api* api;

extern "C" {
  int world_size_x()
  {
    return api->world_size_x();
  }
  int world_size_y()
  {
    return api->world_size_y();
  }
  int land_value(unsigned int x, unsigned int y)
  {
    return api->land_value(x, y);
  }
  int land_type(unsigned x, unsigned y)
  {
    return api->land_type(x, y);
  }
  int current_turn()
  {
    return api->current_turn();
  }
  int remaining_turns()
  {
    return api->remaining_turns();
  }
  int time_remaining()
  {
    return api->time_remaining();
  }
  int extra_time_remaining()
  {
    return api->extra_time_remaining();
  }
  int coleoptera_team(unsigned x, unsigned y)
  {
    return api->coleoptera_team(x, y);
  }
  int building_type(unsigned x, unsigned y)
  {
    return api->building_type(x, y);
  }
  int building_team(unsigned x, unsigned y)
  {
    return api->building_team(x, y);
  }
  int coleoptera_id(unsigned x, unsigned y)
  {
    return api->coleoptera_id(x, y);
  }
  int factory_id(unsigned x, unsigned y)
  {
    return api->factory_id(x, y);
  }
  int factory_plasma(unsigned id)
  {
    return api->factory_plasma(id);
  }
  int factory_ore(unsigned id)
  {
    return api->factory_ore(id);
  }
  int factory_state(unsigned id)
  {
    return api->factory_state(id);
  }
  int hotel_id(unsigned x, unsigned y)
  {
    return api->hotel_id(x, y);
  }
  int central_id(unsigned x, unsigned y)
  {
    return api->central_id(x, y);
  }
  int mine_id(unsigned x, unsigned y)
  {
    return api->mine_id(x, y);
  }
  int coleoptera_x(unsigned id)
  {
    return api->coleoptera_x(id);
  }
  int coleoptera_y(unsigned id)
  {
    return api->coleoptera_y(id);
  }
  int coleoptera_ore(unsigned id)
  {
    return api->coleoptera_ore(id);
  }
  int coleoptera_plasma(unsigned id)
  {
    return api->coleoptera_plasma(id);
  }
  int coleoptera_damage(unsigned id)
  {
    return api->coleoptera_damage(id);
  }
  int coleoptera_state(unsigned id)
  {
    return api->coleoptera_state(id);
  }
  int get_first_coleoptera()
  {
    return api->get_first_coleoptera();
  }
  int get_next_coleoptera(int id)
  {
    return api->get_next_coleoptera(id);
  }
  int factory_y(unsigned id)
  {
    return api->factory_y(id);
  }
  int factory_x(unsigned id)
  {
    return api->factory_x(id);
  }
  int mine_y(unsigned id)
  {
    return api->mine_y(id);
  }
  int mine_x(unsigned id)
  {
    return api->mine_x(id);
  }
  int central_y(unsigned id)
  {
    return api->central_y(id);
  }
  int central_x(unsigned id)
  {
    return api->central_x(id);
  }
  int hotel_y(unsigned id)
  {
    return api->hotel_y(id);
  }
  int hotel_x(unsigned id)
  {
    return api->hotel_x(id);
  }
  int hotel_value(unsigned id)
  {
    return api->hotel_value(id);
  }

  // les actions

  int move_coleoptera(unsigned id, int direction)
  {
    return api->move_coleoptera(id, direction);
  }
  int dock_coleoptera_to_coleoptera(unsigned id1, unsigned id2)
  {
    return api->dock_coleoptera_to_coleoptera(id1, id2);
  }
  int dock_coleoptera_to_building(unsigned id)
  {
    return api->dock_coleoptera_to_building(id);
  }
  int transfert(unsigned id, unsigned ore, unsigned plasma)
  {
    return api->transfert(id, ore, plasma);
  }
  int open_canal(unsigned id)
  {
    return api->open_canal(id);
  }
  int close_canal(unsigned id)
  {
    return api->close_canal(id);
  }
  int throw_plasma(unsigned id, unsigned quantity, unsigned direction)
  {
    return api->throw_plasma(id, quantity, direction);
  }
  int make_building(unsigned id, unsigned type)
  {
    return api->make_building(id, type);
  }
  int make_coleoptera(unsigned id, unsigned count)
  {
    return api->make_coleoptera(id, count);
  }
  int repair_coleoptera_by_coleoptera(int id1, int id2)
  {
    return api->repair_coleoptera_by_coleoptera(id1, id2);
  }
  int repair_coleoptera_by_factory(int id)
  {
    return api->repair_coleoptera_by_factory(id);
  }
  int teams()
  {
    return api->teams();
  }
  int my_team()
  {
    return api->my_team();
  }
};
