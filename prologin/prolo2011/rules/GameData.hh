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

#ifndef GAMEDATA_HH_
# define GAMEDATA_HH_

# include "Contest.hh"
# include "Constant.hh"
# include "InternalTraineeMoto.hh"
# include "Actions.hh"

class Action;


/*!
** This class is meant to contain all data, accessible from
** everywhere.
*/
class GameData: public StechecGameData
{
public:
  GameData();
  // Call this before everything else.
  void Init();
  void check(const char * file, int line);

  // side effects
  void team_switched();

  // actions
  std::vector<Action*> actions;
  void appliquer_action(Action* act);
  void send_actions();
  bool annuler();

  // turn
  int get_current_player();
  bool mon_tour();
  int mon_equipe();
  int get_real_turn();


  bool isMatchFinished();


  // data
  bool can_play;
  int current_player;
  // server internal // pourquoi ne pas mettre des actions ici ? il est trop tard maintenant, mais bon...
  std::vector<std::vector<int> > packets;

  std::vector<InternalTraineeMoto> motos;

private:
  bool initialized_;

};

#endif // !GAMEDATA_HH_
