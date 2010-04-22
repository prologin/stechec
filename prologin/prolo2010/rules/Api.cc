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
bool Api::need_retirer_ko()
{
  for (std::vector<unite>::iterator it = g_->unites.begin();
       it != g_->unites.end(); ++it)
  {
    if (it->ennemi && it->ko < 0)
      return true;
  }

  return false;
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

///
// Renvoie le numéro du tour actuel.
//
int Api::tour_actuel()
{
  return g_->get_real_turn();;  // todo
}

///
// Renvoie la position du spawn (ennemi ou non).
//
position Api::pos_spawn(bool ennemi)
{
  // TODO
  abort();
}

///
// Renvoie les caractéristiques d'un type d'unité.
//
caracs Api::caracteristiques(type_unite tu)
{
  static caracs c[] = {
    { 1, 1 },
    { 2, 3 },
    { 5, 1 },
    { 3, 2 }
  };

  return c[tu];
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
  ASSERT(g_->can_play_card, PHASE_CARTES_TERMINEE);	\


///
// Utilise une carte « Quoi d'neuf docteur ? » que vous avez dans votre main.
//
erreur Api::soin(position cible)
{
  PLAY_CARD;
  g_->appliquer_action(new ActionSoin(g_->get_current_player(), g_->indice_at(cible)));
  return OK;
}

///
// Utilise une carte « Déguisement » que vous avez dans votre main.
//
erreur Api::deguisement(position cible, type_unite nouveau_type)
{
  PLAY_CARD;
  g_->appliquer_action(new ActionDeguisement(g_->get_current_player(), g_->indice_at(cible),
                                             nouveau_type));
  return OK;
}

///
// Utilise une carte « Banzaï » que vous avez dans votre main.
//
erreur Api::banzai(position cible)
{
  PLAY_CARD;
  g_->appliquer_action(new ActionBanzai(g_->get_current_player(), g_->indice_at(cible)));
  return OK;
}

///
// Utilise une carte « Pacifisme » que vous avez dans votre main.
//
erreur Api::pacifisme()
{
  PLAY_CARD;
  g_->appliquer_action(new ActionPacifisme(g_->get_current_player()));
  return OK;
}

///
// Déplace une unité vers une position à portée.
//
erreur Api::deplacer(position cible, position pos)
{
  int indice = g_->indice_at(cible);
  unite u = g_->get_unite(indice);
  ASSERT(indice != -1, PAS_A_MOI); // todo
  ASSERT(!u.ennemi, PAS_A_MOI);
  int d = distance(pos, cible);
  ASSERT(u.pa >= d, PAS_A_PORTEE);
  ASSERT(g_->can_active(indice), PLUS_DE_PA); // todo

  g_->appliquer_action(new ActionDeplacer(g_->get_current_player(), indice, pos));

  return OK;
}

///
// Relève une unité n'ayant plus de tours KO.
//
erreur Api::relever(position cible)
{
  // TODO
  return OK;
}

///
// Attaque une autre unité.
//
erreur Api::attaquer(position attaquant, position cible)
{
 /* TODO */
  return OK;
}

///
// Fait apparaitre une unité sur la case de spawn.
//
erreur Api::spawn(type_unite quoi)
{
  position p = g_->spawn_pos();
  int i =  g_->indice_at(p);
  ASSERT(i == -1, SPAWN_OCCUPE);
  ASSERT(quoi != PERROQUET, PAS_SPAWNABLE);
  ASSERT(g_->nbr_toons(false) < 9, PAS_SPAWNABLE); // todo

  /* TODO */

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

