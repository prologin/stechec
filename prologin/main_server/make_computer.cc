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
#include "end_delete_free.hh"
#include "connection_meta.hh"
#include "conf.hh"


// FIXME: Fonction must go in Computer class


/*!
** Increment number of process of the meta server
** 
** @param mysql		The MYSQL strucure
** @param id_computer	Id of computer
**
** @return		1 if error, 0 otherwise
*/
static int	db_increment_computer_use(MYSQL *mysql, int id_computer)
{
  char		query[200];

  print_verbose(3, 0, "[MAin] Update query: %d.\n", id_computer);
  sprintf(query, 
	  "UPDATE `machine`"
	  " SET `nb_pross` = `nb_pross` + 1"
	  " WHERE `id` = %d ;", 
	  id_computer);
  print_verbose(4, 0, "[Main] SQL: \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return print_verbose(1, 1, "[Main] Sql query failed: %s.\n", mysql_error(mysql));
  if (mysql_affected_rows(mysql) < 1)
    return print_verbose(1, 1, "[Main] Sql query not applied: %s.\n", mysql_error(mysql));
  return 0;
}


/*!
** Increment number of process of the meta server
** 
** @param mysql		The MYSQL strucure
** @param id_computer	Id of computer
**
** @return		1 if error, 0 otherwise
*/
/*
static int	db_use_server_in_computer(MYSQL *mysql, int id_computer, int port_server)
{
  char		query[200];

  print_verbose(3, 0, "[Main] Update query: %d.\n", id_computer);
  sprintf(query, 
	  "UPDATE machine"
	  " SET   exec_server = 1, port_server = %d"
	  " WHERE id = %d ;", 
	  port_server, id_computer);
  print_verbose(4, 0, "[Main] SQL: \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return print_verbose(1, 1, "[Main] Sql query failed: %s.\n", mysql_error(mysql));
  if (mysql_affected_rows(mysql) < 1)
    return print_verbose(1, 1, "[Main] Sql query not applied: %s.\n", mysql_error(mysql));
  return 0;
}
*/

/**
** Update db to tell the meta server is down.
** 
** @param mysql		the MYSQL structure
** @param id_computerr	Id of computer
** 
** @return		1 if error, 0 otherwise
*/
static int	db_down_computer(MYSQL *mysql, int id_computer)
{
  char		query[200];

  print_verbose(3, 0, "[Main] Update query: Connection Lost with com id: %d.\n", 
		id_computer);
  sprintf(query, 
	  "UPDATE `machine`"
	  " SET `status` = 0" 
	  " WHERE `id` = %d ;",
	  id_computer);
  print_verbose(4, 0, "[Main] SQL: \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return print_verbose(1, 1, "[Main] Sql query failed: %s.\n", mysql_error(mysql));
  if (mysql_affected_rows(mysql) < 1)
    return print_verbose(1, 1, "[Main] Sqlquery not applied", mysql_error(mysql));
  return 0;
}

/*!
** Like do_match, parse the query and fill the computer structure
** if no query error occured, it calls 2 functions
** test_retrieved_row like in do_match
** init_connection witch is done here to test if the meta server is up and ruinning
** otherwise we select an other computer.
** 
** @param mysql		MYSQL structure used for the query
** @param result	the results of the query
** @param computer	Store information
** 
** @return		1 if error, 0 otherwise
*/
static int	db_get_computer_info(MYSQL *mysql, MYSQL_RES *result, 
				     Computer *computer)
{
  MYSQL_ROW	row;
  
  assert (result && computer);
    
  if ((row = mysql_fetch_row(result)) == NULL)
     return print_verbose(1, 1, "[Main] Sql computer data failed: %s.\n", mysql_error(mysql));
  if (test_retrieved_row(&row, 3))
     return print_verbose(1, 1, "[Main] Sql computer data failed: %s.\n", mysql_error(mysql));

  computer->_id = atoi(row[0]);
  computer->_nb_pross = atoi(row[1]);
  computer->_ip = strdup(row[2]);
  computer->_port = atoi(row[3]);

  print_verbose(2, 0, "[Main] Found computer %d: %s:%d\n", 
		computer->_id, computer->_ip, computer->_port);
  
  mysql_free_result (result);
  return 0;
}


/*!
** Find a computer which can execute a server
** 
** @param mysql		MYSQL structure used for the query
** @param computer	struct to store information
** @param conf		configuration information
** @param is_server	want a computer to do a server
**
** @return		1 if error, 0 oterwise
*/
int find_computer(MYSQL *mysql, Computer *computer, struct s_conf *conf, bool is_server)
{
  MYSQL_RES	*result = NULL;
  char		query[300];
 
  sprintf(query, "SELECT id, nb_pross, ip, port_meta"
		 " FROM `machine`"
	         " WHERE `status` = 1" 
	         " AND `nb_pross` < %d"
	         " %s" 
	         " ORDER BY nb_pross ASC"
	         " LIMIT 0, 1;",
		 conf->nb_max_process,
	         is_server ? "AND `exec_server` = 0" : "");

  // Loop while don't find a computer
  do
  {
    print_verbose(4, 0, "[Main] SQL: \"%s\".\n", query);
    print_verbose(2, 0, "[Main] Try to find a computer.\n");
    if (mysql_query(mysql, query))
      return print_verbose(1, 1, "[Main] Sql query failed: %s.\n", mysql_error(mysql));
    result = mysql_store_result(mysql);
    if (!result && mysql_errno(mysql) != 0)
      return print_verbose(1, 1, "[Main] Sql store failed: %s.\n", mysql_error(mysql));
    if (mysql_num_rows(result) != 0)
      break ;
    if (result) 
      mysql_free_result(result);
    sleep(conf->wait_sleep);
  } while (1);

  return db_get_computer_info(mysql, result, computer);
}


/*!
** Do connection with computer
**
** @param mysql		the MYSQL structure used for the query
** @param computer	Computer witch connected
**
** @return 		0: Connection ok
** 			1: Computer don't ask
** 			2: Fatal error
*/
int	test_connection_with_computer(MYSQL *mysql, Computer *computer)
{
  assert (computer);
  
  if ((computer->_fd = init_connection(computer->_ip, computer->_port)) < 0)
  {
    if (db_down_computer(mysql, computer->_id))
      return 2;
    return 1;
  }
  if (db_increment_computer_use(mysql, computer->_id))
    return 2;
  write(computer->_fd, "ping", 4);
  close (computer->_fd);
  return 0;
}

/*!
** The main function witch will make the computer tab
** 
** @param match		the match structure
** @param mysql		the MYSQL structure used for the query
** @param conf		the configuration information
** 
** @return		1 if error, 0 oterwise
*/
int			make_computer_tab(Match *match, MYSQL *mysql,
					  struct s_conf *conf)
{
  static int 		port_server = 0;
  print_verbose(2, 0, "[Main] Making computer tab.\n");

  // Find a server
  do
  {
    print_verbose(2, 0, "[Main] Try to find a server.\n");
    if (find_computer(mysql, &(match->_computer_server), conf, false))
      return 1;
    port_server++; port_server %= 600;
    //db_use_server_in_computer(mysql, match->_computer_server._id, port_server + 1300);
    match->_port_server = port_server + 1300;
  } while (test_connection_with_computer(mysql, &(match->_computer_server)) == 1);
    
  // Find client(s)
  if (match->_type == match_real_match)
  {
    print_verbose(2, 0, "[Main] Try to find a client.\n");
    if ((match->_computer = new Computer[match->_nb_players]) == NULL)
      return mem_return();

    for (int i = 0; i < match->_nb_players; ++i)
      do
      {
	if (find_computer(mysql, match->_computer + i, conf, false))
	  return 1;
      } while (test_connection_with_computer(mysql, match->_computer + i) == 1);
  }
          
  return 0;
}
