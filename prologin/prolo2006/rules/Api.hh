/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#ifndef API_HH_
# define API_HH_

# include <Contest.hh>
# include "Client.hh"

# define TEST_POS(x, y) \
  if (x >= g_->map_size.col || y >= g_->map_size.row || y < 0 || x < 0) \
    return BAD_ARGUMENT;

# define IF_FOG(x, y) \
  if (g_->getUid() >= 0 && c_->player_fog[g_->getUid()][y][x] == 0) \
    return UNKNOWN;

# define TEST_ID_ACTION(id) \
  if (g_->p->team[id].action) \
{ LOG2("Forbidden move (bad id or it has already moved this turn)"); return BAD_ARGUMENT; }

# define ID_EXISTS(id) \
  (id >= 0 && id < (MAX_GOODMEN * g_->getNbPlayer()))

# define GET_TEAM(id)\
  (id / MAX_GOODMEN)

# define GET_GOOD(id)\
  (id % MAX_GOODMEN)

# define GOOD_FROM_IDS(player, id) \
  (id + (player * MAX_GOODMEN))

/*!
** Method of this call are called by the candidat, throught 'interface.cc'
**
** You can:
**  - access to the GameData class: _g
**  - send message to the server: void SendToServer(struct s_com&)
*/
class Api: public StechecApi
{
public:
  Api(GameData* gameData, Client* c);
  virtual ~Api() { }

protected:
  virtual void teamSwitched();

public:
   std::pair<char, short>	astar(Position *start, Position *end);
   int equipes();
   int taille_equipe();
   int mon_equipe();
   int taille_ville_x();
   int taille_ville_y();
   int distance(int x_d, int y_d, int x_a, int y_a);
   int meilleure_direction(int x_d, int y_d, int x_a, int y_a);
   int regarde(int x, int y);
   int visible(int x, int y);
   int visible_depuis(int from_x, int from_y, int to_x, int to_y);
   int visible_test(int to_x, int to_y, Position ref, Position pos);
   int score(int id);
   int current_date();
   int remaining_turn();
   int almanach_visible ();
   int position_almanach_x();
   int position_almanach_y();
   int qui_possede_almanach();
   int joueur_possede_almanach();
   int delorean_visible ();
   int position_delorean_x();
   int position_delorean_y();
   int autonomie_delorean();
   int qui_possede_delorean();
   int joueur_possede_delorean();
   int joueur_visible (int id);
   int equipe(int id);
   int position_joueur_x(int id);
   int position_joueur_y(int id);
   int etat_joueur(int id);
   int casino(int x, int y);
   int casino_ouvert(int id);
   int jeu_du_casino(int id);
   int jeux();
   int cagnotte(int id);
   int casino_du_jeu(int id);
   int date_jeu(int id);
   int memorise_jeu(int id);
   int convecteur_temporel(int id, int date);
   int deplace_joueur(int id, int x, int y);

  // Special for gui. Not exported in prolo2006.yml.
  int terrain_graphique(int x, int y);
};


#endif /* !API_HH_ */
