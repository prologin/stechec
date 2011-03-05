/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#ifndef CONSTANT_HH_
# define CONSTANT_HH_

/* Constants for our shining static arrays */
# define MAX_TEAM		6
# define MAX_GOODMEN		3
# define MAX_MAP_SIZE           40 ///< For x and y.
# define MAX_DATE		500

/* Ground type */
# define STANDARD		0
# define WALL			1
# define BET_PLACE		2
# define GOODMAN		3

/* For the gui, precise STANDARD and WALL */
# define GRASSY_AREA            10
# define STREET                 11
# define BUILDING               12

/* GoodMan Constants */
# define RAM_SIZE		7

/* Delorean constants */
# define DELOREAN_RELOAD	20
# define DELOREAN_WARP_TIME	4
# define DELOREAN_AUTONOMIE	1000

/* Constants for messages */
# define BAD_ARGUMENT		-4
# define INVALID_COMMAND	-3
# define NO_UNIT		-2
# define UNKNOWN		-1
# define SUCCESS		0

/* Constant for fog of war */
# define MAX_VIEW		5

/* Constant for bets */
# define BET_LENGTH		10

/* Constants for goodmen */
# define STATE_NORMAL			0
# define STATE_BETTING			1
# define STATE_TIME_WARP		2
# define STATE_BACK_IN_THE_FUTURE	3
# define STATE_KO			4

# define KO_TIME			4

# define MAX_BET			10

/* Constants for moving */
# define DEC_Y				0
# define DEC_X				1
# define INC_Y				2
# define INC_X				3

// Ids for message type.
enum e_com_type {
  INIT_DATA = 0,
  NEW_GOODMAN,
  NEW_CASINO,
  NEW_BET,
  MAP_CONTENT,
  MOVE_GOODMAN,
  GOODMAN_CHANGE_STATE,
  GOODMAN_DELOREAN,
  GOODMAN_ALMANACH,
  GOODMAN_MONEY_CHANGE,
  MEMORIZE,
  UPDATE_ROM,
  DELOREAN_MOVE,
  ALMANACH_MOVE,
  CHANGE_DATE,
  BET_TIME,
  DELOREAN,
  DELOREAN_RELOADING,
  DELOREAN_AUTO,
  // ...
  LAST_MSG
};

#endif // !CONSTANT_HH_
