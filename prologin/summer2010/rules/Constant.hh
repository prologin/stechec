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


# define NB_TEAMS                  2
# define NB_PIECES_SUIVANTES_VISIBLES 50
# define NB_PIECES_SUIVANTES_VISIBLES_PAR_TOUR 5

# define PRIX_FIN_PARTIE 10


# define MAX_PA                  3
///
// Taille du terrain.
//
# define TAILLE_TERRAIN            21

///
// Le nombre d'unités par équipe.
//
# define NBR_UNITES                9

///
// Le nombre maximum de tours.
//
# define MAX_TURN                  100

#define MARTEAU_KO                 10
#define FILET_KO                    4

///
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
//
typedef enum erreur {
  OK, /* <- aucune erreur n'est survenue */
  POSITION_INVALIDE, /* <- la position spécifiée est invalide */
  PLUS_DE_PA, /* <- plus de points d'actions */
  PAS_A_PORTE, /* <- l'unite ne peut pas atteindre la cible */
  UNITE_KO,
  PAS_A_TOI, /* <- l'unite est a l'adversaire */
  UTILISATION_IMPOSSIBLE, /* <- vous ne pouvez pas utiliser cet objet */
  PLUS_D_ARGENT, /* <- vous n'avez pas assez d'argent pour acheter l'objet en question */
} erreur;


///
// Le type d'un objet
//
typedef enum type_objet {
  FILET, /* <- permet d'attraper des pieces ou d'immobiliser des unités */
  MARTEAU, /* <- permet de frapper des unités. */
  RIEN, /* <- l'unite ne porte aucun objet */
} type_objet;


///
// Représente une position sur le terrain du jeu.
//
typedef struct position {
  int x;  /* <- coordonnée X */
  int y;  /* <- coordonnée Y */
} position;


///
// les caracteristiques d'un objet
//
typedef struct caracteristiques_objet {
  int cout;  /* <- ce que coute l'objet */
  int porte;  /* <- la porte de l'objet */
} caracteristiques_objet;


///
// Représente une unité sur le terrain.
//
typedef struct unite {
  position pos_unite;  /* <- la position de l'unité */
  int team;  /* <- donne l'equipe du joueur */
  int ko;  /* <- renvoie le temps que l'unite passera sans pouvoir bouger. */
  int pa;  /* <- le nombre de PA restants à l'unité */
  type_objet objet;  /* <- renvoie l'objet que porte l'unite. */
  int id;  /* <- l'identifiant unique de l'unité */
} unite;


///
// Représente le type d'une piece.
//
typedef struct piece {
  int valeur;  /* <- la valeur de la piece. */
  position pos_piece;  /* <- renvoie la position de la piece */
  int tour_apparition;  /* <- renvoie le tour ou la piece apparaitra ou est apparu. */
} piece;



// Ids for message type.
enum e_com_type {
  DEPLACER_MSG = 0,
  FINIR_MSG,
  ACHETER_OBJET_MSG,
  UTILISER_OBJET_MSG,
  NEXT_PIECE_MSG,
  LAST_MSG
};

#endif // !CONSTANT_HH_
