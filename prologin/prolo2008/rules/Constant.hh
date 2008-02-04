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

#ifndef CONSTANT_HH_
# define CONSTANT_HH_

# define BAD_ARGUMENT		-1
# define SUCCESS		0

# define MAP_WALL		'X'
# define MAP_EMPTY		'.'
# define MAP_HOLE		'0'
# define MAP_BALL		'B'
# define MAP_ROBOT_TEAM1	'R'
# define MAP_ROBOT_TEAM2	'r'

# define MAP_MAX_X		20
# define MAP_MAX_Y		20
# define MAX_ROBOTS		6

// types for INIT_DATA packets :
# define INIT_MAP_SIZE		0

// Ids for message type.
enum e_com_type {
  // from server
  INIT_DATA,        /* MAP_WIDTH, MAP_HEIGHT, ... */
  MAP_CONTENT,      /* X, Y, VALUE */
  ROBOT_POS,        /* ROBOT_ID, X, Y */
  ROBOT_HAS_BALL,   /* ROBOT_ID, TRUE or FALSE */
  ROBOT_HOOK,       /* ROBOT_ID, HOOKED_ROBOT_ID */
  ROBOT_UNHOOK,     /* ROBOT_ID */
  // from client
  DROP_BALL,        /* ROBOT_ID */
  MOVE,             /* ROBOT_ID, WAY */
  PICK_UP_BALL,     /* ROBOT_ID */
  HOOK_ROBOT,       /* ROBOT_ID, HOOKED_ROBOT_ID */
  RELEASE_HOOK,     /* ROBOT_ID */
  BOOST_TURBO,      /* ROBOT_ID */
  // misc
  LAST_MSG          /* */
};

#endif // !CONSTANT_HH_
