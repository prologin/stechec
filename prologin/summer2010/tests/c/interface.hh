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
// Représente une position sur le terrain du jeu.
//
typedef struct __internal__cxx__position {
  int x; 
  int y; 
} __internal__cxx__position;


///
// les caracteristiques d'un objet
//
typedef struct __internal__cxx__caracteristiques_objet {
  int cout; 
  int porte; 
} __internal__cxx__caracteristiques_objet;


///
// Représente une unité sur le terrain.
//
typedef struct __internal__cxx__unite {
  __internal__cxx__position pos_unite;
  int team; 
  int ko; 
  int pa; 
  type_objet objet; 
  int id; 
} __internal__cxx__unite;


///
// Représente le type d'une piece.
//
typedef struct __internal__cxx__piece {
  int valeur; 
  __internal__cxx__position pos_piece;
  int tour_apparition; 
} __internal__cxx__piece;


///
// Renvoie le numero de votre equipe.
//
extern "C" int api_mon_equipe();

///
// Renvoie le score d'une equipe.
//
extern "C" int api_score(int team);

///
// Renvoie le nombre d'equipes sur la map
//
extern "C" int api_nombre_equipes();

///
// Renvoie le numéro du tour actuel.
//
extern "C" int api_tour_actuel();

///
// Renvoie les pieces qui sont sur la map
//
extern "C" std::vector<__internal__cxx__piece> api_pieces_en_jeu();

///
// Renvoie le 10 prochaines pieces a vennir
//
extern "C" std::vector<__internal__cxx__piece> api_pieces_a_vennir();

///
// Retourne la liste des unités actuellement en jeu.
//
extern "C" std::vector<__internal__cxx__unite> api_unites();

///
// Retourne les caracteristiques de l'objet.
//
extern "C" __internal__cxx__caracteristiques_objet api_proprietes_objet(type_objet to);

///
// Déplace une unité vers une position à portée.
//
extern "C" erreur api_deplacer(__internal__cxx__position cible, __internal__cxx__position pos);

///
// Achete un objet
//
extern "C" erreur api_acheter_objet(__internal__cxx__position cible, type_objet objet);

///
// utilise un objet
//
extern "C" erreur api_utiliser(__internal__cxx__position attaquant, __internal__cxx__position cible);

///
// coupe la partie, cette action coute 100 d'or et met fin a la partie.
//
extern "C" erreur api_achever_la_partie();

///
// Affiche le contenu d'une valeur de type erreur
//
extern "C" void api_afficher_erreur(erreur v);

///
// Affiche le contenu d'une valeur de type type_objet
//
extern "C" void api_afficher_type_objet(type_objet v);

///
// Affiche le contenu d'une valeur de type position
//
extern "C" void api_afficher_position(__internal__cxx__position v);

///
// Affiche le contenu d'une valeur de type caracteristiques_objet
//
extern "C" void api_afficher_caracteristiques_objet(__internal__cxx__caracteristiques_objet v);

///
// Affiche le contenu d'une valeur de type unite
//
extern "C" void api_afficher_unite(__internal__cxx__unite v);

///
// Affiche le contenu d'une valeur de type piece
//
extern "C" void api_afficher_piece(__internal__cxx__piece v);

#endif
