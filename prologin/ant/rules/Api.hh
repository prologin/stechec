/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2004, 2005, 2006 Prologin
*/

#ifndef API_HH_
# define API_HH_

# include "Contest.hh"
# include "GameData.hh"

/*!
** @ingroup stechec_ant
** @{
*/

/*!
** @class _ant__Api
**
** @brief Method of this call are called by the candidat, throught 'interface.cc'
**
** You can:
**  - access to the GameData class: _g
**  - send message to the server: void SendToServer(struct s_com&)
*/
class Api: public StechecApi          
{

public:

  Api(GameData* gameData, Client* c);
  virtual ~Api();

  //
  // these functions are local, no need to query the server.
  //
  int get_my_team();
  int get_nb_ant(int team_id);
  int get_landscape_width();
  int get_landscape_height();
  int get_anthill_pos_x(int team_id);
  int get_anthill_pos_y(int team_id);
  int get_ant_pos_x(int ant_id);
  int get_ant_pos_y(int ant_id);
  int get_land_type(int x, int y);
  int get_land_ant(int x, int y);
  int get_ant_luggage(int ant_id);

  //
  // these three functions ask something from server
  //  
  int move_ant(int ant_id, int x, int y);
  int carry_food(int ant_id);
  int let_food(int ant_id);

  /* not really useful yet, as ant can't die */
  int get_next_ant();

protected:
  virtual void teamSwitched();
};

//! @}

#endif /* !API_HH_ */
