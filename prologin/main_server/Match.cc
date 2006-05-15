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

#include <stdlib.h>

#include "Match.hh"
#include "Computer.hh"
#include "verboseur.hh"
#include "conf.hh"


Match::Match():
  _id (-1),
  _type (match_none),
  _nb_players (0),
  _players (NULL),
  _computer (NULL)
{ }

Match::~Match()
{
  if (_players)
    delete[] _players;
  if (_computer)
    delete[] _computer;
}

/*!
** Change a match status in database
** 
** @param mysql		The MYSQL structure
** @param match		The match structure
** @param status	The new status of the match
** 
** @return		1 if error, 0 otherwise
*/
int	Match::db_change_match_status(MYSQL *mysql, int status)
{
  char		query[200];

  print_verbose(3, 0, "[Main] Ready to update match status: %d.\n", _id);
  sprintf(query, 
	  "UPDATE `matchs`" 
	  " SET `status` = %d"
	  " WHERE `id` = %d ;", 
	  status, _id);
  print_verbose(4, 0, "[Main] SQL: \"%s\"\n", query);
  if (mysql_query(mysql, query))
    return print_verbose(1, 1, "[Main] SQL query failed: \"%s\".\n", mysql_error(mysql));
  if (mysql_affected_rows(mysql) < 1)
    return print_verbose(1, 1, "[Main] SQL query not applied: \"%s\".\n", mysql_error(mysql));
  return 0;
}


