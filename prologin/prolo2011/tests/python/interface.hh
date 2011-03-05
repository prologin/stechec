///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_python.rb
//

#ifndef INTERFACE_HH_
# define INTERFACE_HH_

# include <Python.h>
# include <vector>
# include <string>

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
} type_case;


///
// �num�ration repr�sentant les diff�rents types de bonii
//
typedef enum bonus {
  VIDE, /* <- ceci n'est pas un bonus :-) */
  BONUS_CROISEMENT, /* <- bonus permettant de croiser deux tra�n�es de moto sur une case */
  PLUS_LONG, /* <- bonus permettant d'agrandir une tra�n�e de moto */
  PLUS_PA, /* <- bonus permettant d'avoir plus de points d'action */
  BONUS_REGENERATION, /* <- bonus permettant de regenerer une source d'energie */
} bonus;


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



extern "C" {

///
// Retourne le num�ro de votre �quipe
//
int api_mon_equipe();

///
// Retourne les scores de chaque �quipe
//
std::vector<int> api_scores();

///
// Retourne le nombre d'�quipes sur le terrain
//
int api_nombre_equipes();

///
// Retourne le num�ro du tour actuel
//
int api_tour_actuel();

///
// Retourne la liste des sources d'�nergie
//
std::vector<source_energie> api_sources_energie();

///
// Retourne la liste des tra�n�es de moto
//
std::vector<trainee_moto> api_trainees_moto();

///
// Retourne le type d'une case
//
type_case api_regarder_type_case(position pos);

///
// Retourne le type de bonus d'une case
//
bonus api_regarder_type_bonus(position pos);

///
// Retourne la liste des bonus d'une �quipe
//
std::vector<bonus> api_regarder_bonus(int equipe);

///
// D�place une moto
//
erreur api_deplacer(int id, position de, position vers);

///
// Coupe une tra�n�e de moto en deux nouvelles tra�n�es. � entre � et � et � doivent �tre deux positions adjacentes occup�es par une m�me tra�n�e de moto.
//
erreur api_couper_trainee_moto(int id, position entre, position et);

///
// Annuler l'action pr�c�dente
//
erreur api_cancel();

///
// Enrouler la tra�n�e de moto en un point
//
erreur api_enrouler(int id, position point);

///
// R�g�n�re une source d'�nergie � son maximal
//
erreur api_regenerer_source_energie(int id);

///
// Allonge le tour en rajoutant des points d'action
//
erreur api_allonger_pa();

///
// Allonge une tra�n�e de moto. L'allongement se fera aux prochains d�placements. La longueur du prolongement doit �tre comprise entre 0 et MAX_ALLONGEMENT (inclus).
//
erreur api_agrandir_trainee_moto(int id, int longueur);

///
// Pose un point de croisement sur une case du terrain. La case doit ne pas d�j� �tre un point de croisement.
//
erreur api_poser_point_croisement(position point);

///
// Fusionner deux tra�n�es de moto. Les deux doivent appartenir � la m�me �quipe, mais doivent �tre deux tra�n�es distinctes. � pos1 � et � pos2 � doivent �tre adjacentes et occup�es respectivement par � id1 � et � id2 �.
//
erreur api_fusionner(int id1, position pos1, int id2, position pos2);

///
// Affiche le contenu d'une valeur de type erreur
//
void api_afficher_erreur(erreur v);

///
// Affiche le contenu d'une valeur de type type_case
//
void api_afficher_type_case(type_case v);

///
// Affiche le contenu d'une valeur de type bonus
//
void api_afficher_bonus(bonus v);

///
// Affiche le contenu d'une valeur de type position
//
void api_afficher_position(position v);

///
// Affiche le contenu d'une valeur de type source_energie
//
void api_afficher_source_energie(source_energie v);

///
// Affiche le contenu d'une valeur de type trainee_moto
//
void api_afficher_trainee_moto(trainee_moto v);

}

#endif // !INTERFACE_HH_
