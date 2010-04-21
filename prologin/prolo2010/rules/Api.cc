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

static inline int max(int a, int b){
  return (a > b) ? a : b;
}

static inline int distance(position p1, position p2){
  return max(
	     abs(p1.x - p2.x),
	     abs(p1.y - p2.y));
}



// global used in interface.cc
Api* api;


// internal API
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

///
// Renvoie le numéro du tour actuel.
//
int Api::tour_actuel()
{
  return 42;  // todo
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

#define PLAY_CARD \
  ASSERT(g_->can_play_card(), PHASE_CARTES_TERMINEE);	\


///
// Utilise une carte « Quoi d'neuf docteur ? » que vous avez dans votre main.
//
erreur Api::soin(unite cible)
{
  PLAY_CARD;
  return OK;
}

///
// Utilise une carte « Déguisement » que vous avez dans votre main.
//
erreur Api::deguisement(unite cible, type_unite nouveau_type)
{
  PLAY_CARD;
  return OK;
}

///
// Utilise une carte « Banzaï » que vous avez dans votre main.
//
erreur Api::banzai(unite cible)
{
  PLAY_CARD;
  return OK;
}

///
// Utilise une carte « Pacifisme » que vous avez dans votre main.
//
erreur Api::pacifisme()
{
  PLAY_CARD;
  return OK;
}

///
// Déplace une unité vers une position à portée.
//
erreur Api::deplacer(unite cible, position pos)
{
  int indice = g_->indice_of(cible);
  unite u = g_->get_unite(indice);
  ASSERT(indice != -1, PAS_A_MOI); // todo
  ASSERT(!u.ennemi, PAS_A_MOI);
  int d = distance(pos, cible.pos);
  ASSERT(u.pa >= d, PAS_A_PORTEE);
  ASSERT(g_->can_active(indice), PLUS_DE_PA); // todo
  Actions a;
  a.deplacer(indice, pos, d);
  g_->appliquer_action(a);
  return OK;
}

///
// Attaque une autre unité.
//
erreur Api::attaquer(unite attaquant, unite cible)
{
  return OK; // todo
}

///
// Fait apparaitre une unité sur la case de spawn.
//
erreur Api::spawn(type_unite quoi)
{
  position p = g_->spawn_pos();
  int i =  g_->toon_at(p);
  ASSERT(i == -1, SPAWN_OCCUPE);
  ASSERT(quoi != PERROQUET, PAS_SPAWNABLE);
  ASSERT(g_->nbr_toons(false) < 9, PAS_SPAWNABLE); // todo
  Actions a;
  a.spawn(quoi);
  g_->appliquer_action(a);
  return OK;
}

///
// Annule l'effet de la dernière action et remet le jeu dans l'état précédent.
//
bool Api::annuler()
{
  return g_->annuler();
}



void Api::teamSwitched()
{
  g_->team_switched();
}

