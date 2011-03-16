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
** Représente une case du terrain.
*/
struct Case
{
    type_case   type;
    type_bonus  bonus;
    int         nb_trainees_moto; /*! Nombre de trainées sur la case */
    int         source_id; /*! Identifiant de la source d'énergie */

    Case();
};

struct SourceEnergie
{
    position    pos;
    int         potentiel_max;
    int         potentiel_cur;

    source_energie  to_source_energie(int indice);
    void set_potentiel(int potentiel);
};

struct Joueur
{
    int                         score;
    std::vector<type_bonus>     bonus;

    Joueur();
};

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

  // Juste pour les tests
  void generer_terrain();

  // Accès basique aux données
  Case& get_case(int x, int y);
  Case& get_case(const position &pos);

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
  void get_scores(std::vector<int>& scores);
  void get_sources(std::vector<source_energie>& srcs);
  void get_bonus_joueur(int joueur, std::vector<type_bonus>& bonus);
  int get_real_turn();
  InternalTraineeMoto& creer_trainee_moto(int player,
                                          position init,
                                          int max_len);

  bool isMatchFinished();


  // data
  bool can_play;
  int current_player;
  // server internal // pourquoi ne pas mettre des actions ici ? il est trop tard maintenant, mais bon...
  std::vector<std::vector<int> > packets;

  std::vector<Joueur>               joueurs;
  std::vector<InternalTraineeMoto>  motos;
  std::vector<SourceEnergie>        sources;

private:
  bool initialized_;

  std::vector<Case> terrain_;
};

#endif // !GAMEDATA_HH_
