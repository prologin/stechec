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

# define HORS_TERRAIN              -12
# define PAS_DE_MAISON             -2
# define PAS_DE_MONUMENT           -3
# define FINANCES_DEPASSEES        -4
# define BLOCAGE                   -5
//# define TROP_DE_MAISONS           -6
# define JOUEUR_INCORRECT          -7
# define NON_CONNEXE               -8
# define CASE_OCCUPEE              -9
# define ACTION_INTERDITE          -10
# define TROP_LOIN                 50 // -11
# define SUCCES                    0
# define MAIRIE                    3
# define VIDE                      0
# define MAISON                    1
# define RESERVATION               2
# define MONUMENT                  3
# define ROUTE                     4
# define MAX_MONUMENTS             14
# define TAILLE_CARTE              100
# define INFINI 		   30000

// Keep the following after a ./generator :

#define MAX_TEAMS 3
#define ARGENT_INITIAL 10
#define MAX_ORDERS 1000

// distances greater than MAX_DISTANCE won't be computed. This is only to speed
// up computations.
// Must be < 254
#define MAX_DISTANCE 12

// The maximum number of useful parameters for the orders that are just transmitted
// by the ServerResolver.
#define MAX_PARAM_PER_ORDER 10

// Ids for message type.
enum e_com_type {
  // from server (init)
  INIT_DATA = 0,
  INIT_START_TEAM, /* START_TEAM */
  INIT_MONUMENT, /* MONUMENT_ID, PRESTIGE, REACH */
  // MSG2,
  // from server
  NEXT_MONUMENT, /*MONUMENT_ID*/
  NEW_MONEY, /* TEAM_ID, MONEY */
  WON_AUCTION, /* TEAM_ID */
  NEW_SCORE, /* TEAM_ID, SCORE */
  
  // from client
  FIRST_CLIENT_MSG,
  BUILD_ROAD, /* X, Y, COST, ORDER */
  BUILD_HOUSE, /* X, Y, TEAM_ID, COST, SCORE_INCREASE, ORDER */
  RESERVE_CELL, /* X, Y, TEAM_ID, COST, ORDER */
  DESTROY_HOUSE, /* X, Y, COST, TARGET, SCORE_INCREASE, ORDER */
  SELL_HOUSE, /* X, Y, EARNING, SCORE_INCREASE, ORDER */
  BUILD_MONUMENT, /* X, Y, MONUMENT_ID, ORDER */
  LAST_XY_ORDERS, // dummy message to mark the end of the messages that come
  // from clients and affect an (x,y) position in the map. (x,y) positions are
  // expected to come in first and second position in the order's arguments.
  ENCHERE, /* TEAM_ID, MONTANT */
  SCORE_UPDATE, /* TEAM_ID, SCORE_INCREASE , seed_to_distinguish_msgs for stechec*/	
  // todo : circumvent the fact that identical packets won't be send several times
  LAST_MSG
};

#endif // !CONSTANT_HH_
