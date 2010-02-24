/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2010 Prologin
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
  PAS_A_PORTEE, /* <- la cible n'est pas à portée */
  PLUS_DE_PA, /* <- nombre maximal d'actions par tour dépassé */
  DEJA_DEPLACE, /* <- cette unité a déjà été déplacée */
  DEJA_ATTAQUE, /* <- cette unité a déjà attaqué */
  DEJA_SPAWN, /* <- une unité a déjà été spawn à ce tour */
  PAS_SPAWNABLE, /* <- cette unité n'est pas spawnable */
  SPAWN_OCCUPE, /* <- une unité est déjà présente sur le spawn */
  PAS_A_MOI, /* <- l'unité ciblée n'est pas à vous */
  PHASE_CARTES_TERMINEE, /* <- vous ne pouvez plus poser de cartes car vous avez fait une action */
} erreur;


///
// Le type d'une unité sur le terrain
//
typedef enum type_unite {
  PERROQUET, /* <- 1 PA et portée de 1 */
  SINGE, /* <- 2 PA et portée de 3 */
  CHAT, /* <- 5 PA et portée de 1 */
  KANGOUROU, /* <- 3 PA et portée de 2 (explose !) */
} type_unite;


///
// Représente une position sur le terrain du jeu.
//
typedef struct position {
  int x;  /* <- coordonnée X */
  int y;  /* <- coordonnée Y */
} position;


///
// Contient les informations sur la taille du terrain du jeu.
//
typedef struct taille_terrain {
  int taille;  /* <- taille actuelle du terrain */
  int min_coord;  /* <- coordonnée minimale en X ou en Y */
  int max_coord;  /* <- coordonnée maximale en X ou en Y */
} taille_terrain;


///
// Représente une unité sur le terrain.
//
typedef struct unite {
  position pos;  /* <- la position de l'unité */
  bool ennemi;  /* <- vrai si l'unité appartient à l'ennemi */
  type_unite type_unite_actuel;  /* <- le type de l'unité, qui change si l'unité est déguisée */
  type_unite vrai_type_unite;  /* <- le vrai type de l'unité (qui ne change pas lors du déguisement */
  int ko;  /* <- une valeur négative si l'unité n'est pas KO, sinon le nombre de marqueurs KO sur l'unité */
  int pa;  /* <- le nombre de PA restants à l'unité */
  int attaques;  /* <- le nombre d'attaques restants à l'unité */
} unite;


///
// Représente l'ensemble des cartes que vous pouvez utiliser.
//
typedef struct cartes {
  int soin;  /* <- le nombre de cartes « Quoi d'neuf docteur ? » */
  int deguisement;  /* <- le nombre de cartes « Déguisement » */
  int banzai;  /* <- le nombre de cartes « Banzaï » */
  int pacifisme;  /* <- le nombre de cartes « Pacifisme » */
} cartes;



// Ids for message type.
enum e_com_type {
  // MSG1 = 0,
  // MSG2,
  // ...
  LAST_MSG
};

#endif // !CONSTANT_HH_
