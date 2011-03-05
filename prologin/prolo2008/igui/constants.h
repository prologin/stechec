#ifndef DEF_CONSTANTS_H
# define DEF_CONSTANTS_H


# define IMG_PREFIX	"/opt/share/stechec/prolo2008/graphics/"

# define MENU_WIDTH      0
# define FIELD_MARGIN    10

# define MAX_ROBOTS	6

// max number of orders that can be issued by a single robot
# define MAX_ORDERS	3

#define MAX_TOTAL_ORDERS (MAX_ROBOTS / 2 * MAX_ORDERS)

# define MAX_TURN	1000



/*
 * To be used by the GUI : constants related to the actions of the robot during last turn
 */

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


# ifdef REPLAY
#  undef REPLAY
# endif

#endif
