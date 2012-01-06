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

#ifndef CONSTANT_HH_
# define CONSTANT_HH_

// Game constants
# define ORE_BUILDING           1000
# define PLASMA_BUILDING        50

# define ORE_COLEOPTERA         1000
# define PLASMA_COLEOPTERA      200

# define MAX_STOCK_COL_PLASMA   100
# define MAX_STOCK_COL_ORE      1000

# define MAX_COLEOPTERE         200
# define MAX_PLAYER             8
# define MAX_FACTORY            100
# define MAX_CENTRAL            100
# define MAX_MINE               100
# define MAX_HOTEL              250

# define TRANSFERT_ORE_SPEED    100
# define TRANSFERT_PLASMA_SPEED 10 // from a central

# define MAX_DAMAGE             100
# define MAKE_BUILDING_TIME     10
# define MAKE_COLEOPTERA_TIME   10

# define EXPLOSION_CASE_1       50
# define EXPLOSION_CASE_2       25

// Constants for return values
# define INVALID_COMMAND        -3
# define NO_UNIT                -2
# define BAD_ARGUMENT           -1
# define SUCCESS                0

// Constants for messages
# define DEC_Y                  0
# define INC_Y                  1
# define DEC_X                  2
# define INC_X                  3

# define UNKNOWN                -1
# define TERRAIN                0
# define WATER                  1
# define ORE                    2
# define PLASMA                 3

# define MINE                   0
# define FACTORY                1
# define CENTRAL                2
# define HOTEL                  3

# define STATE_NORMAL           0
# define STATE_BUILDING         1
# define STATE_DOCKING          2
# define STATE_TRANSFERING      3
# define STATE_CANAL_OPEN       4
# define STATE_REPAIRING        5
# define STATE_DEAD             6


//
// Ids for message type.
//
enum e_com_type {
  INIT_DATA = 0,
  MAP_CONTENT,
  NEW_UNIT,
  MOVE_COLEOPTERA,
  DOCK_TO_COLEOPTERA,
  DOCK_TO_BUILDING,
  TRANSFERT,
  THROW_PLASMA,
  MAKE_BUILDING,
  MAKE_COLEOPTERA,
  BUILDING_FINISHED,
  COLEOPTERA_CHANGE_STATE,
  UPDATE_PLASMA_STOCK,
  UPDATE_ORE_STOCK,
  UPDATE_LAND_VALUE,
  REPAIR_COLEOPTERA,
  REPAIR_COLEOPTERA_BY_FACTORY,
  LAST_MSG // keep it last
};

#endif // !CONSTANT_HH_
