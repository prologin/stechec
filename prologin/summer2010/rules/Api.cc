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

#include <stdlib.h>

#include "Api.hh"

// global used in interface.cc
Api* api;


Api::Api(GameData* gameData, Client* c) : StechecApi(gameData, c)
{
  api = this;
  last_order_id = 0;
}

///
// Renvoie le numero de votre equipe.
//
int Api::mon_equipe()
{
  return g_->get_current_player();
}

///
// Renvoie le nombre d'equipes sur la map
//
int Api::nombre_equipes()
{
  return NB_TEAMS;
}

///
// Renvoie le numéro du tour actuel.
//
int Api::tour_actuel()
{
  return g_->get_real_turn();
}

///
// Renvoie le numéro du tour actuel.
//
int Api::score(int team)
{
  return g_->score(team);
}

bool Api::mon_tour()
{
  return g_->mon_tour();
}

///
// Renvoie les pieces qui sont sur la map
//
std::vector<piece> Api::pieces_en_jeu()
{
  std::vector<piece> out;
  std::map<position, piece>::const_iterator itr;
  std::map<position, piece> *map = &(g_->map_p);
  for(itr = map->begin(); itr != map->end(); ++itr){
    out.push_back(itr->second);
  }
  return out;
}

///
// Renvoie le 10 prochaines pieces a vennir
//
std::vector<piece> Api::pieces_a_vennir()
{
  std::vector<piece> v;
  for (int i = 0; i < NB_PIECES_SUIVANTES_VISIBLES; i ++)
    {
      v.push_back( g_->next[ ( i + g_->next_indice0) % NB_PIECES_SUIVANTES_VISIBLES] );
    }
  return v;
}

///
// Retourne la liste des unités actuellement en jeu.
//
std::vector<unite> Api::unites()
{
  std::vector<unite> out;
  std::map<position, unite>::const_iterator itr;
  std::map<position, unite> *map = &(g_->map_u);
  for(itr = map->begin(); itr != map->end(); ++itr){
    out.push_back(itr->second);
  }
  return out;
}

#define POSITION_VALIDE(p) if (p.x < 0 || p.y < 0 || p.x >= TAILLE_TERRAIN || p.y >= TAILLE_TERRAIN) return POSITION_INVALIDE;
#define UNITE_IN(p) if (!g_->contains_unite(p)) return POSITION_INVALIDE;
#define PAS_UNITE_IN(p) if (g_->contains_unite(p)) return POSITION_INVALIDE;

///
// Déplace une unité vers une position à portée.
//
erreur Api::deplacer(position cible, position pos)
{
  UNITE_IN(cible);
  PAS_UNITE_IN(pos);
  POSITION_VALIDE(pos);
  int d = distance(cible, pos);
  unite u = g_->get_unite(cible);
  if (u.team != g_->current_player) return PAS_A_TOI;
  if (u.pa < d) return PLUS_DE_PA;
  g_->resoudreDeplacer(cible, pos);

  StechecPkt com(DEPLACER_MSG, -1);
  com.Push(5, last_order_id++, cible.x, cible.y, pos.x, pos.y);
  SendToServer(com);

  return OK;
}

///
// Achete un objet
//
erreur Api::acheter_objet(position cible, type_objet objet)
{
  // TODO
  abort();
}

///
// utilise un objet
//
erreur Api::utiliser(position attaquant, position cible)
{
  // TODO
  abort();
}

///
// coupe la partie, cette action coute 100 d'or et met fin a la partie.
//
erreur Api::achever_la_partie()
{
  if (g_->score_team[g_->current_player] < PRIX_FIN_PARTIE) return PLUS_D_ARGENT;
  g_->resoudreFinPartie();

  StechecPkt com(FINIR_MSG, -1);
  com.Push(1, last_order_id++);
  SendToServer(com);

  return OK;
}

void Api::teamSwitched()
{
  g_->team_switched();
}
