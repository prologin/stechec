/*!
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_c.rb
*/

#include <stdlib.h>
typedef char * charp;
/*!
** Taille de d�part du terrain.
*/
# define TAILLE_DEPART             19

/*!
** Le nombre maximal d'unit�s pouvant appartenir � une �quipe.
*/
# define NBR_MAX_UNITES            10

/*!
** Le temps, en nombre de tours, entre deux r�tr�cissement du terrain.
*/
# define TEMPS_RETRECISSEMENT      5

/*!
** Le nombre maximum en jeu de chaque carte.
*/
# define MAX_CARTES                2

/*!
** �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action.
*/
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


/*!
** Le type d'une unit� sur le terrain
*/
typedef enum type_unite {
  PERROQUET, /* <- 1 PA et port�e de 1 */
  SINGE, /* <- 2 PA et port�e de 3 */
  CHAT, /* <- 5 PA et port�e de 1 */
  KANGOUROU, /* <- 3 PA et port�e de 2 (explose !) */
} type_unite;


/*!
** Repr�sente une position sur le terrain du jeu.
*/
typedef struct position {
  int x;  /* <- coordonn�e X */
  int y;  /* <- coordonn�e Y */
} position;


/*!
** Contient les informations sur la taille du terrain du jeu.
*/
typedef struct taille_terrain {
  int taille;  /* <- taille actuelle du terrain */
  int min_coord;  /* <- coordonn�e minimale en X ou en Y */
  int max_coord;  /* <- coordonn�e maximale en X ou en Y */
} taille_terrain;


/*!
** Donne les caract�ristiques d'un type d'unit�.
*/
typedef struct caracs {
  int pa_init;  /* <- nombre de points d'actions par tour */
  int portee;  /* <- port�e maximale de l'unit� */
} caracs;


/*!
** Repr�sente une unit� sur le terrain.
*/
typedef struct unite {
  position pos;  /* <- la position de l'unit� */
  int ennemi;  /* <- vrai si l'unit� appartient � l'ennemi */
  type_unite type_unite_actuel;  /* <- le type de l'unit�, qui change si l'unit� est d�guis�e */
  type_unite vrai_type_unite;  /* <- le vrai type de l'unit� (qui ne change pas lors du d�guisement) */
  int ko;  /* <- une valeur n�gative si l'unit� n'est pas KO, sinon le nombre de marqueurs KO sur l'unit� */
  int pa;  /* <- le nombre de PA restants � l'unit� */
  int attaques;  /* <- le nombre d'attaques restants � l'unit� */
  int attaques_gratuites;  /* <- le nombre d'attaques gratuites (voir la partie banzai) */
  int id;  /* <- l'identifiant unique de l'unit� */
} unite;


/*!
** Repr�sente l'ensemble des cartes que vous pouvez utiliser.
*/
typedef struct cartes {
  int potion;  /* <- le nombre de cartes � Potion magique � */
  int deguisement;  /* <- le nombre de cartes � D�guisement � */
  int banzai;  /* <- le nombre de cartes � Banza� � */
  int pacifisme;  /* <- le nombre de cartes � Pacifisme � */
} cartes;


/*!
** Renvoie le nombre de points de commandements.
*/
int nombre_pc(void);

/*!
** Renvoie le nombre d'unit�s en jeu.
*/
int nombre_unites(int ennemi);

/*!
** Renvoie le num�ro du tour actuel.
*/
int tour_actuel(void);

/*!
** Renvoie la position du spawn (ennemi ou non).
*/
position pos_renfort(int ennemi);

/*!
** Renvoie les caract�ristiques d'un type d'unit�.
*/
caracs caracteristiques(type_unite tu);

/*!
** Retourne une structure "cartes" contenant les informations sur les cartes que vous avez en main.
*/
cartes mes_cartes(void);

/*!
** Retourne la liste des unit�s actuellement en jeu.
*/
void unites(unite** ret_arr, size_t* ret_len);

/*!
** Retourne la taille actuelle du terrain et les coordonn�es min/max dans une structure "taille_terrain".
*/
taille_terrain taille_terrain_actuelle(void);

/*!
** Utilise une carte � Potion magique � que vous avez dans votre main.
*/
erreur potion(position cible);

/*!
** Utilise une carte � D�guisement � que vous avez dans votre main.
*/
erreur deguisement(position cible, type_unite nouveau_type);

/*!
** Utilise une carte � Banza� � que vous avez dans votre main.
*/
erreur banzai(position cible);

/*!
** Utilise une carte � Pacifisme � que vous avez dans votre main.
*/
erreur pacifisme(void);

/*!
** D�place une unit� vers une position � port�e.
*/
erreur deplacer(position cible, position pos);

/*!
** Rel�ve une unit� n'ayant plus de marqueurs de KO.
*/
erreur relever(position cible);

/*!
** Attaque une autre unit�.
*/
erreur attaquer(position attaquant, position cible);

/*!
** Fait apparaitre une unit� sur la case de spawn.
*/
erreur renfort(type_unite quoi);

/*!
** Annule l'effet de la derni�re action et remet le jeu dans l'�tat pr�c�dent. Renvoie false s'il n'y a rien � annuler, true sinon.
*/
int annuler(void);

/*!
** Affiche le contenu d'une valeur de type erreur
*/
void afficher_erreur(erreur v);

/*!
** Affiche le contenu d'une valeur de type type_unite
*/
void afficher_type_unite(type_unite v);

/*!
** Affiche le contenu d'une valeur de type position
*/
void afficher_position(position v);

/*!
** Affiche le contenu d'une valeur de type taille_terrain
*/
void afficher_taille_terrain(taille_terrain v);

/*!
** Affiche le contenu d'une valeur de type caracs
*/
void afficher_caracs(caracs v);

/*!
** Affiche le contenu d'une valeur de type unite
*/
void afficher_unite(unite v);

/*!
** Affiche le contenu d'une valeur de type cartes
*/
void afficher_cartes(cartes v);

/*!
** Fonction appell�e au d�but de la partie.
*/
void init_game(void);

/*!
** Fonction appell�e pour la phase de retrait de KO.
*/
position retirer_ko(void);

/*!
** Fonction appell�e pour la phase de jeu.
*/
void jouer(void);

/*!
** Fonction appell�e � la fin de la partie.
*/
void end_game(void);

