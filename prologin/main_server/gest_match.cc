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
#include <string.h>
#include <assert.h>

#include "verboseur.hh"
#include "Match.hh"
#include "Computer.hh"
#include "simple_func.hh"
#include "make_int_tab.hh"
#include "make_computer.hh"
#include "end_delete_free.hh"
#include "connection_meta.hh"
#include "conf.hh"

/**
** A simple function to print the results of the parsing
** 
** @param match		the match structure fully filled and ready to use
*/
static void	print_match_information(Match *match)
{
  assert (match);
  print_verbose(2, 0, "[Main] Match information (id == %d).\n", match->_id);
  print_verbose(2, 0, "[Main]   _ Player(s): ");
  for (int i = 0; i < match->_nb_players; i++)
    print_verbose(2, 0, "{ id=%d }", match->_players[i]);
  print_verbose(2, 0, "\n");
  print_verbose(2, 0, "[Main]   _ Server: id=%d; ip=\"%s\"; port=%d",
		match->_computer_server._id, 
		match->_computer_server._ip, 
		match->_computer_server._port);
  if (match->_computer)
  {
    print_verbose(2, 0, "\n");
    print_verbose(2, 0, "[Main]   _ Client(s): ");
    for (int i = 0; i < match->_nb_players; i++)
      print_verbose(2, 0, "{ id=%d; ip=\"%s\"; port=%d }, ",
		    match->_computer[i]._id, 
		    match->_computer[i]._ip, 
		    match->_computer[i]._port);
  }
  print_verbose(2, 0, "\n");
}

/**
** Get all players id for a match in db.
** 
** @return	0 => Ok
** 		1 => KO
*/
static int	db_get_players(MYSQL *mysql, Match *match)
{
  char		query[200];
  MYSQL_RES	*result;
  MYSQL_ROW	rows;

  print_verbose(2, 0, "[Main] Making players id list.\n");
  sprintf(query,
	  "SELECT id"
	  " FROM `competiteur`"
	  " WHERE `id_match` = %d ;",
	  match->_id);
  print_verbose (4, 0, "[Main] SQL: \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return print_verbose(1, 1, "[Main] SQL query failed: \"%s\".\n", mysql_error(mysql));
  if ((result = mysql_store_result(mysql)) == NULL)
    return print_verbose(1, 1, "[Main] SQL store failed: \"%s\".\n", mysql_error(mysql));
  match->_nb_players = mysql_num_rows(result);
  match->_players = new int[match->_nb_players];
  for (int i = 0; (rows = mysql_fetch_row(result)) != NULL; ++i)
    match->_players[i] = atoi(rows[0]);
  mysql_free_result(result);
  return 0;
}

/**
** It parses the result given by the MYSQL query done in boucle_princpale
** First of all, it checks if we found only 1 results; otherwise it occurs an error
** If it is ok, we fetch the row and fill the structure match.
** We call 6 functions to fill it,
** test_retrieved row witch test if we got "expected" results
** delete_query witch update the query status to 1 meaning it is being take care of.(don't know if it is good english :p)
** make_int_tab applied to players
** make computer_tab to make the computer tab
** make_int_tab again but for the viewers...It might be remove in the futur
** Then if everything worked it print the results on stdout and 
** call main_meta_connection to connect to the meta server
** If an error occured during the process; end_delete_free is called
** with the approriate settings depending on what failed.
** 
** @param myres		The results of the MYSQL querry.
** @param mysql		The MYSQL structure
**
** @return		1 if error, otherwise 0
*/
static int		do_match(MYSQL_RES *myres, MYSQL *mysql,
				 struct s_conf *conf)
{
  MYSQL_ROW		row;
  Match			match;

  print_verbose(2, 0, "[Main] Do a match.\n");
  
  if (mysql_num_rows(myres) == 0)
    return 1;

  row = mysql_fetch_row(myres);
  if (test_retrieved_row(&row, 2))
    return 1;
  match._id = atoi(row[0]);
  if (match.db_change_match_status(mysql, 1))
    return 1;
  match._type = (enum e_type_match)atoi(row[1]);

  // Get all player id.
  if (db_get_players(mysql, &match))
  {
    match.db_change_match_status(mysql, 2);
    return 1;
  }

  // Read connected computers information.
  if (make_computer_tab(&match, mysql, conf))
  {
    match.db_change_match_status(mysql, 2);
    return 1;
  }

  print_match_information(&match);

  if (main_meta_connection(&match))
  {
    match.db_change_match_status(mysql, 2);
    return 1;
  }
  
  return 0;
}

/**
** The main loop:
** It selects the next game with the mysql querry,
** then it calls do_match witch will parse the results
** @param mysql	the MYSQL structure
** @return		1 if error, otherwise 0
*/
int		boucle_principale(MYSQL *mysql, struct s_conf *conf)
{
  MYSQL_RES *myres = NULL;
  int  exec_match = 0;

  while (1)
  {
    print_verbose(2, 0, "[Main] Wait a match to execute.\n");
    exec_match = 1;
    if (mysql_query(mysql,
		    "SELECT id, type"
		    " FROM `matchs`"
		    " WHERE `status` = 0"
		    " ORDER BY id"
		    " LIMIT 0,1;"))
      return print_verbose(1, 1, "[Main] Sql query failed: %s.\n", mysql_error(mysql));
    if ((myres = mysql_store_result(mysql)) != NULL)
    {
      if (mysql_num_rows(myres) != 0)
	exec_match = do_match(myres, mysql, conf);
      mysql_free_result(myres);
    }
    else if(mysql_field_count(mysql) != 0)
      return print_verbose(1, 1, "[Main] SQL store failed: %s.\n", mysql_error(mysql));
    if (exec_match)
      sleep(conf->wait_sleep);
    else
      usleep(200000);   
  }
  return 0;
}
