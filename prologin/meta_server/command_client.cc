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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql.h>

#include "verboseur.hh"
#include "explode.hh"
#include "fork.hh"
#include "db.hh"


/*!
** Generate command line to execute check_triche script
*/
static char	**gen_exec_client_cmd(MYSQL *mysql, struct s_conf *conf, 
				      int id_competiteur, int id_machine,
				      int port_server)
{
  char		query[600];
  char		cmd[600];
  MYSQL_RES	*result;
  MYSQL_ROW	row;

  print_verbose(3, 0, "[Meta] Generate client command line.\n");
  sprintf(query,
	  "SELECT game.lib_name, game.repertoire, matchs.is_competition, champion.id_candidat,"
	  " champion.id, competiteur.id, competiteur.id_match, machine.ip"
	  " FROM competiteur, game, champion, machine, matchs"
	  " WHERE competiteur.id = %d "
	  " AND   champion.id = competiteur.id_champion"
	  " AND   game.id = champion.id_game"
	  " AND   matchs.id = competiteur.id_match"
	  " AND   machine.id = %d;",
	  id_competiteur, id_machine);
  print_verbose (4, 0, "[Main] SQL: \"%s\".\n", query);
  if (mysql_query(mysql, query))
    return (char **)print_verbose(1, 0, "[Main] SQL query failed: \"%s\".\n", mysql_error(mysql));
  if ((result = mysql_store_result(mysql)) == NULL)
    return (char **)print_verbose(1, 0, "[Main] SQL store failed: \"%s\".\n", mysql_error(mysql));
  if ((row = mysql_fetch_row(result)) == NULL)
    return (char **)print_verbose(1, 0, "[Main] SQL rows failed: \"%s\".\n", mysql_error(mysql));
  sprintf(cmd, "%s %s %s %s %s %s %s %s %d %s", conf->client_cmd, row[0], 
	  row[1], row[2], row[3], row[4], row[5], row[6], port_server, row[7]);
  print_verbose(3, 0, "[Meta] Generate client command: \"%s\".\n", cmd);
  mysql_free_result(result);
  char **cmd_split = explode(cmd, ' ');
  return cmd_split;
}

/*!
** Create struct for check_triche action
*/
int 	exec_client(char **com, struct s_fork_list *list, struct s_conf *conf)
{
  char **gen_cmd_split = NULL;
  
  print_verbose(2, 0, "[Meta] FONCTION: CLIENT.\n");
  if ((gen_cmd_split = gen_exec_client_cmd(list->mysql, conf, 
	  atoi(com[1]), atoi(com[2]), atoi(com[3]))) == NULL)
    return 1;
  s_fork *cmd = NULL;
  if ((cmd = make_fork(match_real_match_client, gen_cmd_split, 0, 0)) == NULL)
    return 1;
  add_fork(list, cmd);
  return 0;
}

/*!
** Command to execute after check_triche run
*/
int	finish_client(MYSQL *, struct s_fork *, int )
{
  return 0;
}


