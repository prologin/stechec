/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2010 Prologin
*/

#include "Api.hh"

#include <iostream>
#include <vector>

// from Api.cc
extern Api* api;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& arr)
{
  os << "[";
  typename std::vector<T>::const_iterator it;
  for (it = arr.begin(); it != arr.end(); ++it)
  {
    if (it != arr.begin())
      os << ", ";
    os << *it;
  }
  os << "]";
  return os;
}

///
// Renvoie le num�ro du tour actuel.
//
extern "C" int api_tour_actuel()
{
  return api->tour_actuel();
}

///
// Renvoie la position du spawn (ennemi ou non).
//
extern "C" position api_pos_spawn(bool ennemi)
{
  return api->pos_spawn(ennemi);
}

///
// Renvoie les caract�ristiques d'un type d'unit�.
//
extern "C" caracs api_caracteristiques(type_unite tu)
{
  return api->caracteristiques(tu);
}

///
// Retourne une structure \texttt{cartes} contenant les informations sur les cartes que vous avez en main.
//
extern "C" cartes api_mes_cartes()
{
  return api->mes_cartes();
}

///
// Retourne la liste des unit�s actuellement en jeu.
//
extern "C" std::vector<unite> api_unites()
{
  return api->unites();
}

///
// Retourne la taille actuelle du terrain et les coordonn�es min/max dans une structure \texttt{taille_terrain}.
//
extern "C" taille_terrain api_taille_terrain_actuelle()
{
  return api->taille_terrain_actuelle();
}

///
// Utilise une carte � Quoi d'neuf docteur ? � que vous avez dans votre main.
//
extern "C" erreur api_soin(position cible)
{
  return api->soin(cible);
}

///
// Utilise une carte � D�guisement � que vous avez dans votre main.
//
extern "C" erreur api_deguisement(position cible, type_unite nouveau_type)
{
  return api->deguisement(cible, nouveau_type);
}

///
// Utilise une carte � Banza� � que vous avez dans votre main.
//
extern "C" erreur api_banzai(position cible)
{
  return api->banzai(cible);
}

///
// Utilise une carte � Pacifisme � que vous avez dans votre main.
//
extern "C" erreur api_pacifisme()
{
  return api->pacifisme();
}

///
// D�place une unit� vers une position � port�e.
//
extern "C" erreur api_deplacer(position cible, position pos)
{
  return api->deplacer(cible, pos);
}

///
// Rel�ve une unit� n'ayant plus de tours KO.
//
extern "C" erreur api_relever(position cible)
{
  return api->relever(cible);
}

///
// Attaque une autre unit�.
//
extern "C" erreur api_attaquer(position attaquant, position cible)
{
  return api->attaquer(attaquant, cible);
}

///
// Fait apparaitre une unit� sur la case de spawn.
//
extern "C" erreur api_spawn(type_unite quoi)
{
  return api->spawn(quoi);
}

///
// Annule l'effet de la derni�re action et remet le jeu dans l'�tat pr�c�dent.
//
extern "C" bool api_annuler()
{
  return api->annuler();
}

///
// Affiche le contenu d'une valeur de type erreur
//
std::ostream& operator<<(std::ostream& os, erreur v)
{
  switch (v) {
  case OK: os << "OK"; break;
  case POSITION_INVALIDE: os << "POSITION_INVALIDE"; break;
  case CASE_OCCUPEE: os << "CASE_OCCUPEE"; break;
  case PAS_A_PORTEE: os << "PAS_A_PORTEE"; break;
  case UNITE_KO: os << "UNITE_KO"; break;
  case QUOTA_DEPASSE: os << "QUOTA_DEPASSE"; break;
  case PLUS_DE_PA: os << "PLUS_DE_PA"; break;
  case DEJA_ATTAQUE: os << "DEJA_ATTAQUE"; break;
  case PAS_SPAWNABLE: os << "PAS_SPAWNABLE"; break;
  case SPAWN_OCCUPE: os << "SPAWN_OCCUPE"; break;
  case PAS_A_MOI: os << "PAS_A_MOI"; break;
  case PLUS_DE_CARTES: os << "PLUS_DE_CARTES"; break;
  case PHASE_CARTES_TERMINEE: os << "PHASE_CARTES_TERMINEE"; break;
  }
  return os;
}
extern "C" void api_afficher_erreur(erreur v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type type_unite
//
std::ostream& operator<<(std::ostream& os, type_unite v)
{
  switch (v) {
  case PERROQUET: os << "PERROQUET"; break;
  case SINGE: os << "SINGE"; break;
  case CHAT: os << "CHAT"; break;
  case KANGOUROU: os << "KANGOUROU"; break;
  }
  return os;
}
extern "C" void api_afficher_type_unite(type_unite v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type position
//
std::ostream& operator<<(std::ostream& os, position v)
{
  os << "{ ";
  os << "x" << "=" << v.x;
  os << ", ";
  os << "y" << "=" << v.y;
  os << " }";
  return os;
}
extern "C" void api_afficher_position(position v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type taille_terrain
//
std::ostream& operator<<(std::ostream& os, taille_terrain v)
{
  os << "{ ";
  os << "taille" << "=" << v.taille;
  os << ", ";
  os << "min_coord" << "=" << v.min_coord;
  os << ", ";
  os << "max_coord" << "=" << v.max_coord;
  os << " }";
  return os;
}
extern "C" void api_afficher_taille_terrain(taille_terrain v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type caracs
//
std::ostream& operator<<(std::ostream& os, caracs v)
{
  os << "{ ";
  os << "pa" << "=" << v.pa;
  os << ", ";
  os << "portee" << "=" << v.portee;
  os << " }";
  return os;
}
extern "C" void api_afficher_caracs(caracs v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type unite
//
std::ostream& operator<<(std::ostream& os, unite v)
{
  os << "{ ";
  os << "pos" << "=" << v.pos;
  os << ", ";
  os << "ennemi" << "=" << v.ennemi;
  os << ", ";
  os << "type_unite_actuel" << "=" << v.type_unite_actuel;
  os << ", ";
  os << "vrai_type_unite" << "=" << v.vrai_type_unite;
  os << ", ";
  os << "ko" << "=" << v.ko;
  os << ", ";
  os << "pa" << "=" << v.pa;
  os << ", ";
  os << "attaques" << "=" << v.attaques;
  os << " }";
  return os;
}
extern "C" void api_afficher_unite(unite v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type cartes
//
std::ostream& operator<<(std::ostream& os, cartes v)
{
  os << "{ ";
  os << "soin" << "=" << v.soin;
  os << ", ";
  os << "deguisement" << "=" << v.deguisement;
  os << ", ";
  os << "banzai" << "=" << v.banzai;
  os << ", ";
  os << "pacifisme" << "=" << v.pacifisme;
  os << " }";
  return os;
}
extern "C" void api_afficher_cartes(cartes v)
{
  std::cerr << v << std::endl;
}
