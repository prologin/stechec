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
// Taille de d�part du terrain.
//
# define TAILLE_DEPART             25

///
// �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action.
//
typedef enum erreur {
  OK, /* <- aucune erreur n'est survenue */
  PAS_UNE_UNITE, /* <- la position ne repr�sente pas une unit� */
  PAS_A_PORTEE, /* <- la cible n'est pas � port�e */
  PLUS_DE_PA, /* <- nombre maximal d'actions par tour d�pass� */
  DEJA_DEPLACE, /* <- cette unit� a d�j� �t� d�plac�e */
  DEJA_ATTAQUE, /* <- cette unit� a d�j� attaqu� */
  DEJA_SPAWN, /* <- une unit� a d�j� �t� spawn � ce tour */
  PAS_SPAWNABLE, /* <- cette unit� n'est pas spawnable */
  SPAWN_OCCUPE, /* <- une unit� est d�j� pr�sente sur le spawn */
} erreur;


///
// Repr�sente une position sur le terrain du jeu.
//
typedef struct position {
  int x;  /* <- coordonn�e X */
  int y;  /* <- coordonn�e Y */
} position;


///
// Contient les informations sur le terrain du jeu.
//
typedef struct infos_terrain {
  int taille;  /* <- taille actuelle du terrain */
  int min_coord;  /* <- coordonn�e minimale en X ou en Y */
  int max_coord;  /* <- coordonn�e maximale en X ou en Y */
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
