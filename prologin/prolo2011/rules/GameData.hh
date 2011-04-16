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
# include "Utils.hh"

# include <deque>
# include <map>

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
    int		id;
    position    pos;
    int         potentiel_max;
    int         potentiel_cur;

    source_energie  to_source_energie(int indice);
    void set_potentiel(int potentiel);
    int regenerer();
    void reset(int old_potentiel);

    void consume(int degree);
    void release();
};

struct Joueur
{
    typedef std::list<type_bonus>	bonus_list;

    int		id;
    int		score;
    bonus_list	bonus;

    Joueur(int id);
    /*! Test weither a player is able to use a bonus */
    bool is_able(type_bonus	b);

    /*!
     * Remove a bonus from the players’s bonus and return OK, or return
     * BONUS_INVALIDE if there’s no such bonus.
     */
    erreur use_capacity(type_bonus	b);

protected:
    bonus_list::iterator
    get_bonus(type_bonus	b);
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

    // Accès basique aux données
    Case& get_case(int x, int y);
    Case& get_case(const position &pos);

    // Path processing
    void get_next_pos(const position& p,
		      std::vector<position>& next_pos);
    bool is_crossable_pos(const position& p);
    void build_from_reverse_path(const position& reverse_begin,
				 const position& reverse_end,
				 std::map<pair_position, position>& back_path,
				 std::vector<position>& path);


    /*
     * Build a shortest path betwee 'begin' and 'end' in 'path', or make 'path'
     * empty if there's no such path.
     */
    void get_shortest_path(const position& begin,
			   const position& end,
			   std::vector<position>& path);

    // Manipulations basiques des motos

    // Chaque moto est identifiée par un entier, et celui-ci ne doit pas
    // changer. Il arrive que des motos soit supprimées, ce qui laisse
    // apparaître des trous dans l’ensemble des identifiants. C’est uniquement
    // pour cela que std::map est le plus adapté.
    InternalTraineeMoto&
    creer_trainee_moto(int player, position init, int max_len);
    bool moto_valide(int id);
    void supprimer_moto(int id);

    bool source_valide(int id);

    // side effects
    void team_switched();

    // actions
    std::vector<Action*> actions;
    void appliquer_action(Action* act);
    void send_actions();
    bool annuler();

    /* Return if "pa" PAs are available. Return false if "pa" is negative. */
    bool poll_pa(int pa);
    /* Take "pa" PAs if poll_pa returned true and return ask_pa's result */
    bool take_pa(int pa);
    /* If "pa" is positive, add "pa" PAs */
    void give_pa(int pa);

    // turn
    int get_current_player();
    bool mon_tour();
    int mon_equipe();
    void get_scores(std::vector<int>& scores);
    void get_sources(std::vector<source_energie>& srcs);
    void get_bonus_joueur(int joueur, std::vector<type_bonus>& bonus);
    int get_real_turn();

    bool isMatchFinished();

    void stocker_action(Action* act);
    std::vector<std::vector<int> > actions_stockees;


    // data
    typedef std::map<int, InternalTraineeMoto>	motos_type;

    bool can_play;
    int current_player;
    // server internal
    // Coucou: pourquoi ne pas mettre des actions ici ? il est trop tard
    // maintenant, mais bon...
    std::vector<std::vector<int> > packets;

    std::vector<Joueur>         joueurs;
    motos_type			motos;
    std::vector<SourceEnergie>  sources;

protected:
    bool initialized_;
    int remaining_pa_;

    std::vector<Case> terrain_;

    int get_free_moto_id();

    /*
     * Look for every connection between trainees_moto and sources, compute the
     * sources potentiel’s changes and the increase the scores.
     */
    void apply_connections();

  void apply_connections_group(int id_trainee, std::vector<int> &degrees, std::set<int> &deja_traitees, int map[TAILLE_TERRAIN][TAILLE_TERRAIN][4]);
    /*
     * Look for an energy source at a (potentially invalid) position, and
     * categorize it (positive & negative).
     */
    void categorize_case(const position& p,
			 std::set<SourceEnergie*>& src_p,
			 std::set<SourceEnergie*>& src_n,
			 std::deque<InternalTraineeMoto*> &a_traiter,
			 int player,
			 int map[TAILLE_TERRAIN][TAILLE_TERRAIN][4]);
};

#endif // !GAMEDATA_HH_
