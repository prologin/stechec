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


extern bool operator==(position p1, position p2);

// global used in interface.cc
Api* api;

bool operator==(taille_terrain p1, taille_terrain p2){
  return p1.taille == p2.taille &&
    p1.min_coord == p2.min_coord &&
    p1.max_coord == p2.max_coord;
}
bool operator==(cartes p1, cartes p2){
  return p1.potion == p2.potion &&
    p1.deguisement == p2.deguisement &&
    p1.banzai == p2.banzai &&
    p1.pacifisme == p2.pacifisme;
}
bool operator==(unite u1, unite u2){
  return u1.pos == u2.pos &&
    u1.ennemi == u2.ennemi &&
    u1.type_unite_actuel == u2.type_unite_actuel &&
    u1.vrai_type_unite == u2.vrai_type_unite &&
    u1.ko == u2.ko &&
    u1.pa == u2.pa &&
    u1.attaques == u2.attaques &&
    u1.attaques_gratuites == u2.attaques_gratuites &&
    u1.id == u2.id;
}

bool operator==(GameData p1, GameData p2){
  for (int i = 0; i < NBR_MAX_UNITES * 2; i++){
    if (p1.deja_bougee[i] != p2.deja_bougee[i]){
      LOG3("deja_bougee %1 differentes", i);
      return false;
    }
  }
  if (p1.unites.size() != p2.unites.size()) return false;
  for (int i = 0; i < p1.unites.size(); i++){
    if (p1.unites[i] == p2.unites[i]) continue;
    LOG3("unite %1 differentes", i);
    return false;
  }
  if (p1.current_player != p2.current_player){
    LOG3("mauvais current_player");
    return false;
  }
  if (p1.nbr_unites_allowed != p2.nbr_unites_allowed){
    LOG3("mauvais nbr unites allowed");
    return false;
  }
  if (p1.tt == p2.tt){
    if (p1.players_cartes[0] == p2.players_cartes[0] &&
	p1.players_cartes[1] == p2.players_cartes[1]){
      return
	p1.can_play_card == p2.can_play_card &&
	p1.players_cartes[0] == p2.players_cartes[0] &&
	p1.players_cartes[1] == p2.players_cartes[1] &&
	p1.nbr_toons_spawn[0] == p2.nbr_toons_spawn[0] &&
	p1.nbr_toons_spawn[1] == p2.nbr_toons_spawn[1];
    }else{
      LOG3("mauvaise cartes");
      return false;
    }
  }else{
    LOG3("mauvaise taille terrain");
    return false;
  }

}

// internal API
bool Api::need_retirer_ko()
{
  return g_->must_remove_ko();
}

bool Api::retirer_ko(position p)
{
  int indice = g_->indice_at(p);
  if (indice == -1 || g_->retirer_ko(indice)) {
    StechecPkt com(RETIRER_KO, -1);
    com.Push(2, 0, indice);
    SendToServer(com);
    return true;
  }else{
    return false;
  }
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
  return g_->nbr_toons(ennemi);
}

///
// Renvoie le numéro du tour actuel.
//
int Api::tour_actuel()
{
  return g_->get_real_turn();;
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
  int tuu = tu;  // necessary, otherwise the compiler can assume the eunm
                 // variable is in the range.
  if (tuu < 0 || tuu >= DERNIERE_UNITE) {
    caracs dummy; dummy.pa_init = -1; dummy.portee = -1;
    return dummy.
  }
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
  if (g_->can_play) { \
  try { \
    Action* act = new type(__VA_ARGS__); \
    act->verifier(g_); \
    g_->appliquer_action(act); \
    return OK; \
  } catch (erreur err) { \
    return err; \
  } }else abort();

#define PLAY_CARD \
  ASSERT(g_->can_play_card, PHASE_CARTES_TERMINEE);	\


///
// Utilise une carte « Quoi d'neuf docteur ? » que vous avez dans votre main.
//
erreur Api::soin(position cible)
{
  LOG3("soin : (%1, %2)", cible.x, cible.y);
  g_->check(__FILE__, __LINE__);
#ifdef DEBUG
  check.push_back(*g_);
#endif
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
#ifdef DEBUG
  check.push_back(*g_);
#endif
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
#ifdef DEBUG
  check.push_back(*g_);
#endif
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
#ifdef DEBUG
  check.push_back(*g_);
#endif
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
#ifdef DEBUG
  check.push_back(*g_);
#endif
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
#ifdef DEBUG
  check.push_back(*g_);
#endif
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
#ifdef DEBUG
  check.push_back(*g_);
#endif
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
#ifdef DEBUG
  check.push_back(*g_);
#endif
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
#ifdef DEBUG
  if (out) {
    if (check.size() == 0){
      LOG3("erreur dans les actions (empty stack)");
      abort();
    }
    GameData g = check[check.size() - 1];
    
    g.check(" TOTO", 42);
    
    check.pop_back();
    if ( ! (*g_ == g ) ){
      LOG3("erreur dans les actions (bad state)");
      abort();
    }
  }
#endif
  return out;
}



void Api::teamSwitched()
{
}

void Api::sendActions()
{
g_->send_actions();
}
