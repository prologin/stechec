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
#include "Contest.hh"
#include <algorithm>

#ifdef ASSERT
# undef ASSERT
#endif

#define ASSERT(cond, err) \
  if (!(cond)) \
    throw err;

static unsigned int last_order_id = 1;

static inline int distance(position p1, position p2)
{
  return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}



void Action::appliquer(GameData* g)
{
}
void Action::annuler(GameData *g){
  LOG3("annule une action abstraite");
}

/* ********** Action Deplacer ********** */

void ActionDeplacer::verifier(GameData *g){
    if (!g->moto_valide(id_))
	throw ID_INVALIDE;
    InternalTraineeMoto &moto = g->motos.at(id_);
    moto.reject_bad_move(from_, to_);
}

void ActionDeplacer::appliquer(GameData* g)
{
    LOG2("trainee_moto appliquer");
    InternalTraineeMoto &moto = g->motos.at(id_);
    old_len_ = moto.length();
    old_queue_ = moto.queue(from_);
    last_end_moved_ = moto.last_end_moved_;
    moto.move(from_, to_);
    new_queue_ = moto.queue(to_);
}

void ActionDeplacer::annuler(GameData* g)
{
    InternalTraineeMoto &moto = g->motos.at(id_);
    if (old_len_ == moto.len_)
	moto.move(new_queue_, old_queue_);
    else if (old_len_ == moto.len_ - 1)
    {
	moto.len_ --;
	if (last_end_moved_)
	    moto.content_.pop_front();
	else
	    moto.content_.pop_back();
    }
    else
	abort();
    // TODO len : supprimer le debut si la taille a augmentee
    moto.last_end_moved_ = last_end_moved_;
}

void ActionDeplacer::envoyer(Api* api)
{
    StechecPkt com(ACT_DEPLACER, -1);
    com.Push(7, last_order_id++, player_, id_, from_.x, from_.y, to_.x, to_.y);
    LOG3("envoyer un deplacement au serveur...");
    api->SendToServer(com);
}

ActionDeplacer*
ActionDeplacer::recevoir(const StechecPkt* pkt)
{
    position from, to;

    from.x = pkt->arg[3];
    from.y = pkt->arg[4];
    to.x = pkt->arg[5];
    to.y = pkt->arg[6];
    return new ActionDeplacer(pkt->arg[1], pkt->arg[2], from, to);
}

/* ********** Action CouperTraineeMoto ********** */

void ActionCouperTraineeMoto::verifier(GameData* g)
{
    if (!g->moto_valide(id_))
	throw ID_INVALIDE;
    g->motos[id_].reject_bad_coupe(entre_, et_);
}

void ActionCouperTraineeMoto::appliquer(GameData* g)
{
    (void) g;
}

void ActionCouperTraineeMoto::annuler(GameData* g)
{
    (void) g;
}

void ActionCouperTraineeMoto::envoyer(Api* api)
{
    StechecPkt com(ACT_COUPER_TRAINEE_MOTO, -1);
    com.Push(8, last_order_id++, player_,
	     id_, new_id_,
	     entre_.x, entre_.y,
	     et_.x, et_.y);
    LOG3("Envoyer une coupe au serveur...");
    api->SendToServer(com);
}

ActionCouperTraineeMoto*
ActionCouperTraineeMoto::recevoir(const StechecPkt* pkt)
{
    position entre, et;
    ActionCouperTraineeMoto*	result;

    entre.x = pkt->arg[4];
    entre.y = pkt->arg[5];
    et.x = pkt->arg[6];
    et.y = pkt->arg[7];
    result = new ActionCouperTraineeMoto(pkt->arg[1],
					 pkt->arg[2], entre, et);
    result->new_id_ = pkt->arg[3];
    return (result);
}

/* ********** Action Fusionner ********** */

void ActionFusionner::verifier(GameData* g)
{
    if (id1_ == id2_ || !g->moto_valide(id1_) || !g->moto_valide(id2_))
	throw ID_INVALIDE;
    g->motos[id1_].reject_bad_fusion(g->motos[id2_], pos1_, pos2_);
}

void ActionFusionner::appliquer(GameData* g)
{
    (void) g;
}

void ActionFusionner::annuler(GameData* g)
{
    (void) g;
}

void ActionFusionner::envoyer(Api* api)
{
    StechecPkt com(ACT_FUSIONNER, -1);
    com.Push(8, last_order_id++, player_,
	     id1_, id2_,
	     pos1_.x, pos1_.y,
	     pos2_.x, pos2_.x);
    LOG3("Envoyer une fusion au serveur...");
    api->SendToServer(com);
}


ActionFusionner*
ActionFusionner::recevoir(const StechecPkt* pkt)
{
    position pos1, pos2;
    pos1.x = pkt->arg[3];
    pos1.y = pkt->arg[4];
    pos2.x = pkt->arg[5];
    pos2.y = pkt->arg[6];
    return new ActionFusionner(pkt->arg[1],
			       pkt->arg[2], pos1,
			       pkt->arg[3], pos2);
}

Action* act_from_pkt(int type, const StechecPkt* pkt)
{
    switch(type)
    {
    case ACT_DEPLACER:
	return ActionDeplacer::recevoir(pkt);
    case ACT_COUPER_TRAINEE_MOTO:
	return ActionCouperTraineeMoto::recevoir(pkt);
    case ACT_FUSIONNER:
	return ActionFusionner::recevoir(pkt);
    default:
	abort();
    }
}
