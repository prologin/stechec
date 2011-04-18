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
      LOG4("DO_ACTION : OK");			\
      return OK;				\
    } catch (erreur err) {			\
      LOG1("erreur : %1", err);			\
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
    std::vector<trainee_moto>		out;
    GameData::motos_type&			motos = g_->motos;
    GameData::motos_type::const_iterator	it;

    out.reserve(motos.size());
    for (it = motos.begin(); it != motos.end(); ++it)
	out.push_back(it->second.to_trainee_moto());
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

    std::vector<type_bonus>	result;
    g_->get_bonus_joueur(equipe, result);
    return (result);
}

///
// Retourne la liste des id des tra�n�es pr�sentes sur une case
//
std::vector<int> Api::regarder_trainee_case(position pos)
{
  std::vector<int> trainees;
  g_->lookup_trainee_case(pos, trainees);
  return trainees;
}

///
// Retourne si une case peut �tre travers�e par une tra�n�e de plus
//
bool Api::case_traversable(position pos)
{
    return g_->is_crossable_pos(pos);
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
    LOG4("Api::couper_trainee_moto");
    DO_ACTION(ActionCouperTraineeMoto, g_->get_current_player(),
	      id, entre, et);
    LOG4("Api::deplacer : OK");
    return OK;
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
    LOG4("Api::enrouler");
    DO_ACTION(ActionEnrouler, g_->get_current_player(),
	      id, point);
    LOG4("Api::enrouler : OK");
    return OK;
}

///
// R�g�n�re une source d'�nergie � son maximal
//
erreur Api::regenerer_source_energie(int id)
{
    LOG4("Api::regenerer");
    DO_ACTION(ActionRegenererSourceEnergie, g_->get_current_player(),
	      id);
    LOG4("Api::regenerer : OK");
    return OK;
}

///
// Allonge le tour en rajoutant des points d'action
//
erreur Api::allonger_pa()
{
    LOG4("Api::allonger_pa");
    DO_ACTION(ActionAllongerPA, g_->get_current_player());
    LOG4("Api::allonger_pa : OK");
    return OK;
}

///
// Allonge une tra�n�e de moto. L'allongement se fera aux prochains d�placements. La longueur du prolongement doit �tre comprise entre 0 et MAX_ALLONGEMENT (inclus).
//
erreur Api::agrandir_trainee_moto(int id, int longueur)
{
    LOG4("Api::agrandir_trainee_moto");
    DO_ACTION(ActionAgrandirTraineeMoto, g_->get_current_player(), id, longueur);
    LOG4("Api::agrandir_trainee_moto : OK");
    return OK;
}

///
// Pose un point de croisement sur une case du terrain. La case doit ne pas d�j� �tre un point de croisement.
//
erreur Api::poser_point_croisement(position point)
{
    LOG4("Api::poser_point_croisement");
    DO_ACTION(ActionPoserPointCroisement, g_->get_current_player(), point);
    LOG4("Api::poser_point_croisement : OK");
    return OK;
}

///
// Fusionner deux tra�n�es de moto. Les deux doivent appartenir � la m�me �quipe, mais doivent �tre deux tra�n�es distinctes. � pos1 � et � pos2 � doivent �tre adjacentes et occup�es respectivement par � id1 � et � id2 �.
//
erreur Api::fusionner(int	id1, position	pos1,
		      int	id2, position	pos2)
{
    LOG4("Api::fusionner");
    DO_ACTION(ActionFusionner, g_->get_current_player(),
	      id1, pos1, id2, pos2);
    LOG4("Api::fusionner OK");
    return OK;
}

std::vector<position> Api::chemin(position p1, position p2){
  std::vector<position> v;
  g_->get_shortest_path(p1, p2, v);
  return v;
}

std::vector<std::vector<int> > Api::actions_effectuees()
{
  LOG4("Api::actions_effectuees");
    return g_->actions_stockees;
}

bool Api::mon_tour()
{
  return g_->mon_tour();
}

void Api::teamSwitched()
{
}

int Api::diff_score(){
  return g_->apply_connections(false);
}

void Api::sendActions()
{
g_->send_actions();
}
