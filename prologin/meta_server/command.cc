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
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

#include "explode.hh"
#include "command.hh"
#include "verboseur.hh"
#include "conf.hh"
#include "db.hh"
  

/*!
** Dispatch different action by main_server message
*/
int	 	dispatch_command(int fd, struct s_fork_list *list, struct s_conf *conf)
{
  char		buf[1024];
  char		**exp_buf;
  int		len;
  int		res = 0;
  
  print_verbose(3, 0, "[Meta] Choose command to execute.\n");

  // Read command in network.
  len = read(fd, buf, 1024);
  buf[len] = '\0';

  // explode command
  exp_buf = explode(buf, ' ');
  
  // Client action: "client $id_match $id_player $id_computeeeur_server"
  if (strcmp(exp_buf[0], "client") == 0)
    exec_client(exp_buf, list, conf);
  // Server action: "server $id_match"
  else if (strcmp(exp_buf[0], "server") == 0)
    switch (db_get_type_match(list->mysql, atoi(exp_buf[1])))
    {
      case match_check_compil:
	res = exec_compil(exp_buf, list, conf);
	break;
	
      case match_check_triche:
	res = exec_triche(exp_buf, list, conf);
	break;
	
      case match_real_match_server:
	res = exec_server(exp_buf, list, conf);
	break;
	
      default:
	res = print_verbose(1, 1, "[Meta] Type of server not exist.\n");
	break;
    }  

  // Free explode data.
  for (int i = 0; exp_buf[i] != NULL; ++i)
    free (exp_buf[i]);
  free (exp_buf);
  return res;
}


int	dispatch_finish_command(MYSQL *mysql,
			        char *ip,
				struct s_fork *data, 
				int status)
{
  db_dec_nb_proc_computer(mysql, ip);
  switch (data->type)
  {
    case match_check_compil:
      finish_compil(mysql, data, status);
      break ;
      
    case match_check_triche:
      finish_triche(mysql, data, status);
      
      break ;
      
    case match_real_match_server:
      finish_server(mysql, data, status);
      break ;
      
    case match_real_match_client:
      finish_client(mysql, data, status);
      break ;
      
    default:
      return print_verbose(1, 1, "[Meta] Type of data not exist.\n");
      break;
  }
   
  return 0;
}

