///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cxx.rb
//

#ifndef PROLOGIN_HH_
# define PROLOGIN_HH_

# include <vector>

# include <string>

///
// Taille du terrain.
//
# define TAILLE_TERRAIN            21

///
// le prix de la fin d'une partie
//
# define PRIX_FIN_PARTIE           150

///
// le nombre de points d'actions des unites au debut du tour.
//
# define MAX_PA                    3

///
// Le nombre d'unités par équipe.
//
# define NBR_UNITES                9

///
// Le nombre maximum de tours.
//
# define MAX_TURN                  100

///
// Le nombre points KO infliges par un coup de marteau
//
# define MARTEAU_KO                10

///
// Le nombre points KO qu'une unite subbit losqu'elle se prend un filet.
//
# define FILET_KO                  4

///
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
//
typedef enum erreur {
  OK, /* <- aucune erreur n'est survenue */
  POSITION_INVALIDE, /* <- la position spécifiée est invalide */
  PLUS_DE_PA, /* <- vous n'avez pas assez de points d'actions */
  PAS_A_PORTE, /* <- vous ne pouvez pas utiliser cet objet la cible n'est pas a porte */
  UNITE_KO, /* <- votre unite est ko */
  PAS_A_TOI, /* <- l'unite n'est pas a toi. */
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
  int cout;  /* <- ce que coute l'objet */
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


///
// Renvoie le numero de votre equipe.
//
extern "C" int api_mon_equipe();
static inline int mon_equipe()
{
  return api_mon_equipe();
}


///
// Renvoie le score d'une equipe.
//
extern "C" int api_score(int team);
static inline int score(int team)
{
  return api_score(team);
}


///
// Renvoie le nombre d'equipes sur la map
//
extern "C" int api_nombre_equipes();
static inline int nombre_equipes()
{
  return api_nombre_equipes();
}


///
// Renvoie le numéro du tour actuel.
//
extern "C" int api_tour_actuel();
static inline int tour_actuel()
{
  return api_tour_actuel();
}


///
// Renvoie les pieces qui sont sur la map
//
extern "C" std::vector<piece> api_pieces_en_jeu();
static inline std::vector<piece> pieces_en_jeu()
{
  return api_pieces_en_jeu();
}


///
// Renvoie le 10 prochaines pieces a vennir
//
extern "C" std::vector<piece> api_pieces_a_vennir();
static inline std::vector<piece> pieces_a_vennir()
{
  return api_pieces_a_vennir();
}


///
// Retourne la liste des unités actuellement en jeu.
//
extern "C" std::vector<unite> api_unites();
static inline std::vector<unite> unites()
{
  return api_unites();
}


///
// Retourne les caracteristiques de l'objet.
//
extern "C" caracteristiques_objet api_proprietes_objet(type_objet to);
static inline caracteristiques_objet proprietes_objet(type_objet to)
{
  return api_proprietes_objet(to);
}


///
// Déplace une unité vers une position à portée.
//
extern "C" erreur api_deplacer(position cible, position pos);
static inline erreur deplacer(position cible, position pos)
{
  return api_deplacer(cible, pos);
}


///
// Achete un objet
//
extern "C" erreur api_acheter_objet(position cible, type_objet objet);
static inline erreur acheter_objet(position cible, type_objet objet)
{
  return api_acheter_objet(cible, objet);
}


///
// utilise un objet
//
extern "C" erreur api_utiliser(position attaquant, position cible);
static inline erreur utiliser(position attaquant, position cible)
{
  return api_utiliser(attaquant, cible);
}


///
// coupe la partie, cette action coute 100 d'or et met fin a la partie.
//
extern "C" erreur api_achever_la_partie();
static inline erreur achever_la_partie()
{
  return api_achever_la_partie();
}


///
// Affiche le contenu d'une valeur de type erreur
//
extern "C" void api_afficher_erreur(erreur v);
static inline void afficher_erreur(erreur v)
{
  api_afficher_erreur(v);
}


///
// Affiche le contenu d'une valeur de type type_objet
//
extern "C" void api_afficher_type_objet(type_objet v);
static inline void afficher_type_objet(type_objet v)
{
  api_afficher_type_objet(v);
}


///
// Affiche le contenu d'une valeur de type position
//
extern "C" void api_afficher_position(position v);
static inline void afficher_position(position v)
{
  api_afficher_position(v);
}


///
// Affiche le contenu d'une valeur de type caracteristiques_objet
//
extern "C" void api_afficher_caracteristiques_objet(caracteristiques_objet v);
static inline void afficher_caracteristiques_objet(caracteristiques_objet v)
{
  api_afficher_caracteristiques_objet(v);
}


///
// Affiche le contenu d'une valeur de type unite
//
extern "C" void api_afficher_unite(unite v);
static inline void afficher_unite(unite v)
{
  api_afficher_unite(v);
}


///
// Affiche le contenu d'une valeur de type piece
//
extern "C" void api_afficher_piece(piece v);
static inline void afficher_piece(piece v)
{
  api_afficher_piece(v);
}



extern "C" {

///
// Fonction appellée au début de la partie.
//
void init_game();

///
// Fonction appellée pour la phase de jeu.
//
void jouer();

///
// Fonction appellée à la fin de la partie.
//
void end_game();

///
// Affiche le contenu d'une valeur de type erreur
//

///
// Affiche le contenu d'une valeur de type type_objet
//

///
// Affiche le contenu d'une valeur de type position
//

///
// Affiche le contenu d'une valeur de type caracteristiques_objet
//

///
// Affiche le contenu d'une valeur de type unite
//

///
// Affiche le contenu d'une valeur de type piece
//

}
#endif
