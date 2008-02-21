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

#ifndef CONSTANT_HH_
# define CONSTANT_HH_

// Constants candidats :

# define BAD_ARGUMENT              -1

# define TOO_MANY_ORDERS           -2

# define SUCCESS                   0

# define INFINI                    10000

# define NORMAL                    0

# define TRANCHEE                  1

# define OBSTACLE                  2

# define HAUT                      0

# define GAUCHE                    1

# define BAS                       2

# define DROITE                    3

# define ICI                       4

//Constants persos (à conserver lors d'un ./generator .. !):

# define MAP_WALL		'X'
# define MAP_EMPTY		'.'
# define MAP_HOLE		'O'
# define MAP_HOLE_ALTENATE	'0' //only used for input file, no longer used elsewhere !
# define MAP_BALL		'B'
# define MAP_ROBOT_TEAM1	'R'
# define MAP_ROBOT_TEAM2	'r'

# define MAP_MAX_X		20
# define MAP_MAX_Y		20
# define MAX_ROBOTS		6

# define INIT_MAP_SIZE		0

# define INIT_ROBOT		1

# define MAX_ORDERS		3 // max number of orders that can be issued by a single robot

// Ids for message type.
enum e_com_type {
  // from server
  INIT_DATA,        /* depending of the first arg (eg : MAP_WIDTH, MAP_HEIGHT) */
  MAP_CONTENT,      /* X, Y, VALUE */
  BALLS_CONTENT,    /* X, Y, VALUE */
  ROBOT_POS,        /* ROBOT_ID, X, Y */
  ROBOT_HAS_BALL,   /* ROBOT_ID, TRUE or FALSE */
  ROBOT_HOOK,       /* ROBOT_ID, HOOKED_ROBOT_ID */
  ROBOT_UNHOOK,     /* ROBOT_ID */
  // from client
  DROP_BALL,        /* ROBOT_ID, ROBOT_TURN, ORDER_NUMBER, DIRECTION */
  MOVE,             /* ROBOT_ID, ROBOT_TURN, ORDER_NUMBER, DIRECTION */
  PICK_UP_BALL,     /* ROBOT_ID, ROBOT_TURN, ORDER_NUMBER */
  HOOK_ROBOT,       /* ROBOT_ID, ROBOT_TURN, ORDER_NUMBER, DIRECTION */
  RELEASE_HOOK,     /* ROBOT_ID, ROBOT_TURN, ORDER_NUMBER */
  BOOST_TURBO,      /* ROBOT_ID, ROBOT_TURN, ORDER_NUMBER */
  WAIT,		    /* ROBOT_ID, ROBOT_TURN, ORDER_NUMBER */
  LAUNCH_BULLET,    /* ROBOT_ID, ROBOT_TURN, ORDER_NUMBER, DIRECTION */
  // Remark : ROBOT_TURN gives the turn (0,1 or 2) of the robot, among others robot of the same team
  // ORDER_NUMBER give the number of the order (0, 1 or 2). Small robots orders are played first.
  // misc
  LAST_MSG          /* */
};

#endif // !CONSTANT_HH_
