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


#ifndef FORK_H_
# define FORK_H_

// FIXME: find bug
# define FORK_MAX_PROSS 	2000

# include <mysql.h>
# include "conf.hh"

enum e_type_match {
  match_none,
  match_check_compil,
  match_check_triche,
  match_real_match_server,
  match_real_match_client
};


struct s_fork
{
  enum e_type_match	type;
  int			pid;
  int			id;
  int			id_extra;
  char  		*buf_log;
  unsigned int		buf_lenght;
  int			buf_max;
  int			fd_out;
};

struct s_fork_list
{
  int		max;
  char		*ip;
  MYSQL 	*mysql;
  struct s_fork **tab;
};

struct s_fork	*make_fork(enum e_type_match type, char **com,
			   const int id, const int id_extra);
struct s_fork	*found_fork(struct s_fork_list *list, int pid);
int		free_fork(struct s_fork_list *list, struct s_fork *tmp);
int		add_fork(struct s_fork_list *list, struct s_fork *tmp);

#endif // !FORK_H_ 
