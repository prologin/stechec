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

static inline int min(int a, int b)
{
  return (a < b) ? a : b;
}

static inline int distance(position p1, position p2)
{
  return max(abs(p1.x - p2.x), abs(p1.y - p2.y));
}

static int get_ko(type_unite a)
{
  int c[] = {1, 5, 3, 2};
  return c[a];
}

bool operator==(position p1, position p2){
  return p1.x == p2.x && p1.y == p2.y;
}

void ActionAttaquer::get_explosions(GameData *g, std::vector<unite *> &u, position p){
  for (int i = 0, l = g->unites.size(); i < l ; i++){
    if (g->unites[i].ko == -1 && distance(g->unites[i].pos, p) <= 2)
      {
	bool b = true;
	for (int j = 0, l = u.size(); j < l ; j++){
	  if (u[i]->pos == g->unites[i].pos) b = false;
	}
	if (b){
	  u.push_back( & g->unites[i]);
	  if (g->unites[i].type_unite_actuel == KANGOUROU){
	    get_explosions(g, u, g->unites[i].pos);
	  }
	}
      }
  }
}

void ActionAttaquer::appliquer(GameData *g)
{
  unite& a = g->unites[attaquant_];
  unite& v = g->unites[victime_];
  type_unite tu = a.type_unite_actuel;
  a.attaques -= 1;
  a.pa -= 1;
  if (tu == KANGOUROU){
    std::vector<unite *> explosions;
    explosions.push_back( & g->unites[attaquant_] );
    get_explosions(g, explosions, a.pos);
    for (int j = 0, l = explosions.size(); j < l ; j++){
      int mind = 5; // plus que 2
      for (int k = 0, l = explosions.size(); k < l ; k++){
	mind = min( mind, distance(explosions[k]->pos, explosions[j]->pos));
      }
      explosions[j]->ko = 2 - mind;
    }
    u_ = g->unites;
  }else{
    v.ko = get_ko(tu);
  }
}

void ActionAttaquer::annuler(GameData *g)
{
unite& a = g->unites[attaquant_];
  unite& v = g->unites[victime_];
  type_unite tu = a.type_unite_actuel;
  if (g->nbr_unites_allowed != nbr_unites_allowed_)
    g->deja_bougee[attaquant_] = false;
  Action::annuler(g);
  a.attaques += 1;
  a.pa += 1;
  switch (tu){
  case KANGOUROU :
    g->unites = u_;
    break;
  default:
    v.ko = -1;
  }
}

void ActionAttaquer::verifier(GameData *g)
{

  ASSERT(attaquant_ >= 0, POSITION_INVALIDE);
  ASSERT(attaquant_ < g->get_unites().size(), POSITION_INVALIDE);
  ASSERT(victime_ >= 0, POSITION_INVALIDE);
  ASSERT(victime_ < g->get_unites().size(), POSITION_INVALIDE);

  unite& a = g->unites[attaquant_];
  unite& v = g->unites[victime_];
  ASSERT(a.ko == -1, UNITE_KO);
  ASSERT(!a.ennemi, PAS_A_MOI);
  // ASSERT(v.ennemi, UNITE_KO); // TODO ajouter : UNITE_A_MOI
  ASSERT(a.pa >= 1, PLUS_DE_PA);
  ASSERT(a.attaques >+ 1, QUOTA_DEPASSE); // todo ajouter : PEUT_PLUS_ATTAQUER
}

void ActionAttaquer::envoyer(Api * api)
{
StechecPkt com(ACT_ATTAQUER, -1);
 com.Push(4, last_order_id++, player_, attaquant_, victime_);
  api->SendToServer(com);
}

void Action::appliquer(GameData* g)
{
  can_play_card_ = g->can_play_card;
  nbr_unites_allowed_ = g->nbr_unites_allowed;
}

void Action::annuler(GameData *g){
  g->can_play_card = can_play_card_;
  g->nbr_unites_allowed = nbr_unites_allowed_;
}

void ActionRelever::verifier(GameData* g)
{
  ASSERT(unite_ >= 0, POSITION_INVALIDE);
  ASSERT(unite_ < g->get_unites().size(), POSITION_INVALIDE);
  unite& u = g->unites[unite_];
  ASSERT(u.ko >= 0, UNITE_KO);
  ASSERT(u.ko == 0, UNITE_DEBOUT);
  ASSERT(!u.ennemi, PAS_A_MOI);
  ASSERT(g->can_active(unite_), QUOTA_DEPASSE);
}

void ActionRelever::appliquer(GameData* g)
{
  Action::appliquer(g);
  old_ko_ = g->unites[unite_].ko;
  g->unites[unite_].ko = -1;
}

void ActionRelever::annuler(GameData* g)
{
  if (g->nbr_unites_allowed != nbr_unites_allowed_)
    g->deja_bougee[unite_] = false;
  Action::annuler(g);
  g->unites[unite_].ko = old_ko_;
}

void ActionRelever::envoyer(Api *api)
{
  StechecPkt com(ACT_RELEVER, -1);
  com.Push(3, last_order_id++, player_, unite_);
  api->SendToServer(com);
}

void ActionSpawn::verifier(GameData* g)
{
  position p = g->spawn_pos();
  int i =  g->indice_at(p);
  ASSERT(i == -1, SPAWN_OCCUPE);
  ASSERT(tu_ != PERROQUET, PAS_SPAWNABLE);
  ASSERT(g->nbr_toons(false) < NBR_MAX_UNITES / 2, PAS_SPAWNABLE);
}

void ActionSpawn::appliquer(GameData *g)
{
  position p = g->spawn_pos();
  Action::appliquer(g);
  {
    unite u = {
      p,
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
  g->nbr_toons_spawn[player_]++;
}

void ActionSpawn::annuler(GameData *g)
{
  Action::annuler(g);
  g->unites.pop_back();
  g->nbr_toons_spawn[player_]--;
}

void ActionSpawn::envoyer(Api *api)
{
  StechecPkt com(ACT_SPAWN, -1);
  com.Push(3, last_order_id++, player_, tu_);
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

  ASSERT(g->can_active(unite_), QUOTA_DEPASSE);
}

void ActionDeplacer::appliquer(GameData* g)
{
  Action::appliquer(g);
  unite& u = g->unites[unite_];
  
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
  if (g->nbr_unites_allowed != nbr_unites_allowed_)
    g->deja_bougee[unite_] = false;
  Action::annuler(g);
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
  g->unites[unite_].attaques_gratuites += 1;
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

Action* act_from_pkt(int type, const StechecPkt* pkt)
{
  struct position p;

  switch(type)
  {
  case ACT_DEGUISEMENT:
    return new ActionDeguisement(pkt->arg[1], pkt->arg[2], (type_unite)pkt->arg[3]);
  case ACT_BANZAI:
    return new ActionBanzai(pkt->arg[1], pkt->arg[2]);
  case ACT_SOIN:
    return new ActionSoin(pkt->arg[1], pkt->arg[2]);
  case ACT_PACIFISME:
    return new ActionPacifisme(pkt->arg[1]);
  case ACT_DEPLACER:
    p.x = pkt->arg[3];
    p.y = pkt->arg[4];
    return new ActionDeplacer(pkt->arg[1], pkt->arg[2], p);
  case ACT_SPAWN:
    return new ActionSpawn(pkt->arg[1], (type_unite)pkt->arg[2]);
  case ACT_RELEVER:
    return new ActionRelever(pkt->arg[1], pkt->arg[2]);
  case ACT_ATTAQUER:
    return new ActionAttaquer(pkt->arg[1], pkt->arg[2], pkt->arg[3]);
  default:
    LOG1("Unhandled action type : %1", type);
    abort();
  }
}
