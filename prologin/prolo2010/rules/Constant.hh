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

///
// Taille de départ du terrain.
//
# define TAILLE_DEPART             25

///
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
//
typedef enum erreur {
  OK, /* <- aucune erreur n'est survenue */
  PAS_UNE_UNITE, /* <- la position ne représente pas une unité */
  PAS_A_PORTEE, /* <- la cible n'est pas à portée */
  PLUS_DE_PA, /* <- nombre maximal d'actions par tour dépassé */
  DEJA_DEPLACE, /* <- cette unité a déjà été déplacée */
  DEJA_ATTAQUE, /* <- cette unité a déjà attaqué */
  DEJA_SPAWN, /* <- une unité a déjà été spawn à ce tour */
  PAS_SPAWNABLE, /* <- cette unité n'est pas spawnable */
  SPAWN_OCCUPE, /* <- une unité est déjà présente sur le spawn */
} erreur;


///
// Représente une position sur le terrain du jeu.
//
typedef struct position {
  int x;  /* <- coordonnée X */
  int y;  /* <- coordonnée Y */
} position;


///
// Contient les informations sur le terrain du jeu.
//
typedef struct infos_terrain {
  int taille;  /* <- taille actuelle du terrain */
  int min_coord;  /* <- coordonnée minimale en X ou en Y */
  int max_coord;  /* <- coordonnée maximale en X ou en Y */
} infos_terrain;


///
// Une structure de test.
//
typedef struct teststruct {
  erreur err;  /* <- une erreur */
  std::vector<erreur> errarr;  /* <- un tableau d'erreurs */
} teststruct;



// Ids for message type.
enum e_com_type {
  // MSG1 = 0,
  // MSG2,
  // ...
  LAST_MSG
};

#endif // !CONSTANT_HH_
