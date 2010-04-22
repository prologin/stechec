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

  virtual void appliquer(GameData* g) = 0;
  virtual void envoyer(Api* api) = 0;
  virtual void annuler(GameData* g) = 0;
  virtual void verifier(GameData* g) = 0;

protected:
  int player_;
};

class ActionDeplacer : public Action
{
public:
  ActionDeplacer(int player, int unite, position dest) :
    Action(player), unite_(unite), dest_(dest)
  {
  }

  virtual void appliquer(GameData* g);
  virtual void envoyer(Api* api);
  virtual void annuler(GameData* g);
  virtual void verifier(GameData* g);

protected:
  int unite_;
  position dest_;

  position old_pos_;
  int old_nbr_unites_allowed_;
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
  
  virtual void appliquer(GameData* g);
  virtual void envoyer(Api* api);
  virtual void annuler(GameData* g);
  virtual void verifier(GameData* g);

protected:
  int unite_;
  type_unite ancien_type_;
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
  
  virtual void appliquer(GameData* g);
  virtual void envoyer(Api* api);
  virtual void annuler(GameData* g);
  virtual void verifier(GameData* g);

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
  
  virtual void appliquer(GameData* g);
  virtual void envoyer(Api* api);
  virtual void annuler(GameData* g);
  virtual void verifier(GameData* g);

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
  
  virtual void appliquer(GameData* g);
  virtual void envoyer(Api* api);
  virtual void annuler(GameData* g);
  virtual void verifier(GameData* g);

protected:
  std::vector<int> old_attaques_;
};

#endif
