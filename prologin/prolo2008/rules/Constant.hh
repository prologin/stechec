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
# define MAP_BALL		'P'
# define MAP_ROBOT_TEAM1	'H'
# define MAP_ROBOT_TEAM2	'h'

# define MAP_MAX_X		20
# define MAP_MAX_Y		20
# define MAX_ROBOTS		6

// types de messages d'initialisation :
# define INIT_MAP_SIZE		0
# define INIT_ROBOT		1
# define INIT_START_TEAM	2
# define INIT_MAX_TURN		3
// fin msg d'initialisation

# define MAX_ORDERS		3 // max number of orders that can be issued by a single robot

# define MAX_ACTIONS		(MAX_ROBOTS * MAX_ROBOTS * MAX_ROBOTS)

# define NUM_STARTING_BULLETS	2 // at the beginning, the number of bullets a robot has

// Ids for message type.
enum e_com_type {
  // from server :
  INIT_DATA,        /* depending of the first arg (eg : MAP_WIDTH, MAP_HEIGHT) */
  MAP_CONTENT,      /* X, Y, VALUE */
  BALLS_CONTENT,    /* X, Y, VALUE */
  ROBOT_POS,        /* ROBOT_ID, X, Y */
  ROBOT_HAS_BALL,   /* ROBOT_ID, TRUE or FALSE */
  ROBOT_HOOK,       /* ROBOT_ID, HOOKED_ROBOT_ID */
  ROBOT_UNHOOK,     /* ROBOT_ID */
  ACTION_LAST_TURN,  /* INDEX, INTEGER_OF_ACTION */
  //used to broadcast to the clients the successful orders of the previous turn.
  ROBOT_REMAINING_BULLETS, /* ROBOT_ID, VALUE(remaining bullets for that robot) */
  // from client :
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

/*
 * To be used by the GUI : constants related to the actions of the robot during last turn
 */

//some remarks :
// - when a robot A gives a ball to robot B, we have the two actions :
// ACTION_DROP_BALL(id=A, dir)
// ACTION_PICK_UP_BALL(id=B)
// - actions are given in an order so that the state of the field is never inconsistent
// that is, if you follow blindly the action given in order, two robots will never be on the same cell
// - the ACTION_RELEASE_HOOK is only there for hooks released *during* a turn (as a result of a robot move, for example)
// as all the hooks are automatically released after the turn, no action will be issued for that.

#define ACTION_WAIT 0		/* no args */
#define ACTION_DROP_BALL 1      /* DIRECTION */
#define ACTION_MOVE 2		/* DIRECTION */
#define ACTION_PICK_UP_BALL 3	/* no args */
#define ACTION_HOOK_ROBOT 4	/* DIRECTION */
#define ACTION_RELEASE_HOOK 5	/* no args */
#define ACTION_BOOST_TURBO 6	/* no args */
#define ACTION_LAUNCH_BULLET 7	/* DIRECTION */
#define ACTION_SEPARATOR 8	/* separator, no args, the robot's id used is the one for which starts the resolution of its order */
#define ACTION_INIT 9		/* used for internal purposes (initialization of the actions array on the client side) */

#endif // !CONSTANT_HH_
