///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_php.rb
//

#ifndef INTERFACE_HH_
# define INTERFACE_HH_

# include <cstdlib>
# include <sapi/embed/php_embed.h>
# include <vector>
# include <string>


///
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
//
typedef enum erreur {
  OK, /* <- aucune erreur n'est survenue */
  POSITION_INVALIDE, /* <- la position spécifiée est invalide */
  UTILISATION_IMPOSSIBLE, /* <- vous ne pouvez pas utiliser cet objet */
  PLUS_D_ARGENT, /* <- vous n'avez pas assez d'argent pour acheter l'objet en question */
} erreur;


///
// Le type d'un objet
//
typedef enum type_objet {
  FILET, /* <- permet d'attraper des pieces ou d'immobiliser des unités */
  MARTEAU, /* <- permet de frapper des unités. */
  RIEN, /* <- l'unite ne porte aucun objet */
} type_objet;


///
// Représente une position sur le terrain du jeu.
//
typedef struct position {
  int x;  /* <- coordonnée X */
  int y;  /* <- coordonnée Y */
} position;


///
// les caracteristiques d'un objet
//
typedef struct caracteristiques_objet {
  int coute;  /* <- ce que coute l'objet */
  int porte;  /* <- la porte de l'objet */
} caracteristiques_objet;


///
// Représente une unité sur le terrain.
//
typedef struct unite {
  position pos_unite;  /* <- la position de l'unité */
  int team;  /* <- donne l'equipe du joueur */
  int ko;  /* <- renvoie le temps que l'unite passera sans pouvoir bouger. */
  int pa;  /* <- le nombre de PA restants à l'unité */
  type_objet objet;  /* <- renvoie l'objet que porte l'unite. */
  int id;  /* <- l'identifiant unique de l'unité */
} unite;


///
// Représente le type d'une piece.
//
typedef struct piece {
  int valeur;  /* <- la valeur de la piece. */
  position pos_piece;  /* <- renvoie la position de la piece */
  int tour_apparition;  /* <- renvoie le tour ou la piece apparaitra ou est apparu. */
} piece;



extern "C" {

///
// Renvoie le numero de votre equipe.
//
int api_mon_equipe();

///
// Renvoie le score d'une equipe.
//
int api_score(int team);

///
// Renvoie le nombre d'equipes sur la map
//
int api_nombre_equipes();

///
// Renvoie le numéro du tour actuel.
//
int api_tour_actuel();

///
// Renvoie les pieces qui sont sur la map
//
std::vector<piece> api_pieces_en_jeu();

///
// Renvoie le 10 prochaines pieces a vennir
//
std::vector<piece> api_pieces_a_vennir();

///
// Retourne la liste des unités actuellement en jeu.
//
std::vector<unite> api_unites();

///
// Déplace une unité vers une position à portée.
//
erreur api_deplacer(position cible, position pos);

///
// Achete un objet
//
erreur api_acheter_objet(position cible, type_objet objet);

///
// utilise un objet
//
erreur api_utiliser(position attaquant, position cible);

///
// coupe la partie, cette action coute 100 d'or et met fin a la partie.
//
erreur api_achever_la_partie();

///
// Affiche le contenu d'une valeur de type erreur
//
void api_afficher_erreur(erreur v);

///
// Affiche le contenu d'une valeur de type type_objet
//
void api_afficher_type_objet(type_objet v);

///
// Affiche le contenu d'une valeur de type position
//
void api_afficher_position(position v);

///
// Affiche le contenu d'une valeur de type caracteristiques_objet
//
void api_afficher_caracteristiques_objet(caracteristiques_objet v);

///
// Affiche le contenu d'une valeur de type unite
//
void api_afficher_unite(unite v);

///
// Affiche le contenu d'une valeur de type piece
//
void api_afficher_piece(piece v);

}

#endif // !INTERFACE_HH_
