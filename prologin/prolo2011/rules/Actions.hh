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

#ifndef ACTIONS_HH_
# define ACTIONS_HH_

# include "Api.hh"
# include "Contest.hh"
# include "Constant.hh"
# include "GameData.hh"

#include "InternalTraineeMoto.hh"

# include <map>
# include <vector>

class Action
{
public:
  Action(int player) : player_(player) {}

  virtual ~Action() {}

  virtual void appliquer(GameData* g);
  virtual void envoyer(Api* api) = 0;
  virtual void annuler(GameData* g);
  virtual void verifier(GameData* g) = 0;

protected:
  int player_;
};

#define ACTIONS \
  virtual void appliquer(GameData* g); \
  virtual void envoyer(Api* api); \
  virtual void annuler(GameData* g); \
  virtual void verifier(GameData* g);

class ActionDeplacer : public Action
{
public:
  ActionDeplacer(int player, int id, position from, position to) :
    Action(player), id_(id), from_(from), to_(to)
  {
  }

  ACTIONS

protected:
  position from_;
  position to_;
  int id_;
  int old_len_;
  position old_queue_;
  position new_queue_;
};

Action* act_from_pkt(int type, const StechecPkt* pkt);

#endif
