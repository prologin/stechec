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

#include <stdlib.h>

#include "Api.hh"

// global used in interface.cc
Api* api;


Api::Api(GameData* gameData, Client* c) : StechecApi(gameData, c)
{
  api = this;
}

#define DO_ACTION(type, ...)			\
  if (g_->can_play) {				\
    try {					\
      Action* act = new type(__VA_ARGS__);	\
      LOG4("DO_ACTION : verifier...");		\
      act->verifier(g_);			\
      LOG4("DO_ACTION : appliquer...");		\
      g_->appliquer_action(act);		\
      LOG4("DO_ACTION : OK");		\
      return OK;				\
    } catch (erreur err) {			\
      LOG1("erreur : %1", err);				\
      return err;				\
    }						\
  }else{					\
    LOG1("ERREUR : PAS TON TOUR !");		\
    abort();					\
  }


///
// Retourne le numéro de votre équipe
//
int Api::mon_equipe()
{
  return g_->mon_equipe();
}

///
// Retourne les scores de chaque équipe
//
std::vector<int> Api::scores()
{
  // TODO
  abort();
}

///
// Retourne le nombre d'équipes sur le terrain
//
int Api::nombre_equipes()
{
  // TODO
  abort();
}

///
// Retourne le numéro du tour actuel
//
int Api::tour_actuel()
{
  return g_->get_real_turn();
}

///
// Retourne la liste des sources d'énergie
//
std::vector<source_energie> Api::sources_energie()
{
  // TODO
  abort();
}

///
// Retourne la liste des traînées de moto
//
std::vector<trainee_moto> Api::trainees_moto()
{
  int len = g_->motos.size();
  std::vector<trainee_moto> out;
  out.reserve(len);
  for (int i = 0; i < len; i++){
    out.push_back(g_->motos.at(i).to_trainee_moto(i));
  }
  return out;
}

///
// Retourne le type d'une case
//
type_case Api::regarder_type_case(position pos)
{
  // TODO
  abort();
}

///
// Retourne le type de bonus d'une case
//
type_bonus Api::regarder_type_bonus(position pos)
{
  // TODO
  abort();
}

///
// Retourne la liste des bonus d'une équipe
//
std::vector<type_bonus> Api::regarder_bonus(int equipe)
{
  // TODO
  abort();
}

///
// Déplace une moto
//
erreur Api::deplacer(int id, position de, position vers)
{
  // TODO
  abort();
}

///
// Coupe une traînée de moto en deux nouvelles traînées. « entre » et « et » doivent être deux positions adjacentes occupées par une même traînée de moto.
//
erreur Api::couper_trainee_moto(int id, position entre, position et)
{
  // TODO
  abort();
}

///
// Annuler l'action précédente
//
erreur Api::cancel()
{
  LOG2("annuler");
  LOG4("state avant annuler"); 
  g_->check(__FILE__, __LINE__);
  bool out = g_->annuler();
  LOG4("state apres annuler"); 
  g_->check(__FILE__, __LINE__);
  // TODO return out
  return OK;
}

///
// Enrouler la traînée de moto en un point
//
erreur Api::enrouler(int id, position point)
{
  // TODO
  abort();
}

///
// Régénère une source d'énergie à son maximal
//
erreur Api::regenerer_source_energie(int id)
{
  // TODO
  abort();
}

///
// Allonge le tour en rajoutant des points d'action
//
erreur Api::allonger_pa()
{
  // TODO
  abort();
}

///
// Allonge une traînée de moto. L'allongement se fera aux prochains déplacements. La longueur du prolongement doit être comprise entre 0 et MAX_ALLONGEMENT (inclus).
//
erreur Api::agrandir_trainee_moto(int id, int longueur)
{
  // TODO
  abort();
}

///
// Pose un point de croisement sur une case du terrain. La case doit ne pas déjà être un point de croisement.
//
erreur Api::poser_point_croisement(position point)
{
  // TODO
  abort();
}

///
// Fusionner deux traînées de moto. Les deux doivent appartenir à la même équipe, mais doivent être deux traînées distinctes. « pos1 » et « pos2 » doivent être adjacentes et occupées respectivement par « id1 » et « id2 ».
//
erreur Api::fusionner(int id1, position pos1, int id2, position pos2)
{
  // TODO
  abort();
}

bool Api::mon_tour()
{
  return g_->mon_tour();
}

void Api::teamSwitched()
{
}

void Api::sendActions()
{
g_->send_actions();
}
