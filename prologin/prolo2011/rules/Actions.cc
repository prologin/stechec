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
#include "Utils.hh"

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

static inline std::vector<int> make_int_vect(int size, ...)
{
    va_list va;
    std::vector<int> ret;

    va_start(va, size);
    while (size--)
    {
        ret.push_back(va_arg(va, int));
    }
    va_end(va);

    return ret;
}

void Action::appliquer(GameData* g)
{
}
void Action::annuler(GameData *g){
  LOG3("WARNING: Action::annuler ne devrait pas etre appele");
}

void Action::envoyer(Api* api)
{
    std::vector<int> params = this->serialiser();
    StechecPkt pkt(this->type(), -1);

    pkt.Push(1, last_order_id++);
    for (std::vector<int>::const_iterator it = params.begin();
            it != params.end(); ++it)
    {
        pkt.Push(1, *it);
    }

    api->SendToServer(pkt);
}

void Action::verifier_pa(GameData *g, int pa)
{
    if (!g->poll_pa(pa))
	throw PLUS_DE_PA;
}

/* ********** Action Deplacer ********** */

void ActionDeplacer::verifier(GameData *g){
    LOG2("ActionDeplacer::verifier()");
    verifier_pa(g, 1);
    if (!g->moto_valide(id_))
	throw ID_INVALIDE;
    InternalTraineeMoto &moto = g->motos.at(id_);
    if (player_ != moto.player_)
	throw PAS_A_TOI;
    moto.reject_bad_move(from_, to_);
}

void ActionDeplacer::appliquer(GameData* g)
{
    LOG2("ActionDeplacer::appliquer()");
    g->take_pa(1);
    InternalTraineeMoto &moto = g->motos.at(id_);
    old_len_ = moto.length();
    old_queue_ = moto.queue(from_);
    last_end_moved_ = moto.last_end_moved_;
    moto.move(from_, to_, taken_bonus_);
    new_queue_ = moto.queue(to_);
}

void ActionDeplacer::annuler(GameData* g)
{
    LOG2("ActionDeplacer::annuler()");
    g->give_pa(1);
    type_bonus		trash;
    InternalTraineeMoto &moto = g->motos.at(id_);

    if (old_len_ == moto.len_)
	moto.move(new_queue_, old_queue_, trash);
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
    g->get_case(to_).bonus = taken_bonus_;
}

std::vector<int> ActionDeplacer::serialiser()
{
    return make_int_vect(6, player_, id_, from_.x, from_.y, to_.x, to_.y);
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
    verifier_pa(g, 1);
    if (!g->moto_valide(id_))
	throw ID_INVALIDE;
    InternalTraineeMoto& moto = g->motos[id_];
    if (player_ != moto.player_)
	throw PAS_A_TOI;
    moto.reject_bad_coupe(entre_, et_);
}

void ActionCouperTraineeMoto::appliquer(GameData* g)
{
    LOG2("ActionCouperTraineeMoto::appliquer()");
    g->take_pa(1);
    InternalTraineeMoto&	moto1 = g->motos[id_];
    InternalTraineeMoto*	pmoto2;

    moto1.couper(entre_, et_, &pmoto2);
    new_id_ = pmoto2->id_;
}

void ActionCouperTraineeMoto::annuler(GameData* g)
{
    LOG2("ActionCouperTraineeMoto::annuler()");
    g->give_pa(1);
    InternalTraineeMoto&	moto1 = g->motos[id_];
    InternalTraineeMoto&	moto2 = g->motos[new_id_];

    moto1.fusionner(moto2, entre_, et_);
}

std::vector<int> ActionCouperTraineeMoto::serialiser()
{
    return make_int_vect(6, player_, id_, entre_.x, entre_.y, et_.x, et_.y);
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
    verifier_pa(g, 1);
    if (id1_ == id2_ || !g->moto_valide(id1_) || !g->moto_valide(id2_))
	throw ID_INVALIDE;
    InternalTraineeMoto& moto = g->motos[id1_];
    if (player_ != moto.player_)
	throw PAS_A_TOI;
    moto.reject_bad_fusion(g->motos[id2_], pos1_, pos2_);
}

void ActionFusionner::appliquer(GameData* g)
{
    LOG2("ActionFusionner::appliquer()");
    g->take_pa(1);
    InternalTraineeMoto&	moto1 = g->motos[id1_];
    InternalTraineeMoto&	moto2 = g->motos[id2_];

    moto1.fusionner(moto2, pos1_, pos2_);
}

void ActionFusionner::annuler(GameData* g)
{
    LOG2("ActionFusionner::annuler()");
    g->give_pa(1);
    InternalTraineeMoto&	moto1 = g->motos[id1_];
    InternalTraineeMoto*	pmoto2;

    moto1.couper(pos1_, pos2_, &pmoto2);
    id2_ = pmoto2->id_;
}

std::vector<int> ActionFusionner::serialiser()
{
    return make_int_vect(7, player_, id1_, id2_, pos1_.x, pos1_.y,
                         pos2_.x, pos2_.y);
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
    verifier_pa(g, 1);
    if (!g->moto_valide(id_))
	throw ID_INVALIDE;
    InternalTraineeMoto& moto = g->motos[id_];
    if (player_ != moto.player_)
	throw PAS_A_TOI;
    moto.reject_bad_enroule(point_);
}

void ActionEnrouler::appliquer(GameData* g)
{
    LOG2("ActionEnrouler::appliquer()");
    g->take_pa(1);
    g->motos[id_].enrouler(point_, data_);
}

void ActionEnrouler::annuler(GameData* g)
{
    LOG2("ActionEnrouler::annuler()");
    g->give_pa(1);
    g->motos[id_].load_data(data_);
}

std::vector<int> ActionEnrouler::serialiser()
{
    return make_int_vect(4, player_, id_, point_.x, point_.y);
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

/* ********** Action RegenererSourceEnergie ********** */

void ActionRegenererSourceEnergie::verifier(GameData* g)
{
    LOG2("ActionRegenererSourceEnergie::verifier()");
    verifier_pa(g, 1);
    if (!g->source_valide(id_))
	throw ID_INVALIDE;
    if (!g->joueurs[player_].is_able(BONUS_REGENERATION))
	throw BONUS_INVALIDE;
}

void ActionRegenererSourceEnergie::appliquer(GameData* g)
{
    LOG2("ActionRegenererSourceEnergie::appliquer()");
    g->take_pa(1);
    old_potentiel_ = g->sources[id_].regenerer();
    g->joueurs[player_].use_capacity(BONUS_REGENERATION);
}

void ActionRegenererSourceEnergie::annuler(GameData* g)
{
    LOG2("ActionRegenererSourceEnergie::annuler()");
    g->give_pa(1);
    g->sources[id_].reset(old_potentiel_);
    g->joueurs[player_].bonus.push_back(BONUS_REGENERATION);
}

std::vector<int> ActionRegenererSourceEnergie::serialiser()
{
    return make_int_vect(2, player_, id_);
}

ActionRegenererSourceEnergie*
ActionRegenererSourceEnergie::recevoir(const StechecPkt* pkt)
{
    LOG2("ActionRegenererSourceEnergie::recevoir()");

    return new ActionRegenererSourceEnergie(pkt->arg[1],
					    pkt->arg[2]);
}

void ActionRegenererSourceEnergie::print()
{
    std::cout << "ActionRegenererSourceEnergie: [" << id_ << "] ";
    std::cout << std::endl;
}

/* ********** Action AllongerPA ********** */

void ActionAllongerPA::verifier(GameData* g)
{
    LOG2("ActionAllongerPA::verifier()");
    verifier_pa(g, 1);
    if (!g->joueurs[player_].is_able(PLUS_PA))
	throw BONUS_INVALIDE;
}

void ActionAllongerPA::appliquer(GameData* g)
{
    LOG2("ActionAllongerPA::appliquer()");
    g->take_pa(1);
    g->joueurs[player_].use_capacity(PLUS_LONG);
    g->give_pa(AJOUT_PA);
}

void ActionAllongerPA::annuler(GameData* g)
{
    LOG2("ActionAllongerPA::annuler()");
    g->give_pa(1);
    g->take_pa(AJOUT_PA);
    g->joueurs[player_].bonus.push_back(PLUS_LONG);
}

std::vector<int> ActionAllongerPA::serialiser()
{
    return make_int_vect(1, player_);
}

ActionAllongerPA*
ActionAllongerPA::recevoir(const StechecPkt* pkt)
{
    LOG2("ActionAllongerPA::recevoir()");

    return new ActionAllongerPA(pkt->arg[1]);
}

void ActionAllongerPA::print()
{
    std::cout << "ActionAllongerPA: nothing :-)";
    std::cout << std::endl;
}

/* ********** Action AgrandirTraineeMoto ********** */

void ActionAgrandirTraineeMoto::verifier(GameData* g)
{
    LOG2("ActionAgrandirTraineeMoto::verifier()");
    verifier_pa(g, 1);
    if (!g->joueurs[player_].is_able(PLUS_LONG))
	throw BONUS_INVALIDE;
    if (!g->moto_valide(id_))
	throw ID_INVALIDE;
    if (longueur_ < 0 || longueur_ >= MAX_ALLONGEMENT)
	throw 100; /* TODO: introduce a meaningful error code */
}

void ActionAgrandirTraineeMoto::appliquer(GameData* g)
{
    LOG2("ActionAgrandirTraineeMoto::appliquer()");
    g->take_pa(1);
    g->joueurs[player_].use_capacity(PLUS_LONG);
    g->motos[id_].len_ += longueur_;
}

void ActionAgrandirTraineeMoto::annuler(GameData* g)
{
    LOG2("ActionAgrandirTraineeMoto::annuler()");
    g->give_pa(1);
    g->joueurs[player_].bonus.push_back(PLUS_LONG);
    g->motos[id_].len_ -= longueur_;
}

std::vector<int> ActionAgrandirTraineeMoto::serialiser()
{
    return make_int_vect(3, player_, id_, longueur_);
}

ActionAgrandirTraineeMoto*
ActionAgrandirTraineeMoto::recevoir(const StechecPkt* pkt)
{
    LOG2("ActionAgrandirTraineeMoto::recevoir()");

    return new ActionAgrandirTraineeMoto(pkt->arg[1],
					 pkt->arg[2],
					 pkt->arg[3]);
}

void ActionAgrandirTraineeMoto::print()
{
    std::cout << "ActionAgrandirTraineeMoto: [" << id_ << "] + "
	      << longueur_;
    std::cout << std::endl;
}

/* ********** Action PoserPointCroisement ********** */

void ActionPoserPointCroisement::verifier(GameData* g)
{
    LOG2("ActionPoserPointCroisement::verifier()");
    verifier_pa(g, 1);
    if (!g->joueurs[player_].is_able(BONUS_CROISEMENT))
	throw BONUS_INVALIDE;
    if (position_invalide(point_.x, point_.y))
	throw POSITION_INVALIDE;
    Case& c = g->get_case(point_);
    if (c.type != VIDE)
	throw POSITION_INVALIDE;
}

void ActionPoserPointCroisement::appliquer(GameData* g)
{
    LOG2("ActionPoserPointCroisement::appliquer()");
    g->take_pa(1);
    g->joueurs[player_].use_capacity(BONUS_CROISEMENT);
    Case& c = g->get_case(point_);
    old_type_ = c.type;
    c.type = POINT_CROISEMENT;
}

void ActionPoserPointCroisement::annuler(GameData* g)
{
    LOG2("ActionPoserPointCroisement::annuler()");
    g->give_pa(1);
    Case& c = g->get_case(point_);
    c.type = old_type_;
}

std::vector<int> ActionPoserPointCroisement::serialiser()
{
    return make_int_vect(3, player_, point_.x, point_.y);
}

ActionPoserPointCroisement*
ActionPoserPointCroisement::recevoir(const StechecPkt* pkt)
{
    LOG2("ActionPoserPointCroisement::recevoir()");

    position point;
    point.x = pkt->arg[2];
    point.y = pkt->arg[3];
    return new ActionPoserPointCroisement(pkt->arg[1],
					  point);
}

void ActionPoserPointCroisement::print()
{
    std::cout << "ActionPoserPointCroisement: ("
	      << point_.x << ", " << point_.y << ")";
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
    case ACT_REGENERER:
	return ActionRegenererSourceEnergie::recevoir(pkt);
    case ACT_ALLONGER_PA:
	return ActionAllongerPA::recevoir(pkt);
    case ACT_AGRANDIR:
	return ActionAgrandirTraineeMoto::recevoir(pkt);
    case ACT_POSER_PT_CROIX:
	return ActionPoserPointCroisement::recevoir(pkt);
    default:
	abort();
    }
}
