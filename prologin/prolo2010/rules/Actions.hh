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
  bool can_play_card_;
  int nbr_unites_allowed_;
};

#define ACTIONS \
  virtual void appliquer(GameData* g); \
  virtual void envoyer(Api* api); \
  virtual void annuler(GameData* g); \
  virtual void verifier(GameData* g);

class ActionDeplacer : public Action
{
public:
  ActionDeplacer(int player, int unite, position dest) :
    Action(player), unite_(unite), dest_(dest)
  {
  }

  ACTIONS

protected:
  int unite_;
  position dest_;

  position old_pos_;
};

class ActionSpawn : public Action
{
public:
  ActionSpawn(int player, type_unite tu) :
    Action(player), tu_(tu)
  {
  }

  ACTIONS

protected :
  type_unite tu_;
};

class ActionAttaquer : public Action
{
public:
  ActionAttaquer(int player, int attaquant, int victime) :
    Action(player), attaquant_(attaquant), victime_(victime), gratuite(false)
  {
  }

  ACTIONS

protected:
  int attaquant_;
  int victime_;
  bool gratuite;
  std::vector<unite> u_;

};

class ActionRelever : public Action
{
public :
  ActionRelever(int player, int unite) :
    Action(player), unite_(unite)
  {
  }

  ACTIONS

protected :
  int unite_;
  int old_ko_;
};


class ActionCarte : public Action
{
public:
  enum Type
  {
    DEGUISEMENT = 0,
    BANZAI,
    PACIFISME,
    SOIN
  };

  ActionCarte(int player, Type type_carte) :
    Action(player), type_carte_(type_carte)
  {
  }

  virtual ~ActionCarte() {}

  virtual void appliquer(GameData* g);
  virtual void annuler(GameData* g);
  virtual void verifier(GameData* g);

protected:
  Type type_carte_;

  void add_to_carte_count(GameData* g, int p, int increment);
};

class ActionDeguisement : public ActionCarte
{
public:
  ActionDeguisement(int player, int unite, type_unite nouveau_type) :
    ActionCarte(player, ActionCarte::DEGUISEMENT),
    unite_(unite), nouveau_type_(nouveau_type)
  {
  }

  virtual ~ActionDeguisement() {}
  
  ACTIONS

protected:
  int unite_;
  type_unite ancien_type_;  // initialized when the action is actually applied.
  int old_pa_;
  type_unite nouveau_type_;
};

class ActionBanzai : public ActionCarte
{
public:
  ActionBanzai(int player, int unite) :
    ActionCarte(player, ActionCarte::BANZAI),
    unite_(unite)
  {
  }

  virtual ~ActionBanzai() {}

  ACTIONS

protected:
  int unite_;
};

class ActionSoin : public ActionCarte
{
public:
  ActionSoin(int player, int unite) :
    ActionCarte(player, ActionCarte::SOIN),
    unite_(unite)
  {
  }

  virtual ~ActionSoin() {}
  
  ACTIONS

protected:
  int unite_;
  int old_pa_;
  int old_attaques_;
  int old_ko_;
};

class ActionPacifisme : public ActionCarte
{
public:
  ActionPacifisme(int player) :
    ActionCarte(player, ActionCarte::PACIFISME)
  {
  }

  virtual ~ActionPacifisme() {}
  
  ACTIONS

protected:
  std::vector<int> old_attaques_;
};

Action* act_from_pkt(int type, const StechecPkt* pkt);

#endif
