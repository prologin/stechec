/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2011 Prologin
*/

#ifndef INTERNAL_TRAINEE_MOTO_HH_
#define INTERNAL_TRAINEE_MOTO_HH_

#include "Constant.hh"
#include <deque>

class GameData;

class InternalTraineeMoto
{
public:
  InternalTraineeMoto(GameData* gd, int player, position init, int max_len);
  erreur move(position from, position to);
  void reject_bad_move(position from, position to);
  bool begin(position pos);
  bool end(position pos);

  trainee_moto to_trainee_moto(int indice);

  int length();
  position queue(position head);
  position head();
  position queue();

private:
  GameData* gd_;
  std::deque<position> content_;
  int len_;
  int max_len_;
  int player_;
};

#endif // !INTERNAL_TRAINEE_MOTO_HH_
