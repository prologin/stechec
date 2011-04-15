/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2011 Prologin
*/

#include "Api.hh"
#include "interface.hh"
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


// todo avoir un ostringstream a la place de std::string

std::string convert_to_string(int i){
  std::ostringstream s;
  s << i;
  std::string result = s.str();
  return result;
}
std::string convert_to_string(std::string i){
  return i;
}
std::string convert_to_string(bool i){
  return i?"true":"false";
}
std::string convert_to_string(std::vector<int> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(erreur in){
  switch (in)
  {
    case OK: return "\"ok\"";
    case ID_INVALIDE: return "\"id_invalide\"";
    case POSITION_INVALIDE: return "\"position_invalide\"";
    case PLUS_DE_PA: return "\"plus_de_pa\"";
    case BONUS_INVALIDE: return "\"bonus_invalide\"";
    case PAS_A_TOI: return "\"pas_a_toi\"";
  }
  return "bad value";
}
std::string convert_to_string(std::vector<erreur> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(type_case in){
  switch (in)
  {
    case VIDE: return "\"vide\"";
    case OBSTACLE: return "\"obstacle\"";
    case BONUS: return "\"bonus\"";
    case POINT_CROISEMENT: return "\"point_croisement\"";
  }
  return "bad value";
}
std::string convert_to_string(std::vector<type_case> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(type_bonus in){
  switch (in)
  {
    case VIDE: return "\"vide\"";
    case BONUS_CROISEMENT: return "\"bonus_croisement\"";
    case PLUS_LONG: return "\"plus_long\"";
    case PLUS_PA: return "\"plus_pa\"";
    case BONUS_REGENERATION: return "\"bonus_regeneration\"";
  }
  return "bad value";
}
std::string convert_to_string(std::vector<type_bonus> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
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
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(source_energie in){
  std::string id = convert_to_string(in.id);
  std::string pos = convert_to_string(in.pos);
  std::string coef = convert_to_string(in.coef);
  std::string out = "{";
  out += "id:" + id;
  out += ", ";
  out += "pos:" + pos;
  out += ", ";
  out += "coef:" + coef;
  return out + "}";
}

std::string convert_to_string(std::vector<source_energie> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(trainee_moto in){
  std::string id = convert_to_string(in.id);
  std::string emplacement = convert_to_string(in.emplacement);
  std::string team = convert_to_string(in.team);
  std::string out = "{";
  out += "id:" + id;
  out += ", ";
  out += "emplacement:" + emplacement;
  out += ", ";
  out += "team:" + team;
  return out + "}";
}

std::string convert_to_string(std::vector<trainee_moto> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string dump_infos()
{
  std::string string_info1 = convert_to_string(api->sources_energie());
  std::string string_info2 = convert_to_string(api->trainees_moto());
  std::string string_info3 = convert_to_string(api->scores());
  // TODO modifie les infos ici, si besoin (si tu as une vue subjective, ca peut-etre utile)
  std::string out = "[";
  out += string_info1 + ", " + string_info2 + ", " + string_info3;
  return out + "]";
}
///
// Retourne le numéro de votre équipe
//
extern "C" int api_mon_equipe()
{
  return api->mon_equipe();
}

///
// Retourne les scores de chaque équipe
//
extern "C" std::vector<int> api_scores()
{
  return api->scores();
}

///
// Retourne le nombre d'équipes sur le terrain
//
extern "C" int api_nombre_equipes()
{
  return api->nombre_equipes();
}

///
// Retourne le numéro du tour actuel
//
extern "C" int api_tour_actuel()
{
  return api->tour_actuel();
}

///
// Retourne la liste des sources d'énergie
//
extern "C" std::vector<source_energie> api_sources_energie()
{
  return api->sources_energie();
}

///
// Retourne la liste des traînées de moto
//
extern "C" std::vector<trainee_moto> api_trainees_moto()
{
  return api->trainees_moto();
}

///
// Retourne le type d'une case
//
extern "C" type_case api_regarder_type_case(position pos)
{
  return api->regarder_type_case(pos);
}

///
// Retourne le type de bonus d'une case
//
extern "C" type_bonus api_regarder_type_bonus(position pos)
{
  return api->regarder_type_bonus(pos);
}

///
// Retourne la liste des bonus d'une équipe
//
extern "C" std::vector<type_bonus> api_regarder_bonus(int equipe)
{
  return api->regarder_bonus(equipe);
}

///
// Déplace une moto
//
extern "C" erreur api_deplacer(int id, position de, position vers)
{
  return api->deplacer(id, de, vers);
}

///
// Coupe une traînée de moto en deux nouvelles traînées. « entre » et « et » doivent être deux positions adjacentes occupées par une même traînée de moto.
//
extern "C" erreur api_couper_trainee_moto(int id, position entre, position et)
{
  return api->couper_trainee_moto(id, entre, et);
}

///
// Annuler l'action précédente
//
extern "C" erreur api_cancel()
{
  return api->cancel();
}

///
// Enrouler la traînée de moto en un point
//
extern "C" erreur api_enrouler(int id, position point)
{
  return api->enrouler(id, point);
}

///
// Régénère une source d'énergie à son maximal
//
extern "C" erreur api_regenerer_source_energie(int id)
{
  return api->regenerer_source_energie(id);
}

///
// Allonge le tour en rajoutant des points d'action
//
extern "C" erreur api_allonger_pa()
{
  return api->allonger_pa();
}

///
// Allonge le tour en rajoutant des points d'action
//
extern "C" std::vector<position> api_chemin(position p1, position p2)
{
  return api->chemin(p1, p2);
}

///
// Allonge une traînée de moto. L'allongement se fera aux prochains déplacements. La longueur du prolongement doit être comprise entre 0 et MAX_ALLONGEMENT (inclus).
//
extern "C" erreur api_agrandir_trainee_moto(int id, int longueur)
{
  return api->agrandir_trainee_moto(id, longueur);
}

///
// Pose un point de croisement sur une case du terrain. La case doit ne pas déjà être un point de croisement.
//
extern "C" erreur api_poser_point_croisement(position point)
{
  return api->poser_point_croisement(point);
}

///
// Fusionner deux traînées de moto. Les deux doivent appartenir à la même équipe, mais doivent être deux traînées distinctes. « pos1 » et « pos2 » doivent être adjacentes et occupées respectivement par « id1 » et « id2 ».
//
extern "C" erreur api_fusionner(int id1, position pos1, int id2, position pos2)
{
  return api->fusionner(id1, pos1, id2, pos2);
}

///
// Affiche le contenu d'une valeur de type erreur
//
std::ostream& operator<<(std::ostream& os, erreur v)
{
  switch (v) {
  case OK: os << "OK"; break;
  case ID_INVALIDE: os << "ID_INVALIDE"; break;
  case POSITION_INVALIDE: os << "POSITION_INVALIDE"; break;
  case PLUS_DE_PA: os << "PLUS_DE_PA"; break;
  case BONUS_INVALIDE: os << "BONUS_INVALIDE"; break;
  case PAS_A_TOI: os << "PAS_A_TOI"; break;
  }
  return os;
}
extern "C" void api_afficher_erreur(erreur v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type type_case
//
std::ostream& operator<<(std::ostream& os, type_case v)
{
  switch (v) {
  case VIDE: os << "VIDE"; break;
  case OBSTACLE: os << "OBSTACLE"; break;
  case BONUS: os << "BONUS"; break;
  case POINT_CROISEMENT: os << "POINT_CROISEMENT"; break;
  }
  return os;
}
extern "C" void api_afficher_type_case(type_case v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type bonus
//
std::ostream& operator<<(std::ostream& os, type_bonus v)
{
  switch (v) {
  case VIDE: os << "VIDE"; break;
  case BONUS_CROISEMENT: os << "BONUS_CROISEMENT"; break;
  case PLUS_LONG: os << "PLUS_LONG"; break;
  case PLUS_PA: os << "PLUS_PA"; break;
  case BONUS_REGENERATION: os << "BONUS_REGENERATION"; break;
  }
  return os;
}
extern "C" void api_afficher_type_bonus(type_bonus v)
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
// Affiche le contenu d'une valeur de type source_energie
//
std::ostream& operator<<(std::ostream& os, source_energie v)
{
  os << "{ ";
  os << "id" << "=" << v.id;
  os << ", ";
  os << "pos" << "=" << v.pos;
  os << ", ";
  os << "coef" << "=" << v.coef;
  os << " }";
  return os;
}
extern "C" void api_afficher_source_energie(source_energie v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type trainee_moto
//
std::ostream& operator<<(std::ostream& os, trainee_moto v)
{
  os << "{ ";
  os << "id" << "=" << v.id;
  os << ", ";
  os << "emplacement" << "=" << v.emplacement;
  os << ", ";
  os << "team" << "=" << v.team;
  os << " }";
  return os;
}
extern "C" void api_afficher_trainee_moto(trainee_moto v)
{
  std::cerr << v << std::endl;
}

