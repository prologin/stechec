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
# define TAILLE_TERRAIN            30

/*!
** Nombre de tours par partie
*/
# define FIN_PARTIE                1500

/*!
** Nombre de points d'action par tour
*/
# define MAX_PA                    3

/*!
** Taille des tra�n�es de moto
*/
# define TAILLE_TRAINEE            120

/*!
** Longueur maximale de l'allongement
*/
# define MAX_ALLONGEMENT           5

/*!
** Nombre de points d'action � rajouter avec bonus
*/
# define AJOUT_PA                  5

/*!
** �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action
*/
typedef enum erreur {
  OK, /* <- aucune erreur n'est survenue */
  ID_INVALIDE, /* <- identifiant invalide */
  POSITION_INVALIDE, /* <- la position sp�cifi�e est invalide */
  PLUS_DE_PA, /* <- vous n'avez pas assez de points d'action */
  BONUS_INVALIDE, /* <- vous n'avez pas ce bonus */
  PAS_A_TOI, /* <- l'unit� n'est pas a vous */
} erreur;


/*!
** �num�ration repr�sentant les diff�rents types de case
*/
typedef enum type_case {
  VIDE, /* <- rien n'est pr�sent sur la case */
  OBSTACLE, /* <- cette case est inaccessible */
  BONUS, /* <- cette case cotient un bonus */
  POINT_CROISEMENT, /* <- point de croisement de tra�n�es */
  SOURCE, /* <- source ou consommateur d'energie */
  TRAINEE, /* <- une train�e de moto */
  TRAINEE_ET_CROISEMENT, /* <- une trainee sur un point de croisement */
} type_case;


/*!
** �num�ration repr�sentant les diff�rents types de bonii
*/
typedef enum type_bonus {
  PAS_BONUS, /* <- ceci n'est pas un bonus :-) */
  BONUS_CROISEMENT, /* <- bonus permettant de croiser deux tra�n�es de moto sur une case */
  PLUS_LONG, /* <- bonus permettant d'agrandir une tra�n�e de moto */
  PLUS_PA, /* <- bonus permettant d'avoir plus de points d'action */
  BONUS_REGENERATION, /* <- bonus permettant de regenerer une source d'energie */
} type_bonus;


/*!
** Repr�sente une position sur le terrain du jeu
*/
typedef struct position {
  int x;  /* <- coordonn�e en X */
  int y;  /* <- coordonn�e en Y */
} position;


/*!
** Caracteristiques d'une source d'�nergie
*/
typedef struct source_energie {
  int id;  /* <- identifiant de la source d'�nergie */
  position pos;  /* <- position de la source d'�nergie */
  int coef;  /* <- coefficient representant les points d'�nergie que la source va vous apporter */
} source_energie;


/*!
** Repr�sente une tra�n�e de moto sur le terrain
*/
typedef struct trainee_moto {
  int id;  /* <- identifiant de la tra�nee */
  size_t emplacement_len;
  position *emplacement_arr;  /* <- position de chaque composant de la tra�n�e de moto */
  int team;  /* <- identifiant de l'�quipe qui poss�de cette tra�n�e de moto */
  int len;  /* <- la taille maximale de la trainee */
} trainee_moto;


/*!
** Retourne le num�ro de votre �quipe
*/
int mon_equipe(void);

/*!
** Retourne les scores de chaque �quipe
*/
void scores(int** ret_arr, size_t* ret_len);

/*!
** Retourne le nombre d'�quipes sur le terrain
*/
int nombre_equipes(void);

/*!
** Retourne le num�ro du tour actuel
*/
int tour_actuel(void);

/*!
** Retourne la liste des sources d'�nergie
*/
void sources_energie(source_energie** ret_arr, size_t* ret_len);

/*!
** Retourne la liste des tra�n�es de moto
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
** Retourne la liste des bonus d'une �quipe
*/
void regarder_bonus(int equipe, type_bonus** ret_arr, size_t* ret_len);

/*!
** Renvoie les points que vous allez gagner a la fin du tour
*/
int diff_score(void);

/*!
** Renvoie le chemin le plus court entre deux points (fonction lente)
*/
void chemin(position p1, position p2, position** ret_arr, size_t* ret_len);

/*!
** D�place une moto
*/
erreur deplacer(int id, position de, position vers);

/*!
** Coupe une tra�n�e de moto en deux nouvelles tra�n�es. � entre � et � et � doivent �tre deux positions adjacentes occup�es par une m�me tra�n�e de moto.
*/
erreur couper_trainee_moto(int id, position entre, position et);

/*!
** Annuler l'action pr�c�dente
*/
erreur cancel(void);

/*!
** Enrouler la tra�n�e de moto en un point
*/
erreur enrouler(int id, position point);

/*!
** R�g�n�re une source d'�nergie � son maximal
*/
erreur regenerer_source_energie(int id);

/*!
** Allonge le tour en rajoutant des points d'action
*/
erreur allonger_pa(void);

/*!
** Allonge une tra�n�e de moto. L'allongement se fera aux prochains d�placements. La longueur du prolongement doit �tre comprise entre 0 et MAX_ALLONGEMENT (inclus).
*/
erreur agrandir_trainee_moto(int id, int longueur);

/*!
** Pose un point de croisement sur une case du terrain. La case doit ne pas d�j� �tre un point de croisement.
*/
erreur poser_point_croisement(position point);

/*!
** Fusionner deux tra�n�es de moto. Les deux doivent appartenir � la m�me �quipe, mais doivent �tre deux tra�n�es distinctes. � pos1 � et � pos2 � doivent �tre adjacentes et occup�es respectivement par � id1 � et � id2 �.
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
** Fonction appell�e au d�but de la partie
*/
void init_game(void);

/*!
** Fonction appell�e pour la phase de jeu
*/
void jouer(void);

/*!
** Fonction appell�e � la fin de la partie
*/
void end_game(void);

