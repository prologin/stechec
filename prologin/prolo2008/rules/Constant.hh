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
  INIT_DATA,
  MAP_CONTENT,
  ROBOT_POS,
  ROBOT_HAS_BALL,
  ROBOT_HOOK,
  // from client
  DROP_BALL,
  MOVE,
  PICK_UP_BALL,
  HOOK_ROBOT,
  RELEASE_HOOK,
  BOOST_TURBO,
  // misc
  LAST_MSG
};

#endif // !CONSTANT_HH_
