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
# include "map"

bool operator<(position p1, position p2);
int max(int a, int b);
int min(int a, int b);
int distance(position p1, position p2);

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

  std::map<position, unite> map_u;
  std::map<position, piece> map_p;
  piece next[NB_PIECES_SUIVANTES_VISIBLES];
  int next_indice0;
  int first_indice0;
  int current_player;

  bool can_move;
  int score_team[2];
  bool player_end_match;
  std::vector<std::vector<int> > packets;

  piece push_random_piece();
  void push_piece(int temps, int valeur, position p);
  void random_piece(piece & p);
  void random_position(position & p);
  bool case_occupee(const position p);
  int get_real_turn();
  int get_current_player();
  void team_switched(bool serv);
  bool mon_tour();
  void apparition_piece();
  bool match_finished();
  int score(int uid);
  void set_unite(position cible, unite u);
  unite get_unite(position cible);
  void remove_unite(position cible);
  bool contains_unite(position cible);
  void remove_piece(position cible);
  bool contains_piece(position cible);
  piece get_piece(position cible);

  caracteristiques_objet proprietes_objet(type_objet to);
  void resoudreDeplacer(position cible, position pos);
  void resoudreAcheterObjet(position cible, type_objet objet);
  void resoudreUtiliserObjet(position cible, position pos);
  void resoudreFinPartie();
  void resoudre( const e_com_type type, const int * pkt);
private:
  bool initialized_;
  void push_empty_player(const position p, const int team);
  bool filet(int x, int y);

};

#endif // !GAMEDATA_HH_
