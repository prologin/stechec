
#ifndef PROLOGIN_HH_
# define PROLOGIN_HH_

#include <vector>
#include "Constant.hh"
///
// Renvoie le numéro du tour actuel.
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
// Renvoie les caractéristiques d'un type d'unité.
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
// Retourne la liste des unités actuellement en jeu.
//
extern "C" std::vector<unite> api_unites();
static inline std::vector<unite> unites()
{
  return api_unites();
}


///
// Retourne la taille actuelle du terrain et les coordonnées min/max dans une structure \texttt{taille_terrain}.
//
extern "C" taille_terrain api_taille_terrain_actuelle();
static inline taille_terrain taille_terrain_actuelle()
{
  return api_taille_terrain_actuelle();
}


///
// Utilise une carte « Quoi d'neuf docteur ? » que vous avez dans votre main.
//
extern "C" erreur api_soin(position cible);
static inline erreur soin(position cible)
{
  return api_soin(cible);
}


///
// Utilise une carte « Déguisement » que vous avez dans votre main.
//
extern "C" erreur api_deguisement(position cible, type_unite nouveau_type);
static inline erreur deguisement(position cible, type_unite nouveau_type)
{
  return api_deguisement(cible, nouveau_type);
}


///
// Utilise une carte « Banzaï » que vous avez dans votre main.
//
extern "C" erreur api_banzai(position cible);
static inline erreur banzai(position cible)
{
  return api_banzai(cible);
}


///
// Utilise une carte « Pacifisme » que vous avez dans votre main.
//
extern "C" erreur api_pacifisme();
static inline erreur pacifisme()
{
  return api_pacifisme();
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
// Relève une unité n'ayant plus de tours KO.
//
extern "C" erreur api_relever(position cible);
static inline erreur relever(position cible)
{
  return api_relever(cible);
}


///
// Attaque une autre unité.
//
extern "C" erreur api_attaquer(position attaquant, position cible);
static inline erreur attaquer(position attaquant, position cible)
{
  return api_attaquer(attaquant, cible);
}


///
// Fait apparaitre une unité sur la case de spawn.
//
extern "C" erreur api_spawn(type_unite quoi);
static inline erreur spawn(type_unite quoi)
{
  return api_spawn(quoi);
}


///
// Annule l'effet de la dernière action et remet le jeu dans l'état précédent.
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

#endif /* !PROLOGIN_HH_ */
