///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_caml.rb
//

#include <vector>
#include <string>
///
// Énumération représentant une erreur renvoyée par une des fonctions d'action
//
typedef enum erreur {
  OK, /* <- aucune erreur n'est survenue */
  ID_INVALIDE, /* <- identifiant invalide */
  POSITION_INVALIDE, /* <- la position spécifiée est invalide */
  PLUS_DE_PA, /* <- vous n'avez pas assez de points d'action */
  BONUS_INVALIDE, /* <- vous n'avez pas ce bonus */
  PAS_A_TOI, /* <- l'unité n'est pas a vous */
} erreur;


///
// Énumération représentant les différents types de case
//
typedef enum type_case {
  VIDE, /* <- rien n'est présent sur la case */
  OBSTACLE, /* <- cette case est inaccessible */
  BONUS, /* <- cette case cotient un bonus */
  POINT_CROISEMENT, /* <- point de croisement de traînées */
} type_case;


///
// Énumération représentant les différents types de bonii
//
typedef enum type_bonus {
  PAS_BONUS, /* <- ceci n'est pas un bonus :-) */
  BONUS_CROISEMENT, /* <- bonus permettant de croiser deux traînées de moto sur une case */
  PLUS_LONG, /* <- bonus permettant d'agrandir une traînée de moto */
  PLUS_PA, /* <- bonus permettant d'avoir plus de points d'action */
  BONUS_REGENERATION, /* <- bonus permettant de regenerer une source d'energie */
} type_bonus;


///
// Représente une position sur le terrain du jeu
//
typedef struct position {
  int x;  /* <- coordonnée en X */
  int y;  /* <- coordonnée en Y */
} position;


///
// Caracteristiques d'une source d'énergie
//
typedef struct source_energie {
  int id;  /* <- identifiant de la source d'énergie */
  position pos;  /* <- position de la source d'énergie */
  int coef;  /* <- coefficient representant les points d'énergie que la source va vous apporter */
} source_energie;


///
// Représente une traînée de moto sur le terrain
//
typedef struct trainee_moto {
  int id;  /* <- identifiant de la traînee */
  std::vector<position> emplacement;  /* <- position de chaque composant de la traînée de moto */
  int team;  /* <- identifiant de l'équipe qui possède cette traînée de moto */
} trainee_moto;


///
// Retourne le numéro de votre équipe
//
extern "C" int api_mon_equipe();

///
// Retourne les scores de chaque équipe
//
extern "C" std::vector<int> api_scores();

///
// Retourne le nombre d'équipes sur le terrain
//
extern "C" int api_nombre_equipes();

///
// Retourne le numéro du tour actuel
//
extern "C" int api_tour_actuel();

///
// Retourne la liste des sources d'énergie
//
extern "C" std::vector<source_energie> api_sources_energie();

///
// Retourne la liste des traînées de moto
//
extern "C" std::vector<trainee_moto> api_trainees_moto();

///
// Retourne le type d'une case
//
extern "C" type_case api_regarder_type_case(position pos);

///
// Retourne le type de bonus d'une case
//
extern "C" type_bonus api_regarder_type_bonus(position pos);

///
// Retourne la liste des bonus d'une équipe
//
extern "C" std::vector<type_bonus> api_regarder_bonus(int equipe);

///
// Déplace une moto
//
extern "C" erreur api_deplacer(int id, position de, position vers);

///
// Coupe une traînée de moto en deux nouvelles traînées. « entre » et « et » doivent être deux positions adjacentes occupées par une même traînée de moto.
//
extern "C" erreur api_couper_trainee_moto(int id, position entre, position et);

///
// Annuler l'action précédente
//
extern "C" erreur api_cancel();

///
// Enrouler la traînée de moto en un point
//
extern "C" erreur api_enrouler(int id, position point);

///
// Régénère une source d'énergie à son maximal
//
extern "C" erreur api_regenerer_source_energie(int id);

///
// Allonge le tour en rajoutant des points d'action
//
extern "C" erreur api_allonger_pa();

///
// Allonge une traînée de moto. L'allongement se fera aux prochains déplacements. La longueur du prolongement doit être comprise entre 0 et MAX_ALLONGEMENT (inclus).
//
extern "C" erreur api_agrandir_trainee_moto(int id, int longueur);

///
// Pose un point de croisement sur une case du terrain. La case doit ne pas déjà être un point de croisement.
//
extern "C" erreur api_poser_point_croisement(position point);

///
// Fusionner deux traînées de moto. Les deux doivent appartenir à la même équipe, mais doivent être deux traînées distinctes. « pos1 » et « pos2 » doivent être adjacentes et occupées respectivement par « id1 » et « id2 ».
//
extern "C" erreur api_fusionner(int id1, position pos1, int id2, position pos2);

///
// Affiche le contenu d'une valeur de type erreur
//
extern "C" void api_afficher_erreur(erreur v);

///
// Affiche le contenu d'une valeur de type type_case
//
extern "C" void api_afficher_type_case(type_case v);

///
// Affiche le contenu d'une valeur de type type_bonus
//
extern "C" void api_afficher_type_bonus(type_bonus v);

///
// Affiche le contenu d'une valeur de type position
//
extern "C" void api_afficher_position(position v);

///
// Affiche le contenu d'une valeur de type source_energie
//
extern "C" void api_afficher_source_energie(source_energie v);

///
// Affiche le contenu d'une valeur de type trainee_moto
//
extern "C" void api_afficher_trainee_moto(trainee_moto v);

///
// Fonction appellée au début de la partie
//
extern "C" void init_game();

///
// Fonction appellée pour la phase de jeu
//
extern "C" void jouer();

///
// Fonction appellée à la fin de la partie
//
extern "C" void end_game();

///
// Affiche le contenu d'une valeur de type erreur
//

///
// Affiche le contenu d'une valeur de type type_case
//

///
// Affiche le contenu d'une valeur de type type_bonus
//

///
// Affiche le contenu d'une valeur de type position
//

///
// Affiche le contenu d'une valeur de type source_energie
//

///
// Affiche le contenu d'une valeur de type trainee_moto
//

