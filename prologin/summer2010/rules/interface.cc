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
// Renvoie le numero de votre equipe.
//
extern "C" int api_mon_equipe()
{
  return api->mon_equipe();
}

///
// Renvoie le nombre d'equipes sur la map
//
extern "C" int api_nombre_equipes()
{
  return api->nombre_equipes();
}

///
// Renvoie le numéro du tour actuel.
//
extern "C" int api_tour_actuel()
{
  return api->tour_actuel();
}

///
// Renvoie les pieces qui sont sur la map
//
extern "C" std::vector<piece> api_pieces_en_jeu()
{
  return api->pieces_en_jeu();
}

///
// Renvoie le 10 prochaines pieces a vennir
//
extern "C" std::vector<piece> api_pieces_a_vennir()
{
  return api->pieces_a_vennir();
}

///
// Retourne la liste des unités actuellement en jeu.
//
extern "C" std::vector<unite> api_unites()
{
  return api->unites();
}

///
// Déplace une unité vers une position à portée.
//
extern "C" erreur api_deplacer(position cible, position pos)
{
  return api->deplacer(cible, pos);
}

///
// Achete un objet
//
extern "C" erreur api_acheter_objet(position cible, type_objet objet)
{
  return api->acheter_objet(cible, objet);
}

///
// Achete un objet
//
extern "C" int api_score(int team)
{
  return api->score(team);
}

///
// utilise un objet
//
extern "C" erreur api_utiliser(position attaquant, position cible)
{
  return api->utiliser(attaquant, cible);
}

///
// coupe la partie, cette action coute 100 d'or et met fin a la partie.
//
extern "C" erreur api_achever_la_partie()
{
  return api->achever_la_partie();
}

///
// Affiche le contenu d'une valeur de type erreur
//
std::ostream& operator<<(std::ostream& os, erreur v)
{
  switch (v) {
  case OK: os << "OK"; break;
  case PLUS_DE_PA: os << "PLUS_DE_PA"; break;
  case PAS_A_TOI: os << "PAS_A_TOI"; break;
  case POSITION_INVALIDE: os << "POSITION_INVALIDE"; break;
  case UTILISATION_IMPOSSIBLE: os << "UTILISATION_IMPOSSIBLE"; break;
  case PLUS_D_ARGENT: os << "PLUS_D_ARGENT"; break;
  }
  return os;
}
extern "C" void api_afficher_erreur(erreur v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type type_objet
//
std::ostream& operator<<(std::ostream& os, type_objet v)
{
  switch (v) {
  case FILET: os << "FILET"; break;
  case MARTEAU: os << "MARTEAU"; break;
  case RIEN: os << "RIEN"; break;
  }
  return os;
}
extern "C" void api_afficher_type_objet(type_objet v)
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
// Affiche le contenu d'une valeur de type caracteristiques_objet
//
std::ostream& operator<<(std::ostream& os, caracteristiques_objet v)
{
  os << "{ ";
  os << "cout" << "=" << v.cout;
  os << ", ";
  os << "porte" << "=" << v.porte;
  os << " }";
  return os;
}
extern "C" void api_afficher_caracteristiques_objet(caracteristiques_objet v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type unite
//
std::ostream& operator<<(std::ostream& os, unite v)
{
  os << "{ ";
  os << "pos_unite" << "=" << v.pos_unite;
  os << ", ";
  os << "team" << "=" << v.team;
  os << ", ";
  os << "ko" << "=" << v.ko;
  os << ", ";
  os << "pa" << "=" << v.pa;
  os << ", ";
  os << "objet" << "=" << v.objet;
  os << ", ";
  os << "id" << "=" << v.id;
  os << " }";
  return os;
}
extern "C" void api_afficher_unite(unite v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type piece
//
std::ostream& operator<<(std::ostream& os, piece v)
{
  os << "{ ";
  os << "valeur" << "=" << v.valeur;
  os << ", ";
  os << "pos_piece" << "=" << v.pos_piece;
  os << ", ";
  os << "tour_apparition" << "=" << v.tour_apparition;
  os << " }";
  return os;
}
extern "C" void api_afficher_piece(piece v)
{
  std::cerr << v << std::endl;
}

extern "C" bool api_mon_tour()
{
  return api->mon_tour();
}

extern "C" caracteristiques_objet api_proprietes_objet(type_objet to){
  return api->proprietes_objet(to);
}
