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

  bool mon_tour();

  int nbr_unites_activees();
  int nbr_toons(bool ennemi);

  bool can_active(int i);
  int toon_at(position p); // returns the index of the toon at the position p. -1 when there is no toon

  int get_current_player();

  cartes get_cartes(int i);
  bool can_play_card();

  int indice_of(unite u);
  unite get_unite(int i);
  std::vector<unite> get_unites();

  taille_terrain get_tt();

  // rules
  position spawn_pos();
  position spawn_position(bool white);
  int porte_attaque(const unite u);
  int pa(const type_unite u);

  // side effects
  void appliquer_action(Actions a);
  void team_switched();
  bool annuler();

  private:

  bool initialized_;

  // for cancel, and to register the diff
  std::vector<Actions> actions;
  GameData *at_init;

  bool deja_bougee[18];
  cartes players_cartes[2];
  int current_player;
  taille_terrain tt;
  bool data_can_play_card;

  std::vector<unite> data_unites;

  // side effects
  void reset_unite(unite &u, bool reset_ko);
  void set_deja_bougee(int i);
  void do_deplacer(int indice, position pos, int pa);
  void do_spawn(unite u);
  void reset_moves();

};

#endif // !GAMEDATA_HH_
