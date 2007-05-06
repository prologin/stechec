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

# define BAD_ARGUMENT              -4

# define INVALID_COMMAND           -3

# define NO_UNIT                   -2

# define UNKNOWN                   -1

# define SUCCESS                   0

# define MAX_TURN                  750

# define MAX_MAP_SIZE              40

// Already defined in prologin/common/Rules.hh
// # define MAX_TEAM                  6

// Use MAX_PLAYER instead, defined in prologin/common/Rules.hh
// MAX_PLAYER = MAX_TEAM * MAX_WHITE_CELL
// # define MAX_WHITE_CELL            1

# define MAX_BACTERY               30

# define MAX_NUTRIENT              17

# define MAX_SKILL                 12

# define NEW_NUTRIENT              10

# define CELL_NUTRIENT             10

# define CELL_MITOSIS_MATERIAL     50

# define VIRUS_MATERIAL            15

# define BACTERY_NUTRIENT          10

# define VIRUS_PRODUCTION_DURATION 10

# define VIRUS_LIFE		   30

# define CELL_LIFE		   50

# define MITOSIS_DURATION          50

# define INFECTION_DURATION        10

# define PHAGOCYTOSIS_DURATION     30

# define VESSEL                    0

# define FLESH                     1

# define WHITE_CELL                2

# define CELL                      3

# define BACTERIA		   4

# define VIRUS                     5

# define CELL_STATE_HEALTHY        0

# define CELL_STATE_INFECTED       1

# define CELL_STATE_BEING_INFECTED 2

# define CELL_STATE_BEING_PHAGOCYTED 3

# define CELL_STATE_DEAD           4

# define STATE_NORMAL              0

# define STATE_PHAGOCYTOSING       1

# define STATE_BEING_PHAGOCYTED    2

# define STATE_DEAD                3

# define DEC_Y                     0

# define DEC_X                     1

# define INC_Y                     2

# define INC_X                     3


// Ids for message type.
enum e_com_type {
  INIT_DATA,
  MAP_CONTENT,
  NEW_VIRUS,
  TRANSMISSION,
  MOVE_LEUCO,
  PHAGOCYTE,
  DROP_ANTIBODY,
  NEW_LEUCO,
  COMPETENCE,
  NEW_CELL,
  MOVE_VIRUS,
  VIRUS_DIE,
  NEW_BACT,
  BACT_DIE,
  CASE_UPDATE,
  CELL_UPDATE,
  LAST_MSG
};

#endif // !CONSTANT_HH_
