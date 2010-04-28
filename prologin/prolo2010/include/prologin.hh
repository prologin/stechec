///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cxx.rb
//

#ifndef PROLOGIN_HH_
# define PROLOGIN_HH_

# include <vector>

///
// Taille de d�part du terrain.
//
# define TAILLE_DEPART             25

///
// Le nombre maximal d'unit�s pouvant appartenir � une �quipe.
//
# define NBR_MAX_UNITES            20

///
// Le temps, en nombre de tours, entre deux r�tr�cissement du terrain.
//
# define TEMPS_RETRECISSEMENT      5

///
// Le nombre maximum en jeu de chaque carte.
//
# define MAX_CARTES                2

///
// �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action.
//
typedef enum erreur {
  OK, /* <- aucune erreur n'est survenue */
  POSITION_INVALIDE, /* <- la position sp�cifi�e est invalide */
  CASE_OCCUPEE, /* <- la case sur laquelle vous tentez de vous d�placer est occup�e */
  PAS_A_PORTEE, /* <- la cible n'est pas � port�e */
  UNITE_KO, /* <- l'unit� que vous essayez de faire agir ou sur laquelle vous essayez d'agir  est KO */
  UNITE_DEBOUT,
  QUOTA_DEPASSE, /* <- nombre maximal d'unites */
  PLUS_DE_PA, /* <- cette unit� a r�alis� toutes ses actions */
  DEJA_ATTAQUE, /* <- cette unit� a d�j� attaqu� */
  PAS_SPAWNABLE, /* <- cette unit� n'est pas spawnable */
  SPAWN_OCCUPE, /* <- une unit� est d�j� pr�sente sur le spawn */
  PAS_A_MOI, /* <- l'unit� cibl�e n'est pas � vous */
  PLUS_DE_CARTES, /* <- il n'y a plus de cartes du type sp�cifi� dans votre main */
  PHASE_CARTES_TERMINEE, /* <- vous ne pouvez plus poser de cartes car vous avez fait une action */
} erreur;


///
// Le type d'une unit� sur le terrain
//
typedef enum type_unite {
  PERROQUET, /* <- 1 PA et port�e de 1 */
  SINGE, /* <- 2 PA et port�e de 3 */
  CHAT, /* <- 5 PA et port�e de 1 */
  KANGOUROU, /* <- 3 PA et port�e de 2 (explose !) */
} type_unite;


///
// Repr�sente une position sur le terrain du jeu.
//
typedef struct position {
  int x;  /* <- coordonn�e X */
  int y;  /* <- coordonn�e Y */
} position;


///
// Contient les informations sur la taille du terrain du jeu.
//
typedef struct taille_terrain {
  int taille;  /* <- taille actuelle du terrain */
  int min_coord;  /* <- coordonn�e minimale en X ou en Y */
  int max_coord;  /* <- coordonn�e maximale en X ou en Y */
} taille_terrain;


///
// Donne les caract�ristiques d'un type d'unit�.
//
typedef struct caracs {
  int pa;  /* <- nombre de points d'actions par tour */
  int portee;  /* <- port�e maximale de l'unit� */
} caracs;


///
// Repr�sente une unit� sur le terrain.
//
typedef struct unite {
  position pos;  /* <- la position de l'unit� */
  bool ennemi;  /* <- vrai si l'unit� appartient � l'ennemi */
  type_unite type_unite_actuel;  /* <- le type de l'unit�, qui change si l'unit� est d�guis�e */
  type_unite vrai_type_unite;  /* <- le vrai type de l'unit� (qui ne change pas lors du d�guisement */
  int ko;  /* <- une valeur n�gative si l'unit� n'est pas KO, sinon le nombre de marqueurs KO sur l'unit� */
  int pa;  /* <- le nombre de PA restants � l'unit� */
  int attaques;  /* <- le nombre d'attaques restants � l'unit� */
  int attaques_gratuites;
} unite;


///
// Repr�sente l'ensemble des cartes que vous pouvez utiliser.
//
typedef struct cartes {
  int soin;  /* <- le nombre de cartes � Quoi d'neuf docteur ? � */
  int deguisement;  /* <- le nombre de cartes � D�guisement � */
  int banzai;  /* <- le nombre de cartes � Banza� � */
  int pacifisme;  /* <- le nombre de cartes � Pacifisme � */
} cartes;


///
// Renvoie le num�ro du tour actuel.
//
extern "C" int api_tour_actuel();
static inline int tour_actuel()
{
  return api_tour_actuel();
}


///
// Renvoie la position du spawn (ennemi ou non).
//
extern "C" position api_pos_spawn(bool ennemi);
static inline position pos_spawn(bool ennemi)
{
  return api_pos_spawn(ennemi);
}


///
// Renvoie les caract�ristiques d'un type d'unit�.
//
extern "C" caracs api_caracteristiques(type_unite tu);
static inline caracs caracteristiques(type_unite tu)
{
  return api_caracteristiques(tu);
}


///
// Retourne une structure \texttt{cartes} contenant les informations sur les cartes que vous avez en main.
//
extern "C" cartes api_mes_cartes();
static inline cartes mes_cartes()
{
  return api_mes_cartes();
}


///
// Retourne la liste des unit�s actuellement en jeu.
//
extern "C" std::vector<unite> api_unites();
static inline std::vector<unite> unites()
{
  return api_unites();
}


///
// Retourne la taille actuelle du terrain et les coordonn�es min/max dans une structure \texttt{taille_terrain}.
//
extern "C" taille_terrain api_taille_terrain_actuelle();
static inline taille_terrain taille_terrain_actuelle()
{
  return api_taille_terrain_actuelle();
}


///
// Utilise une carte � Quoi d'neuf docteur ? � que vous avez dans votre main.
//
extern "C" erreur api_soin(position cible);
static inline erreur soin(position cible)
{
  return api_soin(cible);
}


///
// Utilise une carte � D�guisement � que vous avez dans votre main.
//
extern "C" erreur api_deguisement(position cible, type_unite nouveau_type);
static inline erreur deguisement(position cible, type_unite nouveau_type)
{
  return api_deguisement(cible, nouveau_type);
}


///
// Utilise une carte � Banza� � que vous avez dans votre main.
//
extern "C" erreur api_banzai(position cible);
static inline erreur banzai(position cible)
{
  return api_banzai(cible);
}


///
// Utilise une carte � Pacifisme � que vous avez dans votre main.
//
extern "C" erreur api_pacifisme();
static inline erreur pacifisme()
{
  return api_pacifisme();
}


///
// D�place une unit� vers une position � port�e.
//
extern "C" erreur api_deplacer(position cible, position pos);
static inline erreur deplacer(position cible, position pos)
{
  return api_deplacer(cible, pos);
}


///
// Rel�ve une unit� n'ayant plus de tours KO.
//
extern "C" erreur api_relever(position cible);
static inline erreur relever(position cible)
{
  return api_relever(cible);
}


///
// Attaque une autre unit�.
//
extern "C" erreur api_attaquer(position attaquant, position cible);
static inline erreur attaquer(position attaquant, position cible)
{
  return api_attaquer(attaquant, cible);
}


///
// Fait apparaitre une unit� sur la case de spawn.
//
extern "C" erreur api_spawn(type_unite quoi);
static inline erreur spawn(type_unite quoi)
{
  return api_spawn(quoi);
}


///
// Annule l'effet de la derni�re action et remet le jeu dans l'�tat pr�c�dent.
//
extern "C" bool api_annuler();
static inline bool annuler()
{
  return api_annuler();
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
// Affiche le contenu d'une valeur de type type_unite
//
extern "C" void api_afficher_type_unite(type_unite v);
static inline void afficher_type_unite(type_unite v)
{
  api_afficher_type_unite(v);
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
// Affiche le contenu d'une valeur de type taille_terrain
//
extern "C" void api_afficher_taille_terrain(taille_terrain v);
static inline void afficher_taille_terrain(taille_terrain v)
{
  api_afficher_taille_terrain(v);
}


///
// Affiche le contenu d'une valeur de type caracs
//
extern "C" void api_afficher_caracs(caracs v);
static inline void afficher_caracs(caracs v)
{
  api_afficher_caracs(v);
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
// Affiche le contenu d'une valeur de type cartes
//
extern "C" void api_afficher_cartes(cartes v);
static inline void afficher_cartes(cartes v)
{
  api_afficher_cartes(v);
}



extern "C" {

///
// Fonction appell�e au d�but de la partie.
//
void init_game();

///
// Fonction appell�e pour la phase de retrait de KO.
//
position retirer_ko();

///
// Fonction appell�e pour la phase de jeu.
//
void jouer();

///
// Fonction appell�e � la fin de la partie.
//
void end_game();

}
#endif
