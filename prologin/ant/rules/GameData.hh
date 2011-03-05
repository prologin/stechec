/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2004, 2005, 2006 Prologin
*/

#ifndef GAMEDATA_HH_
# define GAMEDATA_HH_

//! @file

# include "Contest.hh"
# include "Constant.hh"

/*!
** @ingroup stechec
** @defgroup stechec_ant Ant. Special testing contest.
** @{
*/

/*!
** @name Ant Constants
** Global misc constants.
** @{
*/
# define MAX_UNIT       100     ///< Maximum number of ants per team.
# define START_UNIT_NUM 8       ///< Player starts with this many ants.
# define MAX_FOOD_UNIT  7       ///< Maximum food pack that an ant can carry.
//@}

/*!
** @name Ant Constants
** Content of map.
** @{
*/
# define UNKNOWN        0
# define NOTHING        1
# define OBSTACLE       2
# define ANTHILL        3
# define FOOD           4
//@}

/*!
** @name Ant Constants
** Constant that API functions can return.
** @{
*/
# define SUCCESS          0
# define BAD_ARGUMENT    -1
# define INVALID_COMMAND -2
# define NO_ANT          -1
//@}

/*!
** Represent an ant.
*/
struct                  unit_t
{
  unit_t():
    x(0), y(0), food(0)
  { }
  
  uint32_t        x;       /**< position x de la fourmis                        */
  uint32_t        y;       /**< position y de la fourmis                        */
  uint32_t        food;    /**< nb d'unite de nourriture                        */
};

/*!
** Describe a player.
*/
struct                player_t
{
  player_t():
    id(0), nb_unit(0), amount_of_food(0),
    anthill_x(0), anthill_y(0)
  { }
  
  int           id;                  /**< Son id                              */
  unit_t        all_unit[MAX_UNIT];  /**< toutes ses unites (fourmis)         */
  int           nb_unit;             /**< nombre de fourmis                   */
  int           amount_of_food;      /**< quantite de nourriture possedee     */
  int           anthill_x;
  int           anthill_y;
};


/*!
** @brief This class is meant to contain all data, accessible from
**   everywhere.
*/
class GameData: public StechecGameData
{
public:
  GameData();
  virtual ~GameData();

  //! loaded from map, send to clients, then may vary
  char          **map;
  player_t      *player;        /**< les joueurs */

  // loaded from map and send to clients
  int           map_size_x;
  int           map_size_y;

  // these variables are never sended, they are calculated on both side
  uint8_t       ***player_fog;  // nb de fourmis voyant cette case, 0xFF si champ de vue fourmilliere
  char          **map_ant;        // si une fourmi occupe une case ou non
  bool          ***player_know_map;

  // client current player
  player_t      *p;             // must map &player[uid]

  void InitMap();
  void InitMapMisc();
  void MoveAnt(int player_id, int from_x, int from_y, int to_x, int to_y);

  // FIXME: currently not used.
  virtual const char* GetCommandString(int cst);
};

//! @}

#endif /* !GAMEDATA_HH_ */
