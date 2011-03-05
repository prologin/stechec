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


#include <sys/wait.h>
#include <mysql.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "simple_func.hh"
#include "verboseur.hh"
#include "explode.hh"
#include "conf.hh"
#include "fork.hh"
#include "db.hh"


/*!
** Generate command line to execute check_triche script
*/
static char	**gen_exec_server_cmd(MYSQL *mysql, struct s_conf *conf, 
				      int id_match, int port_server)
{
  char		query[2000];
  char		cmd[1000];
  char		opt_server[1000];
  MYSQL_RES	*result;
  MYSQL_ROW	row;

  print_verbose(3, 0, "[Meta] Generate client command line.\n");
  sprintf(query,
	  "SELECT game.lib_name, game.repertoire, matchs.is_competition,"
	  " matchs.id, matchs.opt_match"
	  " FROM  game, matchs, machine"
	  " WHERE matchs.id = %d "
	  " AND   game.id = matchs.id_game"
	  " AND	  machine.ip = \"%s\";",
	  id_match, conf->local_ip);
  print_verbose (4, 0, "[Main] SQL: \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return (char **)print_verbose(1, 0, "[Main] SQL query failed: \"%s\".\n", mysql_error(mysql));
  if ((result = mysql_store_result(mysql)) == NULL)
    return (char **)print_verbose(1, 0, "[Main] SQL store failed: \"%s\".\n", mysql_error(mysql));
  if ((row = mysql_fetch_row(result)) == NULL)
    return (char **)print_verbose(1, 0, "[Main] SQL rows failed: \"%s\".\n", mysql_error(mysql));
  sprintf(cmd, "%s %s %s %s %s %d", conf->server_cmd, row[0], 
	  row[1], row[2], row[3], port_server);
  sprintf(opt_server, " -- %s", row[4]);
  
  mysql_free_result(result);
  sprintf(query,
	  "SELECT id"
	  " FROM  competiteur"
	  " WHERE competiteur.id_match = %d;",
	  id_match);
  print_verbose (4, 0, "[Main] SQL: \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return (char **)print_verbose(1, 0, "[Main] SQL query failed: \"%s\".\n", mysql_error(mysql));
  if ((result = mysql_store_result(mysql)) == NULL)
    return (char **)print_verbose(1, 0, "[Main] SQL store failed: \"%s\".\n", mysql_error(mysql));

  while((row = mysql_fetch_row(result)) != NULL)
    sprintf(cmd, "%s %s", cmd, row[0]);

  sprintf(cmd, "%s %s", cmd, opt_server);
  
  print_verbose(3, 0, "[Meta] Generate server command: \"%s\".\n", cmd);
  mysql_free_result(result);
  return explode(cmd, ' ');
}

/*!
** Create struct for check_triche action
*/
int 	exec_server(char **com, struct s_fork_list *list, struct s_conf *conf)
{
  char **gen_cmd_split = NULL;
  
  print_verbose(2, 0, "[Meta] FONCTION: SERVER.\n");
  if ((gen_cmd_split = 
       gen_exec_server_cmd(list->mysql, conf, atoi(com[1]), atoi(com[2]))) == NULL)
    return 1;
  add_fork(list, make_fork(match_real_match_server, gen_cmd_split, atoi(com[1]), 0));
  return 0;
}

/*!
** Command to execute after check_triche run
*/
int	finish_server(MYSQL *mysql, struct s_fork *data, int status)
{
  if (WIFEXITED(status) && WEXITSTATUS(status))
  {
    db_update_match_status(mysql, data->id, 2);
    return 1;
  }
  char *scores = strdup(data->buf_log);
  char *tmp = scores;
  while (*tmp)
  {
    int id_competiteur;
    int score;
    int	code;
    char msg[300];

    id_competiteur = atoi(tmp); while (*tmp != ' ') ++tmp; ++tmp;
    score = atoi(tmp); while (*tmp != ' ') ++tmp; ++tmp;
    code = atoi(tmp); while (*tmp != ' ') ++tmp; tmp += 2;
    for (int it = 0; *tmp != '\n'; ++it, ++tmp) 
      { msg[it] = *tmp; msg[it + 1] = '\0'; } 
    msg[strlen(msg) - 1] = '\0';
    print_verbose(2, 0, "[Meta] Update score. id_competiteur = %d, score = %d.\n",
		  id_competiteur, score);
    db_update_score(mysql, id_competiteur, score, msg);
    if (*tmp) ++tmp;
  }
  free(scores);
  db_update_match_status(mysql, data->id, 3);
  return 0;
}

