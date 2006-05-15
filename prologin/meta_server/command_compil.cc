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


#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include <mysql.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "verboseur.hh"
#include "conf.hh"
#include "fork.hh"
#include "simple_func.hh"
#include "explode.hh"
#include "db.hh"

static int	db_get_id_competiteur_from_match(MYSQL *mysql, int id_compil_match)
{
  MYSQL_RES	*result;
  MYSQL_ROW	rows;
  char		query[200];
  int		id;
    
  print_verbose(2, 0, "[Meta] Gen id competiteur from id match.\n");
  // Find id of champion
  sprintf(query,
	  "SELECT competiteur.id"
	  " FROM competiteur, matchs"
	  " WHERE matchs.id = %d"
	  " AND competiteur.id_match = matchs.id;",
	  id_compil_match);
  print_verbose (4, 0, "[Main] SQL: \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return print_verbose(1, -1, "[Main] SQL query failed: \"%s\".\n", mysql_error(mysql));
  if ((result = mysql_store_result(mysql)) == NULL)
    return print_verbose(1, -1, "[Main] SQL store failed: \"%s\".\n", mysql_error(mysql));
  if ((rows = mysql_fetch_row(result)) == NULL)
    return print_verbose(1, -1, "[Main] SQL rows failed: \"%s\".\n", mysql_error(mysql));
  id = atoi(rows[0]);
  mysql_free_result(result);
  return id;
}

static char	**gen_exec_compil_cmd(MYSQL *mysql, struct s_conf *conf, 
				      int id_competiteur)
{
  char		query[300];
  char		cmd[300];
  MYSQL_RES	*result;
  MYSQL_ROW	row;

  print_verbose(3, 0, "[Meta] Generate compil command line.\n");
  sprintf(query,
	  "SELECT game.repertoire, champion.id_candidat, champion.id"
	  " FROM competiteur, game, champion"
	  " WHERE competiteur.id = %d"
	  " AND   champion.id = competiteur.id_champion"
	  " AND   game.id = champion.id_game ;",
	  id_competiteur);
  print_verbose (4, 0, "[Main] SQL: \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return (char **)print_verbose(1, 0, "[Main] SQL query failed: \"%s\".\n", mysql_error(mysql));
  if ((result = mysql_store_result(mysql)) == NULL)
    return (char **)print_verbose(1, 0, "[Main] SQL store failed: \"%s\".\n", mysql_error(mysql));
  if ((row = mysql_fetch_row(result)) == NULL)
    return (char **)print_verbose(1, 0, "[Main] SQL rows failed: \"%s\".\n", mysql_error(mysql));
  sprintf(cmd, "%s %s %s %s", conf->compil_cmd, row[0], row[1], row[2]);
  print_verbose(3, 0, "[Meta] Generate compil command: \"%s\".\n", cmd);
  mysql_free_result(result);
  return explode(cmd, ' ');
}

int	db_create_match_for_check_triche(MYSQL *mysql, int id_competiteur)
{
  MYSQL_RES	*result;
  MYSQL_ROW	rows;
  char		query[200];

  print_verbose(2, 0, "[Meta] Create check triche match.\n");
  // Find id of champion
  sprintf(query,
	  "SELECT champion.id, champion.id_game"
	  " FROM competiteur, champion, matchs"
	  " WHERE competiteur.id = %d"
	  " AND champion.id = competiteur.id_champion;",
	  id_competiteur);
  print_verbose (4, 0, "[Main] SQL: \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return print_verbose(1, 1, "[Main] SQL query failed: \"%s\".\n", mysql_error(mysql));
  if ((result = mysql_store_result(mysql)) == NULL)
    return print_verbose(1, 1, "[Main] SQL store failed: \"%s\".\n", mysql_error(mysql));
  if ((rows = mysql_fetch_row(result)) == NULL)
    return print_verbose(1, 1, "[Main] SQL rows failed: \"%s\".\n", mysql_error(mysql));

  int id_champion = atoi(rows[0]);
  int id_game = atoi(rows[1]);
  mysql_free_result(result);
  // Update champion  
  sprintf(query,
	  "INSERT INTO matchs (id_game, type)"
	  " VALUES (%d, 2);",
	  id_game);
  print_verbose(4, 0, "[Meta] SQL: \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return print_verbose(1, 1, "[Meta] Sql query failed: %s.\n", mysql_error(mysql));
  if (mysql_affected_rows(mysql) < 1)
    return print_verbose(1, 1, "[Meta] Sql query not applied", mysql_error(mysql));

  int id_match = mysql_insert_id(mysql);
  // Add competiteur
  sprintf(query,
	  "INSERT INTO competiteur  (id_champion, id_match)"
	  " VALUES (%d, %d);",
	  id_champion, id_match);
  print_verbose(4, 0, "[Meta] SQL: \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return print_verbose(1, 1, "[Meta] Sql query failed: %s.\n", mysql_error(mysql));
  if (mysql_affected_rows(mysql) < 1)
    return print_verbose(1, 1, "[Meta] Sql query not applied", mysql_error(mysql));

  
  return 0;
}

int 	exec_compil(char **com, struct s_fork_list *list, struct s_conf *conf)
{
  char  **gen_cmd_split = NULL;
  int	id_competiteur = db_get_id_competiteur_from_match(list->mysql, atoi(com[1]));

  if (id_competiteur == -1)
    return 1;
  print_verbose(2, 0, "[Meta] FONCTION: COMPIL.\n");
  if ((gen_cmd_split = gen_exec_compil_cmd(list->mysql, conf, id_competiteur)) == NULL)
    return 1;
  add_fork(list, make_fork(match_check_compil, gen_cmd_split, id_competiteur, 0));
  return 0;
}


int	finish_compil(MYSQL *mysql, struct s_fork *data, int status)
{
  if (WIFEXITED(status) && WEXITSTATUS(status))
    db_update_competiteur_data(mysql, data->id, 5, 0);
  else
  {
    db_update_competiteur_data(mysql, data->id, 1, 0);
    db_create_match_for_check_triche(mysql, data->id);
  }
  return 0;
}


