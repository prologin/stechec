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
// �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action.
//
typedef enum erreur {
  OK, /* <- aucune erreur n'est survenue */
  PAS_A_PORTEE, /* <- la cible n'est pas � port�e */
  PLUS_DE_PA, /* <- nombre maximal d'actions par tour d�pass� */
  DEJA_DEPLACE, /* <- cette unit� a d�j� �t� d�plac�e */
  DEJA_ATTAQUE, /* <- cette unit� a d�j� attaqu� */
  DEJA_SPAWN, /* <- une unit� a d�j� �t� spawn � ce tour */
  PAS_SPAWNABLE, /* <- cette unit� n'est pas spawnable */
  SPAWN_OCCUPE, /* <- une unit� est d�j� pr�sente sur le spawn */
  PAS_A_MOI, /* <- l'unit� cibl�e n'est pas � vous */
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
extern "C" erreur api_soin(unite cible);
static inline erreur soin(unite cible)
{
  return api_soin(cible);
}


///
// Utilise une carte � D�guisement � que vous avez dans votre main.
//
extern "C" erreur api_deguisement(unite cible, type_unite nouveau_type);
static inline erreur deguisement(unite cible, type_unite nouveau_type)
{
  return api_deguisement(cible, nouveau_type);
}


///
// Utilise une carte � Banza� � que vous avez dans votre main.
//
extern "C" erreur api_banzai(unite cible);
static inline erreur banzai(unite cible)
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
extern "C" erreur api_deplacer(unite cible, position pos);
static inline erreur deplacer(unite cible, position pos)
{
  return api_deplacer(cible, pos);
}


///
// Attaque une autre unit�.
//
extern "C" erreur api_attaquer(unite attaquant, unite cible);
static inline erreur attaquer(unite attaquant, unite cible)
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
unite retirer_ko();

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
