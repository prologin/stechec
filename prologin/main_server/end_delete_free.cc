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

#include <mysql.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "verboseur.hh"
#include "Match.hh"
#include "simple_func.hh"

/**
** Change a match status in database
** @param mysql		The MYSQL structure
** @param match		The match structure
** @param status	The new status of the match
** @return		1 if error, 0 otherwise
*/
int		db_change_match_status(MYSQL *mysql, Match *match, int status)
{
  char		query[200];

  print_verbose(3, 0, "[Main] Ready to update match status: %d.\n", match->id);
  sprintf(query, 
	  "UPDATE `matchs`" 
	  " SET `status` = %d"
	  " WHERE `id` = %d ;", 
	  status, match->_id);
  print_verbose(4, 0, "[Main] SQL: \"%s\"\n", query);
  if (mysql_query(mysql, query))
    return print_verbose(1, 1, "[Main] SQL query failed: \"%s\".\n", mysql_error(mysql));
  if (mysql_affected_rows(mysql) < 1)
    return print_verbose(1, 1, "[Main] SQL query not applied: \"%s\".\n", mysql_error(mysql));
  return 0;
}

/**
** Clean match information on Db and struct
** @param mysql		the MYSQL structure
** @param myres		The MYSQL_RES structure to be freed
** @param delete	As I explain the depth of action
** @param status	The status to be applied to the match
** @return		1 of error; 0 otherwise
*/
int	clean_match_data(MYSQL *mysql, MYSQL_RES *myres, 
			 Match *match, int status)
{
  /*
  int	i;

    print_verbose(3, 0, "[Main] Update match status query from DB.\n");
    if (status && db_change_match_status(mysql, match, status))
      return 1;
    if (delete_level >= 3)
    {
      print_verbose(3, 0, "[Main] Freeing each computer ip.\n");
      for (i = 0; i < match->nbplayer + 1; i++)
	free(match->com[i].ip);
      print_verbose(3, 0, "[Main] Freeing computer tab.\n");
      free(match->com);
    }
    if (delete_level >= 4 && match->nbviewer > 0)
    {
      print_verbose(3, 0, "[Main] Freeing viewer tab\n");
      free(match->viewers);
    }
  print_verbose(3, 0, "[Main] Freeing the SQL results\n");
  mysql_free_result(myres);
  myres = NULL;
  */
  return 0;
}
