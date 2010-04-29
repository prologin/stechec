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

#ifndef GAMEDATA_HH_
# define GAMEDATA_HH_

# include "Contest.hh"
# include "Constant.hh"

# include "Actions.hh"

# include <vector>

class Action;

/*!
** This class is meant to contain all data, accessible from
** everywhere.
*/
class GameData: public StechecGameData
{
public:
  GameData();
  virtual ~GameData() {}

  // Call this before everything else.
  void Init();
  void check();

  void check(const char * file, int line);

  int nbr_toons(bool ennemi);

  bool can_active(int i);

  // turn
  int get_current_player();
  bool mon_tour();
  int get_real_turn();

  cartes get_cartes(int i);

  // accessors
  int indice_of(unite u);
  int indice_at(position p); // -1 if not found
  unite get_unite(int i);
  std::vector<unite> get_unites();

  taille_terrain get_tt();

  // rules
  position spawn_pos();
  position spawn_position(bool white);
  caracs caracteristiques(type_unite u);
  int porte_attaque(const unite u);
  int pa(const type_unite u);
  bool must_remove_ko();

  // side effects
  void team_switched();
  void reset_unite(unite &u, bool reset_ko);
  void set_deja_bougee(int i);
  void reset_moves();
  bool out_map(position p);

  // actions
  std::vector<Action*> actions;
  void appliquer_action(Action* act);
  void send_actions();
  bool annuler();

  void retrecissement();
  bool retirer_ko(int i);
  // data
  int current_player;
  int nbr_unites_allowed;
  bool can_play_card;
  bool deja_bougee[NBR_MAX_UNITES * 2];
  cartes players_cartes[2];
  taille_terrain tt;
  std::vector<unite> unites;
  int nbr_toons_spawn[2];
  // server internal // pourquoi ne pas mettre des actions ici ? il est trop tard maintenant, mais bon...
  std::vector<std::vector<int> > packets;

private:
  bool initialized_;
};

#endif // !GAMEDATA_HH_
