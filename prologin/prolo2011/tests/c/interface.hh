///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_c.rb
//

#ifndef INTERFACE_HH_
# define INTERFACE_HH_

# include <vector>
# include <string>

extern "C" {
# include "prologin.h"
}

typedef std::string std_string;
///
// Représente une position sur le terrain du jeu
//
typedef struct __internal__cxx__position {
  int x; 
  int y; 
} __internal__cxx__position;


///
// Caracteristiques d'une source d'énergie
//
typedef struct __internal__cxx__source_energie {
  int id; 
  __internal__cxx__position pos;
  int coef; 
} __internal__cxx__source_energie;


///
// Représente une traînée de moto sur le terrain
//
typedef struct __internal__cxx__trainee_moto {
  int id; 
  std::vector<__internal__cxx__position> emplacement;
  int team; 
  int len; 
} __internal__cxx__trainee_moto;


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
extern "C" std::vector<__internal__cxx__source_energie> api_sources_energie();

///
// Retourne la liste des traînées de moto
//
extern "C" std::vector<__internal__cxx__trainee_moto> api_trainees_moto();

///
// Retourne le type d'une case
//
extern "C" type_case api_regarder_type_case(__internal__cxx__position pos);

///
// Retourne le type de bonus d'une case
//
extern "C" type_bonus api_regarder_type_bonus(__internal__cxx__position pos);

///
// Retourne la liste des bonus d'une équipe
//
extern "C" std::vector<type_bonus> api_regarder_bonus(int equipe);

///
// Renvoie les points que vous allez gagner a la fin du tour
//
extern "C" int api_diff_score();

///
// Renvoie le chemin le plus court entre deux points (fonction lente)
//
extern "C" std::vector<__internal__cxx__position> api_chemin(__internal__cxx__position p1, __internal__cxx__position p2);

///
// Déplace une moto
//
extern "C" erreur api_deplacer(int id, __internal__cxx__position de, __internal__cxx__position vers);

///
// Coupe une traînée de moto en deux nouvelles traînées. « entre » et « et » doivent être deux positions adjacentes occupées par une même traînée de moto.
//
extern "C" erreur api_couper_trainee_moto(int id, __internal__cxx__position entre, __internal__cxx__position et);

///
// Annuler l'action précédente
//
extern "C" erreur api_cancel();

///
// Enrouler la traînée de moto en un point
//
extern "C" erreur api_enrouler(int id, __internal__cxx__position point);

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
extern "C" erreur api_poser_point_croisement(__internal__cxx__position point);

///
// Fusionner deux traînées de moto. Les deux doivent appartenir à la même équipe, mais doivent être deux traînées distinctes. « pos1 » et « pos2 » doivent être adjacentes et occupées respectivement par « id1 » et « id2 ».
//
extern "C" erreur api_fusionner(int id1, __internal__cxx__position pos1, int id2, __internal__cxx__position pos2);

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
extern "C" void api_afficher_position(__internal__cxx__position v);

///
// Affiche le contenu d'une valeur de type source_energie
//
extern "C" void api_afficher_source_energie(__internal__cxx__source_energie v);

///
// Affiche le contenu d'une valeur de type trainee_moto
//
extern "C" void api_afficher_trainee_moto(__internal__cxx__trainee_moto v);

#endif
