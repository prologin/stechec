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

///
// Repr�sente une position sur le terrain du jeu.
//
typedef struct __internal__cxx__position {
  int x; 
  int y; 
} __internal__cxx__position;


///
// Contient les informations sur la taille du terrain du jeu.
//
typedef struct __internal__cxx__taille_terrain {
  int taille; 
  int min_coord; 
  int max_coord; 
} __internal__cxx__taille_terrain;


///
// Donne les caract�ristiques d'un type d'unit�.
//
typedef struct __internal__cxx__caracs {
  int pa_init; 
  int portee; 
} __internal__cxx__caracs;


///
// Repr�sente une unit� sur le terrain.
//
typedef struct __internal__cxx__unite {
  __internal__cxx__position pos;
  bool ennemi; 
  type_unite type_unite_actuel; 
  type_unite vrai_type_unite; 
  int ko; 
  int pa; 
  int attaques; 
  int attaques_gratuites; 
  int id; 
} __internal__cxx__unite;


///
// Repr�sente l'ensemble des cartes que vous pouvez utiliser.
//
typedef struct __internal__cxx__cartes {
  int potion; 
  int deguisement; 
  int banzai; 
  int pacifisme; 
} __internal__cxx__cartes;


///
// Renvoie le nombre de points de commandements.
//
extern "C" int api_nombre_pc();

///
// Renvoie le nombre d'unit�s en jeu.
//
extern "C" int api_nombre_unites(bool ennemi);

///
// Renvoie le num�ro du tour actuel.
//
extern "C" int api_tour_actuel();

///
// Renvoie la position du spawn (ennemi ou non).
//
extern "C" __internal__cxx__position api_pos_renfort(bool ennemi);

///
// Renvoie les caract�ristiques d'un type d'unit�.
//
extern "C" __internal__cxx__caracs api_caracteristiques(type_unite tu);

///
// Retourne une structure "cartes" contenant les informations sur les cartes que vous avez en main.
//
extern "C" __internal__cxx__cartes api_mes_cartes();

///
// Retourne la liste des unit�s actuellement en jeu.
//
extern "C" std::vector<__internal__cxx__unite> api_unites();

///
// Retourne la taille actuelle du terrain et les coordonn�es min/max dans une structure "taille_terrain".
//
extern "C" __internal__cxx__taille_terrain api_taille_terrain_actuelle();

///
// Utilise une carte � Potion magique � que vous avez dans votre main.
//
extern "C" erreur api_potion(__internal__cxx__position cible);

///
// Utilise une carte � D�guisement � que vous avez dans votre main.
//
extern "C" erreur api_deguisement(__internal__cxx__position cible, type_unite nouveau_type);

///
// Utilise une carte � Banza� � que vous avez dans votre main.
//
extern "C" erreur api_banzai(__internal__cxx__position cible);

///
// Utilise une carte � Pacifisme � que vous avez dans votre main.
//
extern "C" erreur api_pacifisme();

///
// D�place une unit� vers une position � port�e.
//
extern "C" erreur api_deplacer(__internal__cxx__position cible, __internal__cxx__position pos);

///
// Rel�ve une unit� n'ayant plus de marqueurs de KO.
//
extern "C" erreur api_relever(__internal__cxx__position cible);

///
// Attaque une autre unit�.
//
extern "C" erreur api_attaquer(__internal__cxx__position attaquant, __internal__cxx__position cible);

///
// Fait apparaitre une unit� sur la case de spawn.
//
extern "C" erreur api_renfort(type_unite quoi);

///
// Annule l'effet de la derni�re action et remet le jeu dans l'�tat pr�c�dent. Renvoie false s'il n'y a rien � annuler, true sinon.
//
extern "C" bool api_annuler();

///
// Affiche le contenu d'une valeur de type erreur
//
extern "C" void api_afficher_erreur(erreur v);

///
// Affiche le contenu d'une valeur de type type_unite
//
extern "C" void api_afficher_type_unite(type_unite v);

///
// Affiche le contenu d'une valeur de type position
//
extern "C" void api_afficher_position(__internal__cxx__position v);

///
// Affiche le contenu d'une valeur de type taille_terrain
//
extern "C" void api_afficher_taille_terrain(__internal__cxx__taille_terrain v);

///
// Affiche le contenu d'une valeur de type caracs
//
extern "C" void api_afficher_caracs(__internal__cxx__caracs v);

///
// Affiche le contenu d'une valeur de type unite
//
extern "C" void api_afficher_unite(__internal__cxx__unite v);

///
// Affiche le contenu d'une valeur de type cartes
//
extern "C" void api_afficher_cartes(__internal__cxx__cartes v);

#endif
