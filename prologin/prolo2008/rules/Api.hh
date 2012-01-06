/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#ifndef API_HH_
# define API_HH_

# include <Contest.hh>
# include "GameData.hh"

/*!
** Method of this call are called by the candidat, throught 'interface.cc'
**
** You can:
**  - access to the GameData class: _g
**  - send message to the server: void SendToServer(struct s_com&)
*/

/*
 * Remarks on the robots' id :
 * Team 0 has id in [0, MAX_ROBOTS / 2)
 * Team 1 has id in [MAX_ROBOTS / 2, MAX_ROBOTS)
 * But, on the champion side, his own id are in [0, MAX_ROBOTS / 2) 
 * and the opponents id are in [MAX_ROBOTS / 2, MAX_ROBOTS)
 */

# define TEST_POS(x, y) \
  if (x >= g_->_map_size_x || y >= g_->_map_size_y || y < 0 || x < 0) \
    return BAD_ARGUMENT;

# define CHECK_ID(id) \
  if (id < 0 || id >= MAX_ROBOTS || !g_->_robots[id].IsEnabled()) \
    return BAD_ARGUMENT;

# define CHECK_OWN_ID(id) \
  if (id < 0 || id >= MAX_ROBOTS / 2) \
    return BAD_ARGUMENT;

# define CHECK_DIRECTION(dir, add)			\
  if (dir < 0 || dir >= 4+add) \
    return BAD_ARGUMENT;

class Api: public StechecApi          
{

public:

  Api(GameData* gameData, Client* c);
  virtual ~Api() { }

public:

   int taille_carte_x();
   int taille_carte_y();
   int numero_tour();
   int nombre_tours();
   bool commence();
   int pos_x(int id);
   int pos_y(int id);
   int porte_pomme(int id);
   int distance(int x1, int y1, int x2, int y2, bool sort_tranchee);
   int type_case(int x, int y);
   int pomme(int x, int y);
   int trognons_restants(int id);
   int deplacer(int id, int direction);
   int turbo(int id);
   int lacher_pomme(int id, int direction);
   int ramasser_pomme(int id);
   int grappin(int id, int direction);
   int trognon(int id, int direction);
   int attendre(int id);

  /*
   * not exported to the candidat's API (only for the graphical interface)
   */
  // Function to get all the actions done during last turn
  // returns BAD_ARGUMENT if numero is greater than the number of the last action.
  // returns action + id*10 + arg*100
  // arg is equal to zero if it is not used for the specified action
  // id is the *server's* robot id :
  // (team 0 has robots from 0 to MAX_ROBOTS/2 - 1, and team 1 has robots from MAX_ROBOTS/2 to MAX_ROBOTS-1)
  int actions_faites(int numero);

protected:
  virtual void teamSwitched();
  
private: 
  // some utility function
  void ChampionIdToRealId(int *id);
  int SwitchSubjectiveGlobalCoords(int y);
  void SwitchSubjectiveGlobalDir(int *dir);
  int Order(int real_id); //returns TOO_MUCH_ORDERS, or the turn of the robot whose id is real_id
  void SendBasicOrder(int real_id, int robot_turn, int order_cst);
  void SendOrderWithDirection(int real_id, int robot_turn, int order_cst, int direction);
};


#endif /* !API_HH_ */
