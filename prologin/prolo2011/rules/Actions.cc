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
}

void ActionDeplacer::appliquer(GameData* g)
{
}
void ActionDeplacer::annuler(GameData* g)
{
}

void ActionDeplacer::envoyer(Api* api)
{
  StechecPkt com(ACT_DEPLACER, -1);
  com.Push(7, last_order_id++, player_, unite_, from_.x, from_.y, to_.x, to_.y);
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
