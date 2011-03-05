/*
  Stechec project is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Stechec project is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the Stechec project; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Copyright (C) 2005 Prologin
*/

#ifndef MATCH_H_
# define MATCH_H_

# include <mysql.h>
# include <stdlib.h>

# include "Computer.hh"
# include "conf.hh"

enum e_type_match {
  match_none,
  match_check_compil,
  match_check_triche,
  match_real_match
};

/**
 ** The main structure
 ** @param id		the id of the match in the database
 ** @param player	the player tab, it is filled with the id of the players in the db
 ** @param com		the computer tab
 ** @param type		the type of the match, for the meta server connection
 ** @param nbplayer	number of players
 */
class			Match
{
public:
  
  Match();
  ~Match();

  int	db_change_match_status(MYSQL *mysql, int status);
  
  int			_id;
  enum e_type_match	_type;
  int			_nb_players;
  int			*_players;
  Computer		*_computer;
  Computer		_computer_server;
  int			_port_server;
};

#endif // !MATCH_H_
