/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2011 Prologin
*/

#ifndef CONSTANT_HH_
# define CONSTANT_HH_

#include <vector>

///
// Taille du terrain
//
# define TAILLE_TERRAIN            50

///
// Nombre de tours avant la fin de la partie
//
# define FIN_PARTIE                3

///
// Nombre de points d'action par tour
//
# define MAX_PA                    3

///
// Taille des tra�n�es de moto
//
# define TAILLE_TRAINEE            900

///
// Longueur maximale de l'allongement
//
# define MAX_ALLONGEMENT           5

///
// �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action
//
typedef enum erreur {
  OK, /* <- aucune erreur n'est survenue */
  ID_INVALIDE, /* <- identifiant invalide */
  POSITION_INVALIDE, /* <- la position sp�cifi�e est invalide */
  PLUS_DE_PA, /* <- vous n'avez pas assez de points d'action */
  BONUS_INVALIDE, /* <- vous n'avez pas ce bonus */
  PAS_A_TOI, /* <- l'unit� n'est pas a vous */
} erreur;


///
// �num�ration repr�sentant les diff�rents types de case
//
typedef enum type_case {
  VIDE, /* <- rien n'est pr�sent sur la case */
  OBSTACLE, /* <- cette case est inaccessible */
  BONUS, /* <- cette case cotient un bonus */
  POINT_CROISEMENT, /* <- point de croisement de tra�n�es */
  SOURCE,
} type_case;


///
// �num�ration repr�sentant les diff�rents types de bonii
//
typedef enum type_bonus {
  PAS_BONUS, /* <- ceci n'est pas un bonus :-) */
  BONUS_CROISEMENT, /* <- bonus permettant de croiser deux tra�n�es de moto sur une case */
  PLUS_LONG, /* <- bonus permettant d'agrandir une tra�n�e de moto */
  PLUS_PA, /* <- bonus permettant d'avoir plus de points d'action */
  BONUS_REGENERATION, /* <- bonus permettant de regenerer une source d'energie */
} type_bonus;


///
// Repr�sente une position sur le terrain du jeu
//
typedef struct position {
  int x;  /* <- coordonn�e en X */
  int y;  /* <- coordonn�e en Y */
} position;


///
// Caracteristiques d'une source d'�nergie
//
typedef struct source_energie {
  int id;  /* <- identifiant de la source d'�nergie */
  position pos;  /* <- position de la source d'�nergie */
  int coef;  /* <- coefficient representant les points d'�nergie que la source va vous apporter */
} source_energie;


///
// Repr�sente une tra�n�e de moto sur le terrain
//
typedef struct trainee_moto {
  int id;  /* <- identifiant de la tra�nee */
  std::vector<position> emplacement;  /* <- position de chaque composant de la tra�n�e de moto */
  int team;  /* <- identifiant de l'�quipe qui poss�de cette tra�n�e de moto */
} trainee_moto;



// Ids for message type.
enum e_com_type {
  NB_SOURCES = 0,
  CONTENU_CASE, // x, y, type case
  SOURCE_CONTENT,
  ACT_DEPLACER,
  ACT_COUPER_TRAINEE_MOTO,
  ACT_FUSIONNER,
  ACT_ENROULER,
  ACT_REGENERER,
  LAST_MSG
};

#endif // !CONSTANT_HH_
