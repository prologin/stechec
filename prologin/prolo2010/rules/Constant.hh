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
// Taille de d�part du terrain.
//
# define TAILLE_DEPART             25
//
///
// Le nombre maximal d'unit�s pouvant appartenir � une �quipe.
//
# define NBR_MAX_UNITES            10

///
// Le temps, en nombre de tours, entre deux r�tr�cissement du terrain.
//
# define TEMPS_RETRECISSEMENT      5

# define SPAWN_1_X                 12
# define SPAWN_1_Y                 3
# define SPAWN_2_X                 12
# define SPAWN_2_Y                 21

///
// �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action.
//
typedef enum erreur {
  OK, /* <- aucune erreur n'est survenue */
  POSITION_INVALIDE, /* <- la position sp�cifi�e est invalide */
  CASE_OCCUPEE, /* <- la case sur laquelle vous tentez de vous d�placer est occup�e */
  PAS_A_PORTEE, /* <- la cible n'est pas � port�e */
  UNITE_KO, /* <- l'unit� que vous essayez de faire agir ou sur laquelle vous essayez d'agir  est KO */
  UNITE_DEBOUT, /* <- l'unit� que vous essayez de relever est d�j� debout */
  QUOTA_DEPASSE, /* <- nombre maximal d'unites, de spawn ou de relevages par tour d�pass� */
  PLUS_DE_PA, /* <- cette unit� a r�alis� toutes ses actions */
  DEJA_ATTAQUE, /* <- cette unit� a d�j� attaqu� */
  UNITE_INTERDITE, /* <- cette unit� ne peut pas �tre amen�e en renfort */
  RENFORT_IMPOSSIBLE, /* <- une unit� est d�j� pr�sente sur le spawn */
  PAS_A_MOI, /* <- l'unit� cibl�e n'est pas � vous */
  PLUS_DE_CARTES, /* <- il n'y a plus de cartes du type sp�cifi� dans votre main */
  PHASE_CARTES_TERMINEE, /* <- vous ne pouvez plus poser de cartes car vous avez fait une action */
} erreur;

///
// Le type d'une unit� sur le terrain
//
typedef enum type_unite {
  PERROQUET, /* <- 1 PA et port�e de 1 */
  SINGE, /* <- 2 PA et port�e de 3 */
  CHAT, /* <- 5 PA et port�e de 1 */
  KANGOUROU, /* <- 3 PA et port�e de 2 (explose !) */
} type_unite;


///
// Repr�sente une position sur le terrain du jeu.
//
typedef struct position {
  int x;  /* <- coordonn�e X */
  int y;  /* <- coordonn�e Y */
} position;


///
// Contient les informations sur la taille du terrain du jeu.
//
typedef struct taille_terrain {
  int taille;  /* <- taille actuelle du terrain */
  int min_coord;  /* <- coordonn�e minimale en X ou en Y */
  int max_coord;  /* <- coordonn�e maximale en X ou en Y */
} taille_terrain;

///
// Donne les caract�ristiques d'un type d'unit�.
//
typedef struct caracs {
  int pa_init;  /* <- nombre de points d'actions par tour */
  int portee;  /* <- port�e maximale de l'unit� */
} caracs;


///
// Repr�sente une unit� sur le terrain.
//
typedef struct unite {
  position pos;  /* <- la position de l'unit� */
  bool ennemi;  /* <- vrai si l'unit� appartient � l'ennemi */
  type_unite type_unite_actuel;  /* <- le type de l'unit�, qui change si l'unit� est d�guis�e */
  type_unite vrai_type_unite;  /* <- le vrai type de l'unit� (qui ne change pas lors du d�guisement */
  int ko;  /* <- une valeur n�gative si l'unit� n'est pas KO, sinon le nombre de marqueurs KO sur l'unit� */
  int pa;  /* <- le nombre de PA restants � l'unit� */
  int attaques;  /* <- le nombre d'attaques restants � l'unit� */
  int attaques_gratuites;
} unite;

///
// Repr�sente l'ensemble des cartes que vous pouvez utiliser.
//
typedef struct cartes {
  int potion;  /* <- le nombre de cartes � Potion magique � */
  int deguisement;  /* <- le nombre de cartes � D�guisement � */
  int banzai;  /* <- le nombre de cartes � Banza� � */
  int pacifisme;  /* <- le nombre de cartes � Pacifisme � */
} cartes;


// Ids for message type.
enum e_com_type {
  ACT_DEGUISEMENT = 0,
  ACT_BANZAI,
  ACT_SOIN,
  ACT_PACIFISME,
  ACT_DEPLACER,
  ACT_ATTAQUER,
  ACT_SPAWN,
  ACT_RELEVER,

  RETRECIR,

  LAST_MSG
};

#endif // !CONSTANT_HH_
