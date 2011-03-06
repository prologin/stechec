/*!
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_c.rb
*/

#include <stdlib.h>
typedef char * charp;
/*!
** Taille du terrain
*/
# define TAILLE_TERRAIN            100

/*!
** Nombre de tours par partie
*/
# define FIN_PARTIE                1500

/*!
** Nombre de points d'action par tour
*/
# define MAX_PA                    3

/*!
** Taille des traînées de moto
*/
# define TAILLE_TRAINEE            900

/*!
** Longueur maximale de l'allongement
*/
# define MAX_ALLONGEMENT           5

/*!
** Énumération représentant une erreur renvoyée par une des fonctions d'action
*/
typedef enum erreur {
  OK, /* <- aucune erreur n'est survenue */
  ID_INVALIDE, /* <- identifiant invalide */
  POSITION_INVALIDE, /* <- la position spécifiée est invalide */
  PLUS_DE_PA, /* <- vous n'avez pas assez de points d'action */
  BONUS_INVALIDE, /* <- vous n'avez pas ce bonus */
  PAS_A_TOI, /* <- l'unité n'est pas a vous */
} erreur;


/*!
** Énumération représentant les différents types de case
*/
typedef enum type_case {
  VIDE, /* <- rien n'est présent sur la case */
  OBSTACLE, /* <- cette case est inaccessible */
  BONUS, /* <- cette case cotient un bonus */
  POINT_CROISEMENT, /* <- point de croisement de traînées */
} type_case;


/*!
** Énumération représentant les différents types de bonii
*/
typedef enum type_bonus {
  PAS_BONUS, /* <- ceci n'est pas un bonus :-) */
  BONUS_CROISEMENT, /* <- bonus permettant de croiser deux traînées de moto sur une case */
  PLUS_LONG, /* <- bonus permettant d'agrandir une traînée de moto */
  PLUS_PA, /* <- bonus permettant d'avoir plus de points d'action */
  BONUS_REGENERATION, /* <- bonus permettant de regenerer une source d'energie */
} type_bonus;


/*!
** Représente une position sur le terrain du jeu
*/
typedef struct position {
  int x;  /* <- coordonnée en X */
  int y;  /* <- coordonnée en Y */
} position;


/*!
** Caracteristiques d'une source d'énergie
*/
typedef struct source_energie {
  int id;  /* <- identifiant de la source d'énergie */
  position pos;  /* <- position de la source d'énergie */
  int coef;  /* <- coefficient representant les points d'énergie que la source va vous apporter */
} source_energie;


/*!
** Représente une traînée de moto sur le terrain
*/
typedef struct trainee_moto {
  int id;  /* <- identifiant de la traînee */
  size_t emplacement_len;
  position *emplacement_arr;  /* <- position de chaque composant de la traînée de moto */
  int team;  /* <- identifiant de l'équipe qui possède cette traînée de moto */
} trainee_moto;


/*!
** Retourne le numéro de votre équipe
*/
int mon_equipe(void);

/*!
** Retourne les scores de chaque équipe
*/
void scores(int** ret_arr, size_t* ret_len);

/*!
** Retourne le nombre d'équipes sur le terrain
*/
int nombre_equipes(void);

/*!
** Retourne le numéro du tour actuel
*/
int tour_actuel(void);

/*!
** Retourne la liste des sources d'énergie
*/
void sources_energie(source_energie** ret_arr, size_t* ret_len);

/*!
** Retourne la liste des traînées de moto
*/
void trainees_moto(trainee_moto** ret_arr, size_t* ret_len);

/*!
** Retourne le type d'une case
*/
type_case regarder_type_case(position pos);

/*!
** Retourne le type de bonus d'une case
*/
type_bonus regarder_type_bonus(position pos);

/*!
** Retourne la liste des bonus d'une équipe
*/
void regarder_bonus(int equipe, type_bonus** ret_arr, size_t* ret_len);

/*!
** Déplace une moto
*/
erreur deplacer(int id, position de, position vers);

/*!
** Coupe une traînée de moto en deux nouvelles traînées. « entre » et « et » doivent être deux positions adjacentes occupées par une même traînée de moto.
*/
erreur couper_trainee_moto(int id, position entre, position et);

/*!
** Annuler l'action précédente
*/
erreur cancel(void);

/*!
** Enrouler la traînée de moto en un point
*/
erreur enrouler(int id, position point);

/*!
** Régénère une source d'énergie à son maximal
*/
erreur regenerer_source_energie(int id);

/*!
** Allonge le tour en rajoutant des points d'action
*/
erreur allonger_pa(void);

/*!
** Allonge une traînée de moto. L'allongement se fera aux prochains déplacements. La longueur du prolongement doit être comprise entre 0 et MAX_ALLONGEMENT (inclus).
*/
erreur agrandir_trainee_moto(int id, int longueur);

/*!
** Pose un point de croisement sur une case du terrain. La case doit ne pas déjà être un point de croisement.
*/
erreur poser_point_croisement(position point);

/*!
** Fusionner deux traînées de moto. Les deux doivent appartenir à la même équipe, mais doivent être deux traînées distinctes. « pos1 » et « pos2 » doivent être adjacentes et occupées respectivement par « id1 » et « id2 ».
*/
erreur fusionner(int id1, position pos1, int id2, position pos2);

/*!
** Affiche le contenu d'une valeur de type erreur
*/
void afficher_erreur(erreur v);

/*!
** Affiche le contenu d'une valeur de type type_case
*/
void afficher_type_case(type_case v);

/*!
** Affiche le contenu d'une valeur de type type_bonus
*/
void afficher_type_bonus(type_bonus v);

/*!
** Affiche le contenu d'une valeur de type position
*/
void afficher_position(position v);

/*!
** Affiche le contenu d'une valeur de type source_energie
*/
void afficher_source_energie(source_energie v);

/*!
** Affiche le contenu d'une valeur de type trainee_moto
*/
void afficher_trainee_moto(trainee_moto v);

/*!
** Fonction appellée au début de la partie
*/
void init_game(void);

/*!
** Fonction appellée pour la phase de jeu
*/
void jouer(void);

/*!
** Fonction appellée à la fin de la partie
*/
void end_game(void);

/*!
** Affiche le contenu d'une valeur de type erreur
*/

/*!
** Affiche le contenu d'une valeur de type type_case
*/

/*!
** Affiche le contenu d'une valeur de type type_bonus
*/

/*!
** Affiche le contenu d'une valeur de type position
*/

/*!
** Affiche le contenu d'une valeur de type source_energie
*/

/*!
** Affiche le contenu d'une valeur de type trainee_moto
*/

