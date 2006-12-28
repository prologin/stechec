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
#include <mysql.h>
#include <stdio.h>

#include "verboseur.hh"
#include "conf.hh"
#include "fork.hh"

/*!
** Add a new entry in table machine for this meta-server.
*/
static int      db_add_computer(MYSQL *mysql, const t_conf *conf)
{
  char          query[200];

  print_verbose(3, 0, "[Meta] Add new machine (%s) in DB.\n", conf->local_ip);
  sprintf(query, 
	  "INSERT INTO `machine`"
	  " ( `id` , `ip` , `nb_pross` , `status` , `exec_server`, `port_meta` )"
	  " VALUES ('', '%s', '0', '1', '0', '%d');", 
	  conf->local_ip, conf->db_port);
  print_verbose(4, 0, "[Meta] SQL \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return print_verbose(1, 1, "[Meta] SQL query failed: %s.\n", mysql_error(mysql));
  if (mysql_affected_rows(mysql) != 1)
    return print_verbose(1, 1, "[Meta] SAL query not applied: %s.\n", mysql_error(mysql));
  print_verbose(3, 0, "[Meta] machine (%s) is added in DB.\n", conf->local_ip);
  return 0;
}


/*!
** Update machine table with new value.
*/
int		db_update_computer_data(MYSQL *mysql, int status, int exec_server, struct s_conf *conf)
{
  char		query[300];
  
  print_verbose(3, 0, "[Meta] Update machine (%s) in DB.\n", conf->local_ip);
  sprintf(query, 
	  "UPDATE `machine`"
	  " SET `status` = %d, `port_meta` = %d , `exec_server` = %d"
	  " WHERE `ip` LIKE \'%s\' ;", 
	  status, conf->db_port, exec_server, conf->local_ip); 
  print_verbose(4, 0, "[Meta] SQL \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return print_verbose(1, 1, "[Meta] SQL query failed: %s.\n", mysql_error(mysql));
  print_verbose(3, 0, "[Meta] machine (%s) is update in DB.\n", conf->local_ip);
  return 0;
}

/*!
** Add this meta-server on database.
*/
int		db_send_own_ip(MYSQL *mysql, struct s_conf *conf)
{
  char		query[200];
  MYSQL_RES	*myres;
  int		query_res;
  int		err = 0;
  
  print_verbose(3, 0, "[Meta] Adding/Updating computer in DB.\n");
  sprintf(query, "select status from `machine` where `ip` LIKE \'%s\' ;", conf->local_ip);
  mysql_query(mysql, query);
  myres = mysql_store_result(mysql);
  query_res = mysql_num_rows(myres);
  print_verbose(4, 0, "[Meta] SQL (row == %d) \"%s\".\n", query_res, query);
  if(query_res == 0)
    err = db_add_computer(mysql, conf);
  else if(query_res == 1)
    err = db_update_computer_data(mysql, 1, 0, conf);
  else
   print_verbose(1, 0, "[Meta] Error: Tow many ip (%s) in DB.\n", conf->local_ip); 
  mysql_free_result(myres);
  if (!err)  
    print_verbose(3, 0, "[Meta] Adding/Updating computer in DB: Done.\n");
  return err;
}


/*!
** Search in db the type of server.
**
** @param mysql		MYSQL structure
** @param id_match	Id of the match
**
** @return 		Effective type
*/
enum e_type_match	db_get_type_match(MYSQL *mysql, int id_match)
{
  enum e_type_match	type;
  char			query[200];
  MYSQL_RES		*result;
  MYSQL_ROW		rows;
  
  print_verbose(3, 0, "[Meta] Search type of server.\n");
  sprintf(query,
	  "SELECT type"
	  " FROM `matchs`"
	  " WHERE `id` = %d ;",
	  id_match);
  print_verbose (4, 0, "[Main] SQL: \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return (enum e_type_match)print_verbose(1, 0, "[Main] SQL query failed: \"%s\".\n", mysql_error(mysql));
  if ((result = mysql_store_result(mysql)) == NULL)
    return (enum e_type_match)print_verbose(1, 0, "[Main] SQL store failed: \"%s\".\n", mysql_error(mysql));
  if ((rows = mysql_fetch_row(result)) == NULL)
    return (enum e_type_match)print_verbose(1, 0, "[Main] SQL rows failed: \"%s\".\n", mysql_error(mysql));
  type = (enum e_type_match)atoi(rows[0]);
  mysql_free_result(result);
  return type;
}


/*!
** Decrement number of processus on current computer
*/
int	db_dec_nb_proc_computer(MYSQL *mysql, char *ip)
{
  char	query[200];

  print_verbose(2, 0, "[Meta] Update query_machine: %s.\n", ip);
  sprintf(query, 
	  "UPDATE `machine`"
	  " SET `nb_pross` = `nb_pross` - 1"
	  " WHERE `ip` LIKE \'%s\' ;", 
	  ip);
  print_verbose(4, 0, "[Meta] SQL: \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return print_verbose(1, 1, "[Meta] Sql query failed: %s.\n", mysql_error(mysql));
  if (mysql_affected_rows(mysql) < 1)
    return print_verbose(1, 1, "[Meta] Sql query not applied: %s.\n", mysql_error(mysql));
  print_verbose(3, 0, "[Meta] Update machine ok.\n");
  return 0;
}


/*!
** Update status of a match
*/
int	db_update_match_status(MYSQL *mysql, int id_match, int status)
{
  char		query[200];

  print_verbose(3, 0, "[Meta] Update match status: %d.\n", id_match);
  sprintf(query, 
	  "UPDATE `matchs`" 
	  " SET `status` = %d"
	  " WHERE `id` = %d ;", 
	  status, id_match);
  print_verbose(4, 0, "[Meta] SQL: \"%s\"\n", query);
  if (mysql_query(mysql, query))
    return print_verbose(1, 1, "[Meta] SQL query failed: \"%s\".\n", mysql_error(mysql));
  if (mysql_affected_rows(mysql) < 1)
    return print_verbose(1, 1, "[Meta] SQL query not applied: \"%s\".\n", mysql_error(mysql));
  return 0;
}


/*!
** Add data in competiteur table with match result.
*/
int	db_update_score(MYSQL *mysql, int id_competiteur, int add_score, char *msg)
{
  char	query[200];

  print_verbose(2, 0, "[Meta] Update score de %d avec le score %d.\n", id_competiteur, add_score);
  sprintf(query,
	  "UPDATE competiteur"
	  " SET score = score + %d, log = \"%s\""
	  " WHERE `id` = %d ;",
	  add_score, msg, id_competiteur);
  print_verbose(4, 0, "[Meta] SQL: \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return print_verbose(1, 1, "[Meta] Sql query failed: %s.\n", mysql_error(mysql));
  if (mysql_affected_rows(mysql) < 1)
    return print_verbose(1, 1, "[Meta] Sql query not applied", mysql_error(mysql));
  return 0;
}

/*!
** Update data of competiteur.
*/
int		db_update_competiteur_data(MYSQL *mysql, int id_competiteur, int level, int status)
{
  MYSQL_RES	*result;
  MYSQL_ROW	rows;
  char		query[200];

  print_verbose(2, 0, "[Meta] Update champion data.\n");
  // Find id of champion
  sprintf(query,
	  "SELECT champion.id"
	  " FROM champion, competiteur"
	  " WHERE competiteur.id = %d"
	  " AND champion.id = competiteur.id_champion;", 
	  id_competiteur);
  print_verbose (4, 0, "[Main] SQL: \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return (enum e_type_match)print_verbose(1, 0, "[Main] SQL query failed: \"%s\".\n", mysql_error(mysql));
  if ((result = mysql_store_result(mysql)) == NULL)
    return (enum e_type_match)print_verbose(1, 0, "[Main] SQL store failed: \"%s\".\n", mysql_error(mysql));
  if ((rows = mysql_fetch_row(result)) == NULL)
    return (enum e_type_match)print_verbose(1, 0, "[Main] SQL rows failed: \"%s\".\n", mysql_error(mysql));
  int id_champion = atoi(rows[0]);
  mysql_free_result(result);
  // Update champion
  sprintf(query,
	  "UPDATE `champion`"
	  " SET level = %d, status = %d"
	  " WHERE id = %d;",
	  level, status, id_champion);
  print_verbose(4, 0, "[Meta] SQL: \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return print_verbose(1, 1, "[Meta] Sql query failed: %s.\n", mysql_error(mysql));
  if (mysql_affected_rows(mysql) < 1)
    return print_verbose(1, 1, "[Meta] Sql query not applied", mysql_error(mysql));
  return 0;
}




/*!
** Init SQL connection to Database
*/
int		init_sql(MYSQL *mysql, const t_conf *conf)
{
  my_bool val = 1;

  print_verbose(3, 0, "[Meta] Init SQL.\n");
  if (!(mysql_init(mysql)))
    return print_verbose(1, -1, "[Meta] Init SQL Error: %s.\n", mysql_error(mysql));
  mysql_options(mysql, MYSQL_OPT_RECONNECT, &val);
  if (!mysql_real_connect(mysql, conf->db_ip, conf->db_login,
			  conf->db_pass, conf->db_database, 0, NULL, 0))
    return print_verbose(1, -1, "[Meta] Real connect SQL Error: %s.\n", mysql_error(mysql));
  print_verbose(3, 0, "[Meta] Init SQL done.\n");
  return 0;
}


/*!
** Close SQL connection to Database
*/
int		close_sql(MYSQL *mysql)
{
  print_verbose(3, 0, "[Meta] Close SQL.\n");
  mysql_close(mysql);
  return 0;
}
