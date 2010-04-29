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
** @author delroth
** @author Maxime Audouin <maxime.audouin@mlstate.com>
*/

#include <cstdlib>
#include <vector>
#include "Api.hh"
#include <cmath>




// global used in interface.cc
Api* api;


// internal API
bool Api::need_retirer_ko()
{
  return g_->must_remove_ko();
}

bool Api::retirer_ko(unite u)
{
  return u.ko >= 0;
}

bool Api::mon_tour()
{
  return g_->mon_tour();
}

// constructor
Api::Api(GameData* gameData, Client* c) : StechecApi(gameData, c)
{
  api = this;
}

// user API

int Api::nombre_pc(){
  return g_->nbr_unites_allowed;
}

int Api::nombre_unites_spawnees(bool ennemi){
  int e = ennemi?g_->current_player:((1 + g_->current_player)%2);
  return g_->nbr_toons_spawn[e];
}

///
// Renvoie le numéro du tour actuel.
//
int Api::tour_actuel()
{
  return g_->get_real_turn();;  // TODO
}

///
// Renvoie la position du spawn (ennemi ou non).
//
position Api::pos_spawn(bool ennemi)
{
  return g_->spawn_position(ennemi);
}

///
// Renvoie les caractéristiques d'un type d'unité.
//
caracs Api::caracteristiques(type_unite tu)
{
  return g_->caracteristiques(tu);
}

///
// Retourne une structure \texttt{cartes} contenant les informations sur les cartes que vous avez en main.
//
cartes Api::mes_cartes()
{
  return g_->get_cartes(g_->get_current_player());
}

///
// Retourne la liste des unités actuellement en jeu.
//
std::vector<unite> Api::unites()
{
  return g_->get_unites();
}

///
// Retourne la taille actuelle du terrain et les coordonnées min/max dans une structure \texttt{taille_terrain}.
//
taille_terrain Api::taille_terrain_actuelle()
{
  return g_->get_tt();
}

#define DO_ACTION(type, ...) \
  try { \
    Action* act = new type(__VA_ARGS__); \
    act->verifier(g_); \
    g_->appliquer_action(act); \
    return OK; \
  } catch (erreur err) { \
    return err; \
  }

#define PLAY_CARD \
  ASSERT(g_->can_play_card, PHASE_CARTES_TERMINEE);	\


///
// Utilise une carte « Quoi d'neuf docteur ? » que vous avez dans votre main.
//
erreur Api::soin(position cible)
{
  LOG3("soin : (%1, %2)", cible.x, cible.y);
  g_->check(__FILE__, __LINE__);
  DO_ACTION(ActionSoin, g_->get_current_player(), g_->indice_at(cible));
  return OK;
}

///
// Utilise une carte « Déguisement » que vous avez dans votre main.
//
erreur Api::deguisement(position cible, type_unite nouveau_type)
{
  LOG3("cible : (%1, %2) type : %3", cible.x, cible.y, nouveau_type);
  g_->check(__FILE__, __LINE__);
  DO_ACTION(ActionDeguisement, g_->get_current_player(), g_->indice_at(cible),
            nouveau_type);
  return OK;
}

///
// Utilise une carte « Banzaï » que vous avez dans votre main.
//
erreur Api::banzai(position cible)
{
  LOG3("banzai : (%1, %2)", cible.x, cible.y);
  g_->check(__FILE__, __LINE__);
  DO_ACTION(ActionBanzai, g_->get_current_player(), g_->indice_at(cible));
  return OK;
}

///
// Utilise une carte « Pacifisme » que vous avez dans votre main.
//
erreur Api::pacifisme()
{
  LOG3("pacifisme");
  g_->check(__FILE__, __LINE__);
  DO_ACTION(ActionPacifisme, g_->get_current_player());
  return OK;
}

///
// Déplace une unité vers une position à portée.
//
erreur Api::deplacer(position cible, position pos)
{
  LOG3("deplacer: (%1, %2) (%3, %4)",  cible.x, cible.y, pos.x, pos.y);
  g_->check(__FILE__, __LINE__);
  DO_ACTION(ActionDeplacer, g_->get_current_player(),
            g_->indice_at(cible), pos);
  return OK;
}

///
// Relève une unité n'ayant plus de tours KO.
//
erreur Api::relever(position cible)
{
  LOG3("relever : (%1, %2)", cible.x, cible.y);
  int qui = g_->indice_at(cible);
  g_->check(__FILE__, __LINE__);
  DO_ACTION(ActionRelever, g_->get_current_player(), qui);
  return OK;
}

///
// Attaque une autre unité.
//
erreur Api::attaquer(position attaquant, position cible)
{
  LOG3("attaquer: (%1, %2) (%3, %4)", attaquant.x, attaquant.y, cible.x, cible.y);
  g_->check(__FILE__, __LINE__);
  DO_ACTION(ActionAttaquer, g_->get_current_player(),
	    g_->indice_at(attaquant),
	    g_->indice_at(cible));
  return OK;
}

///
// Fait apparaitre une unité sur la case de spawn.
//
erreur Api::spawn(type_unite quoi)
{
  LOG3("spawn %1", quoi);
  g_->check(__FILE__, __LINE__);
  DO_ACTION(ActionSpawn, g_->get_current_player(), quoi);
  return OK;
}

///
// Annule l'effet de la dernière action et remet le jeu dans l'état précédent.
//
bool Api::annuler()
{
  LOG3("annuler");
  bool out = g_->annuler();
  g_->check(__FILE__, __LINE__);
  return out;
}



void Api::teamSwitched()
{
}

void Api::sendActions()
{
g_->send_actions();
}
