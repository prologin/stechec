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

#ifndef GAMEDATA_HH_
# define GAMEDATA_HH_

# include "Contest.hh"
# include "Constant.hh"

enum building_state_t
  {
    b_normal,
    b_in_construction,
    b_dead
  };

/*****************************************************************************/
/*!
** The superclass for all objects (unities & buildings)
*/
class                Object
{
public:
  unsigned        get_x() const { return x_; }
  unsigned        get_y() const { return y_; }
  unsigned        get_id() const { return id_; }
  unsigned        get_player() const { return player_id_; }
  void                set_x(int x) { x_ = x; }
  void                set_y(int y) { y_ = y; }
  void                set_id(int id) { id_ = id; }
  void                set_player(int player_id) { player_id_ = player_id; }
private:
  unsigned        x_;                /* position x on the map */
  unsigned        y_;                /* position y on the map */
  unsigned        id_;                /* object's identifier */
  unsigned        player_id_;        /* player's identifier */
};

/*****************************************************************************/
/*!
** The superclass for all buildings
*/
class                Building: public Object
{
public:
  int                        type;
  building_state_t        state;
};

/*****************************************************************************/
/*!
** The class for the coleoptere
*/
class                Coleoptere: public Object
{
public:
  Coleoptere();
  void                Reinit();
  int                get_damage() const { return damage_; }
  void                set_damage(int damage)
  {
    damage_ = damage;
    if (damage_ < 0)
      damage = 0;
  }
  void                add_damage(int damage)
  {
    assert(damage > 0);
    damage_ += damage;
    if (damage_ >= MAX_DAMAGE)
      state = STATE_DEAD;
  }
  
  unsigned        stock_ore;        /* stock of ore */
  unsigned        stock_plasma;        /* stock of plasma */
  unsigned        state;                /* coleoptere's state */
  Coleoptere*        dock_col;        /* when docked to another coleoptera */
  Building*        dock_build;        /* when docked to a building */
  unsigned        transfert_ore;        /* stock of ore to be transfered */
  unsigned        build_time;        /* number of remaining turns, to make the building */
  bool                action;
private:
  int                damage_;
};

/*****************************************************************************/
/*!
** The class for the factory
*/
class                Factory: public Building
{
public:
  Factory();
  unsigned      stock_ore;        /* stock of ore */
  unsigned      stock_plasma;        /* stock of plasma */
  unsigned        buildlist;        /* number of turns to finish all the buildings'
                                   list */
  Coleoptere*        col_repair;
};

/*****************************************************************************/
/*!
** The class for the central
*/
class                Central: public Building
{
public:
  Central() {}
};

/*****************************************************************************/
/*!
** The class for the mine
*/
class                Mine: public Building
{
public:
  Mine() {}
};

/*****************************************************************************/
/*!
** The class for the hotel
*/
class                Hotel: public Building
{
public:
  Hotel() {}
};

class                Player
{
public:
  Player();
  unsigned        factory_count;
  unsigned        central_count;
  unsigned        mine_count;
  unsigned        hotel_count;
  Coleoptere        coleopteres[MAX_COLEOPTERE];
  Factory        factories[MAX_FACTORY];
  Central        centrals[MAX_CENTRAL];
  Mine                mines[MAX_MINE];
  Hotel                hotels[MAX_HOTEL];

  int                start_x;
  int                start_y;
private:
};

/*!
** This class is meant to contain all data, accessible from
** everywhere.
**
*/
class GameData: public StechecGameData
{
public:
  GameData();
  ~GameData();

  unsigned        map_size_x;
  unsigned        map_size_y;
  int             maxTurn;

  int**           terrain_value;      // valeur initiale
  uint8_t**       terrain_value_mod;  // % modification
  uint8_t**       terrain_type;
  Coleoptere***   terrain_coleoptere;
  Building***     terrain_building;

  Player*         players;
  Player*         p;

  void InitMap();
};

#endif /* !GAMEDATA_HH_ */
