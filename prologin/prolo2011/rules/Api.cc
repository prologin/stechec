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
#include "Utils.hh"

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
// Retourne le num�ro de votre �quipe
//
int Api::mon_equipe()
{
  return g_->mon_equipe();
}

///
// Retourne les scores de chaque �quipe
//
std::vector<int> Api::scores()
{
    std::vector<int>        scores;
    std::vector<Joueur>&    joueurs = g_->joueurs;

    scores.reserve(joueurs.size());
    for (int i = 0; i < joueurs.size(); ++i)
        scores.push_back(joueurs[i].score);
    return (scores);
}

///
// Retourne le nombre d'�quipes sur le terrain
//
int Api::nombre_equipes()
{
    return g_->joueurs.size();
}

///
// Retourne le num�ro du tour actuel
//
int Api::tour_actuel()
{
  return g_->get_real_turn();
}

///
// Retourne la liste des sources d'�nergie
//
std::vector<source_energie> Api::sources_energie()
{
    std::vector<source_energie> result;
    std::vector<SourceEnergie>&  sources = g_->sources;

    result.reserve(sources.size());
    for (int i = 0; i < sources.size(); ++i)
        result.push_back(sources[i].to_source_energie(i));
    return (result);
}

///
// Retourne la liste des tra�n�es de moto
//
std::vector<trainee_moto> Api::trainees_moto()
{
  std::vector<trainee_moto>         out;
  std::vector<InternalTraineeMoto>& motos = g_->motos;

  out.reserve(motos.size());
  for (int i = 0; i < motos.size(); i++)
    out.push_back(motos[i].to_trainee_moto(i));
  return (out);
}

///
// Retourne le type d'une case
//
type_case Api::regarder_type_case(position pos)
{
    // FIXED by PM: assert position and return a default value (OBSTACLE) if
    // the positin is invalid

    if (position_invalide(pos.x, pos.y))
	return (OBSTACLE);
    Case &c = g_->get_case(pos);
    return (c.type);
}

///
// Retourne le type de bonus d'une case
//
type_bonus Api::regarder_type_bonus(position pos)
{
    // FIXED by PM: assert position and return a default value (PAS_BONUS) if
    // the positin is invalid
    if (position_invalide(pos.x, pos.y))
	return (PAS_BONUS);
    Case &c = g_->get_case(pos);
    return c.bonus;
}

///
// Retourne la liste des bonus d'une �quipe
//
std::vector<type_bonus> Api::regarder_bonus(int equipe)
{
    if (equipe < 0 || equipe >= g_->joueurs.size())
	return std::vector<type_bonus>();

    const std::vector<type_bonus>& bonus = g_->joueurs[equipe].bonus;
    std::vector<type_bonus>	result;

    result.reserve(bonus.size());
    for (int i = 0; i < bonus.size(); ++i)
	result.push_back(bonus[i]);
    return (result);
}

///
// D�place une moto
//
erreur Api::deplacer(int id, position de, position vers)
{
  LOG4("Api::deplacer...");
  DO_ACTION(ActionDeplacer, g_->get_current_player(),
            id, de, vers);
  LOG4("Api::deplacer : OK");
  return OK;
}

///
// Coupe une tra�n�e de moto en deux nouvelles tra�n�es. � entre � et � et � doivent �tre deux positions adjacentes occup�es par une m�me tra�n�e de moto.
//
erreur Api::couper_trainee_moto(int id, position entre, position et)
{
  // TODO
  abort();
}

///
// Annuler l'action pr�c�dente
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
// Enrouler la tra�n�e de moto en un point
//
erreur Api::enrouler(int id, position point)
{
  // TODO
  abort();
}

///
// R�g�n�re une source d'�nergie � son maximal
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
// Allonge une tra�n�e de moto. L'allongement se fera aux prochains d�placements. La longueur du prolongement doit �tre comprise entre 0 et MAX_ALLONGEMENT (inclus).
//
erreur Api::agrandir_trainee_moto(int id, int longueur)
{
  // TODO
  abort();
}

///
// Pose un point de croisement sur une case du terrain. La case doit ne pas d�j� �tre un point de croisement.
//
erreur Api::poser_point_croisement(position point)
{
  // TODO
  abort();
}

///
// Fusionner deux tra�n�es de moto. Les deux doivent appartenir � la m�me �quipe, mais doivent �tre deux tra�n�es distinctes. � pos1 � et � pos2 � doivent �tre adjacentes et occup�es respectivement par � id1 � et � id2 �.
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
