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
  LOG3("WARNING: Action::annuler ne devrait pas etre appele");
}

/* ********** Action Deplacer ********** */

void ActionDeplacer::verifier(GameData *g){
    LOG2("ActionDeplacer::verifier()");
    if (!g->moto_valide(id_))
	throw ID_INVALIDE;
    InternalTraineeMoto &moto = g->motos.at(id_);
    moto.reject_bad_move(from_, to_);
}

void ActionDeplacer::appliquer(GameData* g)
{
    LOG2("ActionDeplacer::appliquer()");
    InternalTraineeMoto &moto = g->motos.at(id_);
    old_len_ = moto.length();
    old_queue_ = moto.queue(from_);
    last_end_moved_ = moto.last_end_moved_;
    moto.move(from_, to_);
    new_queue_ = moto.queue(to_);
}

void ActionDeplacer::annuler(GameData* g)
{
    LOG2("ActionDeplacer::annuler()");
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
    api->SendToServer(com);
}

ActionDeplacer*
ActionDeplacer::recevoir(const StechecPkt* pkt)
{
    LOG3("ActionDeplacer::recevoir()");
    position from, to;

    from.x = pkt->arg[3];
    from.y = pkt->arg[4];
    to.x = pkt->arg[5];
    to.y = pkt->arg[6];
    return new ActionDeplacer(pkt->arg[1], pkt->arg[2], from, to);
}

void ActionDeplacer::print()
{
    std::cout << "ActionDeplacer: [" << player_ << ":" << id_ << "] ";
    std::cout << "(" << from_.x << ", " << from_.y << ")";
    std::cout << "->";
    std::cout << "(" << to_.x << ", " << to_.y << ")";
    std::cout << std::endl;
}

/* ********** Action CouperTraineeMoto ********** */

void ActionCouperTraineeMoto::verifier(GameData* g)
{
    LOG2("ActionCouperTraineeMoto::verifier()");
    if (!g->moto_valide(id_))
	throw ID_INVALIDE;
    g->motos[id_].reject_bad_coupe(entre_, et_);
}

void ActionCouperTraineeMoto::appliquer(GameData* g)
{
    LOG2("ActionCouperTraineeMoto::appliquer()");
    InternalTraineeMoto&	moto1 = g->motos[id_];
    InternalTraineeMoto*	pmoto2;

    moto1.couper(entre_, et_, &pmoto2);
    new_id_ = pmoto2->id_;
}

void ActionCouperTraineeMoto::annuler(GameData* g)
{
    LOG2("ActionCouperTraineeMoto::annuler()");
    InternalTraineeMoto&	moto1 = g->motos[id_];
    InternalTraineeMoto&	moto2 = g->motos[new_id_];

    moto1.fusionner(moto2, entre_, et_);
}

void ActionCouperTraineeMoto::envoyer(Api* api)
{
    LOG3("ActionCouperTraineeMoto::envoyer()");
    StechecPkt com(ACT_COUPER_TRAINEE_MOTO, -1);
    com.Push(7, last_order_id++, player_,
	     id_,
	     entre_.x, entre_.y,
	     et_.x, et_.y);
    api->SendToServer(com);
}

ActionCouperTraineeMoto*
ActionCouperTraineeMoto::recevoir(const StechecPkt* pkt)
{
    LOG3("ActionCouperTraineeMoto::recevoir()");
    position entre, et;
    ActionCouperTraineeMoto*	result;

    entre.x = pkt->arg[3];
    entre.y = pkt->arg[4];
    et.x = pkt->arg[5];
    et.y = pkt->arg[6];
    result = new ActionCouperTraineeMoto(pkt->arg[1],
					 pkt->arg[2], entre, et);
    return (result);
}

void ActionCouperTraineeMoto::print()
{
    std::cout << "ActionCouperTraineeMoto: [" << player_ << ":" << id_ << "] ";
    std::cout << "(" << entre_.x << ", " << entre_.y << ")";
    std::cout << "<->";
    std::cout << "(" << et_.x << ", " << et_.y << ")";
    std::cout << std::endl;
}

/* ********** Action Fusionner ********** */

void ActionFusionner::verifier(GameData* g)
{
    LOG2("ActionFusionner::verifier()");
    if (id1_ == id2_ || !g->moto_valide(id1_) || !g->moto_valide(id2_))
	throw ID_INVALIDE;
    g->motos[id1_].reject_bad_fusion(g->motos[id2_], pos1_, pos2_);
}

void ActionFusionner::appliquer(GameData* g)
{
    LOG2("ActionFusionner::appliquer()");
    InternalTraineeMoto&	moto1 = g->motos[id1_];
    InternalTraineeMoto&	moto2 = g->motos[id2_];

    moto1.fusionner(moto2, pos1_, pos2_);
}

void ActionFusionner::annuler(GameData* g)
{
    LOG2("ActionFusionner::annuler()");
    InternalTraineeMoto&	moto1 = g->motos[id1_];
    InternalTraineeMoto*	pmoto2;

    moto1.couper(pos1_, pos2_, &pmoto2);
    id2_ = pmoto2->id_;
}

void ActionFusionner::envoyer(Api* api)
{
    LOG3("ActionFusionner::envoyer()");
    StechecPkt com(ACT_FUSIONNER, -1);
    com.Push(8, last_order_id++, player_,
	     id1_, id2_,
	     pos1_.x, pos1_.y,
	     pos2_.x, pos2_.y);
    api->SendToServer(com);
}


ActionFusionner*
ActionFusionner::recevoir(const StechecPkt* pkt)
{
    LOG3("ActionFusionner::recevoir()");
    position pos1, pos2;

    pos1.x = pkt->arg[4];
    pos1.y = pkt->arg[5];
    pos2.x = pkt->arg[6];
    pos2.y = pkt->arg[7];
    return new ActionFusionner(pkt->arg[1],
			       pkt->arg[2], pos1,
			       pkt->arg[3], pos2);
}

void ActionFusionner::print()
{
    std::cout << "ActionFusionner: [" << player_ << ":" << id1_ << "<->" << id2_ << "] ";
    std::cout << "(" << pos1_.x << ", " << pos1_.y << ")";
    std::cout << "<->";
    std::cout << "(" << pos2_.x << ", " << pos2_.y << ")";
    std::cout << std::endl;
}

/* ********** Action Enrouler ********** */

void ActionEnrouler::verifier(GameData* g)
{
    LOG2("ActionEnrouler::verifier()");
    if (!g->moto_valide(id_))
	throw ID_INVALIDE;
    g->motos[id_].reject_bad_enroule(point_);
}

void ActionEnrouler::appliquer(GameData* g)
{
    LOG2("ActionEnrouler::appliquer()");
    g->motos[id_].enrouler(point_, data_);
}

void ActionEnrouler::annuler(GameData* g)
{
    LOG2("ActionEnrouler::annuler()");
    g->motos[id_].load_data(data_);
}

void ActionEnrouler::envoyer(Api* api)
{
    LOG3("ActionEnrouler::envoyer()");
    StechecPkt com(ACT_ENROULER, -1);
    com.Push(5, last_order_id++, player_,
	     id_,
	     point_.x, point_.y);
    api->SendToServer(com);
}


ActionEnrouler*
ActionEnrouler::recevoir(const StechecPkt* pkt)
{
    LOG2("ActionEnrouler::recevoir()");
    position point;

    point.x = pkt->arg[3];
    point.y = pkt->arg[4];
    return new ActionEnrouler(pkt->arg[1],
			      pkt->arg[2], point);
}

void ActionEnrouler::print()
{
    std::cout << "ActionEnrouler: [" << player_ << ":" << id_ << "] ";
    std::cout << "(" << point_.x << ", " << point_.y << ")";
    std::cout << std::endl;
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
    case ACT_ENROULER:
	return ActionEnrouler::recevoir(pkt);
    default:
	abort();
    }
}
