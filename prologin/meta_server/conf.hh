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


#ifndef CONF_H_
# define CONF_H_ 

typedef struct s_conf
{
  char	*db_ip;
  char	*db_login;
  char	*db_pass;
  char	*db_database;
  int	db_port;

  int	wait_sleep;
  int	nb_max_process;
  char  *local_ip;
  int	verbose;
  
  char	*compil_cmd;
  char	*triche_cmd;
  char	*client_cmd;
  char	*server_cmd;
} t_conf;


/*
** Read the configuration file and return all parameters
*/
int	read_conf(const char *conf_file, struct s_conf **conf);

/*
** Create an default struct s_conf
*/
t_conf	*init_conf(void);


#endif /* !CONF_H_ */

