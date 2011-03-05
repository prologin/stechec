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


#ifndef FONC_H_
# define FONC_H_

# include "conf.hh"
# include "fork.hh"

int	dispatch_command(int fd, struct s_fork_list *list, struct s_conf *conf);
int	dispatch_finish_command(MYSQL *mysql, char *ip, struct s_fork *data, int status);

int  	exec_client(char **com, struct s_fork_list *list, struct s_conf *conf);
int	finish_client(MYSQL *mysql, struct s_fork *data, int status);

int  	exec_compil(char **com, struct s_fork_list *list, struct s_conf *conf);
int	finish_compil(MYSQL *mysql, struct s_fork *data, int status);

int  	exec_triche(char **com, struct s_fork_list *list, struct s_conf *conf);
int	finish_triche(MYSQL *mysql, struct s_fork *data, int status);

int  	exec_server(char **com, struct s_fork_list *list, struct s_conf *conf);
int	finish_server(MYSQL *mysql, struct s_fork *data, int status);

int  	update_query_server(MYSQL *mysql, int exec_serv, char *match_id);

#endif  /* !FONC_H_ */
