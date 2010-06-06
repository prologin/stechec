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

std::string convert_to_string(int i){
  std::string s="";
  s +=i;
  return s;
}
std::string convert_to_string(std::string i){
  return i;
}
std::string convert_to_string(bool i){
  return i?"true":"false";
}
std::string convert_to_string(erreur in){
  switch (in)
  {
    case OK: return "\"ok\"";
    case POSITION_INVALIDE: return "\"position_invalide\"";
    case PLUS_DE_PA: return "\"plus_de_pa\"";
    case PAS_A_PORTE: return "\"pas_a_porte\"";
    case UNITE_KO: return "\"unite_ko\"";
    case PAS_A_TOI: return "\"pas_a_toi\"";
    case UTILISATION_IMPOSSIBLE: return "\"utilisation_impossible\"";
    case PLUS_D_ARGENT: return "\"plus_d_argent\"";
  }
  return "bad value";
}
std::string convert_to_string(std::vector<erreur> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(s[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[0]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(type_objet in){
  switch (in)
  {
    case FILET: return "\"filet\"";
    case MARTEAU: return "\"marteau\"";
    case RIEN: return "\"rien\"";
  }
  return "bad value";
}
std::string convert_to_string(std::vector<type_objet> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(s[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[0]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(position in){
  std::string x = convert_to_string(in.x);
  std::string y = convert_to_string(in.y);
  std::string out = "{";
  out += "x:" + x;
  out += ", ";
  out += "y:" + y;
  return out + "}";
}

std::string convert_to_string(std::vector<position> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(s[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[0]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(caracteristiques_objet in){
  std::string cout = convert_to_string(in.cout);
  std::string porte = convert_to_string(in.porte);
  std::string out = "{";
  out += "cout:" + cout;
  out += ", ";
  out += "porte:" + porte;
  return out + "}";
}

std::string convert_to_string(std::vector<caracteristiques_objet> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(s[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[0]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(unite in){
  std::string pos_unite = convert_to_string(in.pos_unite);
  std::string team = convert_to_string(in.team);
  std::string ko = convert_to_string(in.ko);
  std::string pa = convert_to_string(in.pa);
  std::string objet = convert_to_string(in.objet);
  std::string id = convert_to_string(in.id);
  std::string out = "{";
  out += "pos_unite:" + pos_unite;
  out += ", ";
  out += "team:" + team;
  out += ", ";
  out += "ko:" + ko;
  out += ", ";
  out += "pa:" + pa;
  out += ", ";
  out += "objet:" + objet;
  out += ", ";
  out += "id:" + id;
  return out + "}";
}

std::string convert_to_string(std::vector<unite> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(s[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[0]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(piece in){
  std::string valeur = convert_to_string(in.valeur);
  std::string pos_piece = convert_to_string(in.pos_piece);
  std::string tour_apparition = convert_to_string(in.tour_apparition);
  std::string out = "{";
  out += "valeur:" + valeur;
  out += ", ";
  out += "pos_piece:" + pos_piece;
  out += ", ";
  out += "tour_apparition:" + tour_apparition;
  return out + "}";
}

std::string convert_to_string(std::vector<piece> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(s[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[0]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string dump_infos()
{
  std::string string_info1 = convert_to_string(api->unites());
  std::string string_info2 = convert_to_string(api->pieces_a_vennir());
  std::string string_info3 = convert_to_string(api->pieces_en_jeu());
  // TODO modifie les infos ici, si besoin (si tu as une vue subjective, ca peut-etre utile)
  std::string out = "[";
  out += string_info1 + ", " + string_info2 + ", " + string_info3;
  return out + "]";
}
///
// Renvoie le numero de votre equipe.
//
extern "C" int api_mon_equipe()
{
  return api->mon_equipe();
}

///
// Renvoie le score d'une equipe.
//
extern "C" int api_score(int team)
{
  return api->score(team);
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
// Retourne les caracteristiques de l'objet.
//
extern "C" caracteristiques_objet api_proprietes_objet(type_objet to)
{
  return api->proprietes_objet(to);
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
  case POSITION_INVALIDE: os << "POSITION_INVALIDE"; break;
  case PLUS_DE_PA: os << "PLUS_DE_PA"; break;
  case PAS_A_PORTE: os << "PAS_A_PORTE"; break;
  case UNITE_KO: os << "UNITE_KO"; break;
  case PAS_A_TOI: os << "PAS_A_TOI"; break;
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

