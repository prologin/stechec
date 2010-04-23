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

#include "Actions.hh"

#ifdef ASSERT
# undef ASSERT
#endif

#define ASSERT(cond, err) \
  if (!(cond)) \
    throw err;

static unsigned int last_order_id = 0;

static inline int max(int a, int b)
{
  return (a > b) ? a : b;
}

static inline int distance(position p1, position p2)
{
  return max(abs(p1.x - p2.x), abs(p1.y - p2.y));
}

void ActionSpawn::up_position(GameData *g){
  if (p_.x == -1) p_ = g->spawn_pos();
}

void ActionSpawn::verifier(GameData* g)
{
  up_position(g);
  int i =  g->indice_at(p_);
  ASSERT(i == -1, SPAWN_OCCUPE);
  ASSERT(tu_ != PERROQUET, PAS_SPAWNABLE);
  ASSERT(g->nbr_toons(false) < NBR_MAX_UNITES / 2, PAS_SPAWNABLE);
}

void ActionSpawn::appliquer(GameData *g)
{
  up_position(g);
  {
    unite u = {
      p_,
      false,
      tu_,
      tu_,
      -1,
      0,
      0
    };
    g->deja_bougee[g->unites.size()] = true;
    g->unites.push_back(u);
  }
}

void ActionSpawn::annuler(GameData *g)
{
  g->unites.pop_back();
}

void ActionSpawn::envoyer(Api *api)
{
  StechecPkt com(ACT_SPAWN, -1);
  com.Push(5, last_order_id++, player_, tu_);
  api->SendToServer(com);
}

void ActionDeplacer::verifier(GameData* g)
{
  ASSERT(unite_ >= 0, POSITION_INVALIDE);
  ASSERT(unite_ < g->get_unites().size(), POSITION_INVALIDE);
  
  unite& u = g->unites[unite_];
  ASSERT(u.ko < 0, UNITE_KO);
  ASSERT(!u.ennemi, PAS_A_MOI);

  ASSERT(g->indice_at(dest_) == -1, CASE_OCCUPEE);

  int dist = distance(u.pos, dest_);
  ASSERT(u.pa >= dist, PLUS_DE_PA);
}

void ActionDeplacer::appliquer(GameData* g)
{
  unite& u = g->unites[unite_];
  
  old_nbr_unites_allowed_ = g->nbr_unites_allowed;
  if (!g->deja_bougee[unite_])
  {
    g->nbr_unites_allowed -= 1;
    g->deja_bougee[unite_] = true;
  }

  old_pos_ = u.pos;
  u.pos = dest_;
}

void ActionDeplacer::annuler(GameData* g)
{
  if (g->nbr_unites_allowed != old_nbr_unites_allowed_)
    g->deja_bougee[unite_] = false;

  g->nbr_unites_allowed = old_nbr_unites_allowed_;
  g->unites[unite_].pos = old_pos_;
}

void ActionDeplacer::envoyer(Api* api)
{
  StechecPkt com(ACT_DEPLACER, -1);
  com.Push(5, last_order_id++, player_, unite_, dest_.x, dest_.y);
  api->SendToServer(com);
}

void ActionCarte::verifier(GameData* g)
{
  ASSERT(g->can_play_card, PHASE_CARTES_TERMINEE);

  cartes& c = g->players_cartes[player_];
  int cnt;

  switch (type_carte_)
  {
  case DEGUISEMENT: cnt = c.deguisement; break;
  case BANZAI: cnt = c.banzai; break;
  case PACIFISME: cnt = c.pacifisme; break;
  case SOIN: cnt = c.soin; break;
  }

  ASSERT(cnt > 0, PLUS_DE_CARTES);
}

void ActionCarte::appliquer(GameData* g)
{
  add_to_carte_count(g, player_, -1);
  add_to_carte_count(g, player_ ^ 1, 1);
}

void ActionCarte::annuler(GameData* g)
{
  add_to_carte_count(g, player_, 1);
  add_to_carte_count(g, player_ ^ 1, -1);
}

void ActionCarte::add_to_carte_count(GameData* g, int p, int increment)
{
  cartes& c = g->players_cartes[p];

  switch (type_carte_)
  {
  case DEGUISEMENT: c.deguisement += increment; break;
  case BANZAI: c.banzai += increment; break;
  case PACIFISME: c.pacifisme += increment; break;
  case SOIN: c.soin += increment; break;
  }
}

void ActionDeguisement::verifier(GameData* g)
{
  ActionCarte::verifier(g);

  ASSERT(unite_ >= 0, POSITION_INVALIDE);
  ASSERT(unite_ < g->get_unites().size(), POSITION_INVALIDE);

  ASSERT(unite_ > PERROQUET, PAS_SPAWNABLE);
  ASSERT(unite_ <= KANGOUROU, PAS_SPAWNABLE);
}

void ActionDeguisement::appliquer(GameData* g)
{
  ActionCarte::appliquer(g);

  ancien_type_ = g->unites[unite_].type_unite_actuel;
  g->unites[unite_].type_unite_actuel = nouveau_type_;
}

void ActionDeguisement::annuler(GameData* g)
{
  ActionCarte::annuler(g);

  unite& u = g->unites[unite_];
  u.type_unite_actuel = ancien_type_;
}

void ActionDeguisement::envoyer(Api* api)
{
  StechecPkt com(ACT_DEGUISEMENT, -1);
  com.Push(4, last_order_id++, player_, unite_, nouveau_type_);
  api->SendToServer(com);
}

void ActionBanzai::verifier(GameData* g)
{
  ActionCarte::verifier(g);

  ASSERT(unite_ >= 0, POSITION_INVALIDE);
  ASSERT(unite_ < g->get_unites().size(), POSITION_INVALIDE);
}

void ActionBanzai::appliquer(GameData* g)
{
  ActionCarte::appliquer(g);

  g->unites[unite_].attaques += 1;
}

void ActionBanzai::annuler(GameData* g)
{
  ActionCarte::annuler(g);

  g->unites[unite_].attaques -= 1;
}

void ActionBanzai::envoyer(Api* api)
{
  StechecPkt com(ACT_BANZAI, -1);
  com.Push(3, last_order_id++, player_, unite_);
  api->SendToServer(com);
}

void ActionSoin::verifier(GameData* g)
{
  ActionCarte::verifier(g);

  ASSERT(unite_ >= 0, POSITION_INVALIDE);
  ASSERT(unite_ < g->get_unites().size(), POSITION_INVALIDE);
}

void ActionSoin::appliquer(GameData* g)
{
  ActionCarte::appliquer(g);

  unite& u = g->unites[unite_];

  old_pa_ = u.pa;
  old_attaques_ = u.attaques;
  old_ko_ = u.ko;

  u.pa = 0;
  u.attaques = 0;
  u.ko = 0;
}

void ActionSoin::annuler(GameData* g)
{
  ActionCarte::annuler(g);

  unite& u = g->unites[unite_];

  u.pa = old_pa_;
  u.attaques = old_attaques_;
  u.ko = old_ko_;
}

void ActionSoin::envoyer(Api* api)
{
  StechecPkt com(ACT_SOIN, -1);
  com.Push(3, last_order_id++, player_, unite_);
  api->SendToServer(com);
}

void ActionPacifisme::verifier(GameData* g)
{
  ActionCarte::verifier(g);
}

void ActionPacifisme::appliquer(GameData* g)
{
  ActionCarte::appliquer(g);

  old_attaques_.resize(0);
  old_attaques_.reserve(g->unites.size());
  for (std::vector<unite>::iterator it = g->unites.begin();
       it != g->unites.end(); ++it)
  {
    old_attaques_.push_back(it->attaques);
    if (it->ennemi)
      it->attaques = 0;
  }

  g->nbr_unites_allowed += 1;
}

void ActionPacifisme::annuler(GameData* g)
{
  int i = 0;
  ActionCarte::annuler(g);

  for (std::vector<int>::const_iterator it = old_attaques_.begin();
       it != old_attaques_.end(); ++it, ++i)
  {
    g->unites[i].attaques = *it;
  }

  g->nbr_unites_allowed -= 1;
}

void ActionPacifisme::envoyer(Api* api)
{
  StechecPkt com(ACT_PACIFISME, -1);
  com.Push(2, last_order_id++, player_);
  api->SendToServer(com);
}
