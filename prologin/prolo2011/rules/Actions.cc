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


void ActionDeplacer::verifier(GameData *g){
  if (id_ < 0 || id_ >= g->motos.size()) throw ID_INVALIDE;
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
  if (old_len_ == moto.len_){
    moto.move(new_queue_, old_queue_);
  }else if (old_len_ == moto.len_ - 1){
    moto.len_ --;
    if (last_end_moved_){
      moto.content_.pop_front();
    }else{
      moto.content_.pop_back();
    }
  }else{
    abort();
  }
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

Action* act_from_pkt(int type, const StechecPkt* pkt)
{
  struct position from, to;
  switch(type)
  {
  case ACT_DEPLACER:
    from.x = pkt->arg[3];
    from.y = pkt->arg[4];
    to.x = pkt->arg[5];
    to.y = pkt->arg[6];
    return new ActionDeplacer(pkt->arg[1], pkt->arg[2], from, to);
  }
  abort();
}
