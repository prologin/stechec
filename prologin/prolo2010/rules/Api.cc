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
  DO_ACTION(ActionSoin, g_->get_current_player(), g_->indice_at(cible));
}

///
// Utilise une carte « Déguisement » que vous avez dans votre main.
//
erreur Api::deguisement(position cible, type_unite nouveau_type)
{
  DO_ACTION(ActionDeguisement, g_->get_current_player(), g_->indice_at(cible),
            nouveau_type);
}

///
// Utilise une carte « Banzaï » que vous avez dans votre main.
//
erreur Api::banzai(position cible)
{
  DO_ACTION(ActionBanzai, g_->get_current_player(), g_->indice_at(cible));
}

///
// Utilise une carte « Pacifisme » que vous avez dans votre main.
//
erreur Api::pacifisme()
{
  DO_ACTION(ActionPacifisme, g_->get_current_player());
}

///
// Déplace une unité vers une position à portée.
//
erreur Api::deplacer(position cible, position pos)
{
  DO_ACTION(ActionDeplacer, g_->get_current_player(),
            g_->indice_at(cible), pos);
}

///
// Relève une unité n'ayant plus de tours KO.
//
erreur Api::relever(position cible)
{
  int qui = g_->indice_at(cible);
  DO_ACTION(ActionRelever, g_->get_current_player(), qui);
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
  DO_ACTION(ActionSpawn, g_->get_current_player(), quoi);
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

