/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2010 Prologin
*/

#include "GameData.hh"
#include "ServerEntry.hh"
#include "misc/Conf.hh"

ServerEntry::ServerEntry(GameData* game, Server* server,
                         const ConfSection* cfg)
  : StechecServerEntry(game, server, cfg)
{
}

ServerEntry::~ServerEntry(void)
{
}


int		ServerEntry::beforeGame(void)
{

  g_->Init();
  for (int i = 0; i < NB_PIECES_SUIVANTES_VISIBLES; i ++) {
    g_->push_random_piece(i / NB_PIECES_SUIVANTES_VISIBLES_PAR_TOUR + 1);
    StechecPkt com(NEXT_PIECE_MSG, -1);
    com.Push(5, g_->next[i].tour_apparition, g_->next[i].valeur, g_->next[i].pos_piece.x, g_->next[i].pos_piece.y);
    SendToAll(com);
  }

  return 0;
}

int         ServerEntry::initGame(void)
{
  return 0;
}

int         ServerEntry::beforeNewTurn(void)
{
  g_->team_switched(true);
  return 0;
}

#define ARG2(a, i) a[(i)], a[(i) + 1]
#define ARG4(a, i) ARG2(a, i), ARG2(a, i + 2)
#define ARG8(a, i) ARG4(a, i), ARG4(a, i + 4)
int         ServerEntry::afterNewTurn(void)
{
  if (g_->match_finished()) return 0;
  // forward the actions
  for (std::vector<std::vector<int> >::iterator it = g_->packets.begin();
       it != g_->packets.end(); ++it)
  {
    SendToAll((*it)[0], -1, 8, ARG8((*it), 1));
  }
  if (g_->getCurrentTurn() % 2 == 0){
    int tour = g_->get_real_turn() + 1 + NB_PIECES_SUIVANTES_VISIBLES / NB_PIECES_SUIVANTES_VISIBLES_PAR_TOUR;
    for (int i = 0; i < NB_PIECES_SUIVANTES_VISIBLES_PAR_TOUR; i ++){
      g_->apparition_piece();
      piece p = g_->push_random_piece(tour);
      LOG2("PUSH PIECES MSG !! %1 - %2", p.tour_apparition, p.valeur);
      StechecPkt com(NEXT_PIECE_MSG, -1);
      com.Push(4, p.tour_apparition, p.valeur, p.pos_piece.x, p.pos_piece.y);
      SendToAll(com);
    }
  }
  return 0;
}


int         ServerEntry::afterGame(void)
{
  // TODO
  return 0;
}

bool        ServerEntry::isMatchFinished(void)
{
  return g_->match_finished();
}

int ServerEntry::getScore(int uid)
{
  return g_->score(uid);
}
