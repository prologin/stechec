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
  com.Push(3, unite_, dest_.x, dest_.y);
  api->SendToServer(com);
}

void ActionCarte::appliquer(GameData* g)
{
  add_to_carte_count(g, -1);
}

void ActionCarte::annuler(GameData* g)
{
  add_to_carte_count(g, 1);
}

void ActionCarte::add_to_carte_count(GameData* g, int increment)
{
  cartes& c = g->players_cartes[player_];

  switch (type_carte_)
  {
  case DEGUISEMENT: c.deguisement += increment; break;
  case BANZAI: c.banzai += increment; break;
  case PACIFISME: c.pacifisme += increment; break;
  case SOIN: c.soin += increment; break;
  }
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
  com.Push(3, player_, unite_, nouveau_type_);
  api->SendToServer(com);
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
  com.Push(2, player_, unite_);
  api->SendToServer(com);
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
  com.Push(2, player_, unite_);
  api->SendToServer(com);
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
  com.Push(1, player_);
  api->SendToServer(com);
}
