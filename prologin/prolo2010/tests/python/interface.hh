///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_python.rb
//

#ifndef INTERFACE_HH_
# define INTERFACE_HH_

# include <Python.h>
# include <vector>

///
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
//
typedef enum erreur {
  OK, /* <- aucune erreur n'est survenue */
  POSITION_INVALIDE, /* <- la position spécifiée est invalide */
  CASE_OCCUPEE, /* <- la case sur laquelle vous tentez de vous déplacer est occupée */
  PAS_A_PORTEE, /* <- la cible n'est pas à portée */
  UNITE_KO, /* <- l'unité que vous essayez de faire agir ou sur laquelle vous essayez d'agir  est KO */
  UNITE_DEBOUT, /* <- l'unité que vous essayez de relever est déjà debout */
  QUOTA_DEPASSE, /* <- nombre maximal d'unites, de spawn ou de relevages par tour dépassé */
  PLUS_DE_PA, /* <- cette unité a réalisé toutes ses actions */
  DEJA_ATTAQUE, /* <- cette unité a déjà attaqué */
  UNITE_INTERDITE, /* <- cette unité ne peut pas être amenée en renfort */
  RENFORT_IMPOSSIBLE, /* <- une unité est déjà présente sur le spawn */
  PAS_A_MOI, /* <- l'unité ciblée n'est pas à vous */
  PLUS_DE_CARTES, /* <- il n'y a plus de cartes du type spécifié dans votre main */
  PHASE_CARTES_TERMINEE, /* <- vous ne pouvez plus poser de cartes car vous avez fait une action */
} erreur;


///
// Le type d'une unité sur le terrain
//
typedef enum type_unite {
  PERROQUET, /* <- 1 PA et portée de 1 */
  SINGE, /* <- 2 PA et portée de 3 */
  CHAT, /* <- 5 PA et portée de 1 */
  KANGOUROU, /* <- 3 PA et portée de 2 (explose !) */
} type_unite;


///
// Représente une position sur le terrain du jeu.
//
typedef struct position {
  int x;  /* <- coordonnée X */
  int y;  /* <- coordonnée Y */
} position;


///
// Contient les informations sur la taille du terrain du jeu.
//
typedef struct taille_terrain {
  int taille;  /* <- taille actuelle du terrain */
  int min_coord;  /* <- coordonnée minimale en X ou en Y */
  int max_coord;  /* <- coordonnée maximale en X ou en Y */
} taille_terrain;


///
// Donne les caractéristiques d'un type d'unité.
//
typedef struct caracs {
  int pa_init;  /* <- nombre de points d'actions par tour */
  int portee;  /* <- portée maximale de l'unité */
} caracs;


///
// Représente une unité sur le terrain.
//
typedef struct unite {
  position pos;  /* <- la position de l'unité */
  bool ennemi;  /* <- vrai si l'unité appartient à l'ennemi */
  type_unite type_unite_actuel;  /* <- le type de l'unité, qui change si l'unité est déguisée */
  type_unite vrai_type_unite;  /* <- le vrai type de l'unité (qui ne change pas lors du déguisement) */
  int ko;  /* <- une valeur négative si l'unité n'est pas KO, sinon le nombre de marqueurs KO sur l'unité */
  int pa;  /* <- le nombre de PA restants à l'unité */
  int attaques;  /* <- le nombre d'attaques restants à l'unité */
  int attaques_gratuites;  /* <- le nombre d'attaques gratuites (voir la partie banzai) */
  int id;  /* <- l'identifiant unique de l'unité */
} unite;


///
// Représente l'ensemble des cartes que vous pouvez utiliser.
//
typedef struct cartes {
  int potion;  /* <- le nombre de cartes « Potion magique » */
  int deguisement;  /* <- le nombre de cartes « Déguisement » */
  int banzai;  /* <- le nombre de cartes « Banzaï » */
  int pacifisme;  /* <- le nombre de cartes « Pacifisme » */
} cartes;



extern "C" {

///
// Renvoie le nombre de points de commandements.
//
int api_nombre_pc();

///
// Renvoie le nombre d'unités en jeu.
//
int api_nombre_unites(bool ennemi);

///
// Renvoie le numéro du tour actuel.
//
int api_tour_actuel();

///
// Renvoie la position du spawn (ennemi ou non).
//
position api_pos_renfort(bool ennemi);

///
// Renvoie les caractéristiques d'un type d'unité.
//
caracs api_caracteristiques(type_unite tu);

///
// Retourne une structure "cartes" contenant les informations sur les cartes que vous avez en main.
//
cartes api_mes_cartes();

///
// Retourne la liste des unités actuellement en jeu.
//
std::vector<unite> api_unites();

///
// Retourne la taille actuelle du terrain et les coordonnées min/max dans une structure "taille_terrain".
//
taille_terrain api_taille_terrain_actuelle();

///
// Utilise une carte « Potion magique » que vous avez dans votre main.
//
erreur api_potion(position cible);

///
// Utilise une carte « Déguisement » que vous avez dans votre main.
//
erreur api_deguisement(position cible, type_unite nouveau_type);

///
// Utilise une carte « Banzaï » que vous avez dans votre main.
//
erreur api_banzai(position cible);

///
// Utilise une carte « Pacifisme » que vous avez dans votre main.
//
erreur api_pacifisme();

///
// Déplace une unité vers une position à portée.
//
erreur api_deplacer(position cible, position pos);

///
// Relève une unité n'ayant plus de marqueurs de KO.
//
erreur api_relever(position cible);

///
// Attaque une autre unité.
//
erreur api_attaquer(position attaquant, position cible);

///
// Fait apparaitre une unité sur la case de spawn.
//
erreur api_renfort(type_unite quoi);

///
// Annule l'effet de la dernière action et remet le jeu dans l'état précédent. Renvoie false s'il n'y a rien à annuler, true sinon.
//
bool api_annuler();

///
// Affiche le contenu d'une valeur de type erreur
//
void api_afficher_erreur(erreur v);

///
// Affiche le contenu d'une valeur de type type_unite
//
void api_afficher_type_unite(type_unite v);

///
// Affiche le contenu d'une valeur de type position
//
void api_afficher_position(position v);

///
// Affiche le contenu d'une valeur de type taille_terrain
//
void api_afficher_taille_terrain(taille_terrain v);

///
// Affiche le contenu d'une valeur de type caracs
//
void api_afficher_caracs(caracs v);

///
// Affiche le contenu d'une valeur de type unite
//
void api_afficher_unite(unite v);

///
// Affiche le contenu d'une valeur de type cartes
//
void api_afficher_cartes(cartes v);

}

#endif // !INTERFACE_HH_
