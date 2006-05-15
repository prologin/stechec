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


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <errno.h>
#include <assert.h>

#include "verboseur.hh"
#include "fork.hh"
#include "simple_func.hh"
#include "explode.hh"
#include "command.hh"
#include "conf.hh"


/*
** Create and return a new allocated struct s_fork.
*/
static struct s_fork	*init_fork_data(const int buf_log_size, 
				        const enum e_type_match type,
					const int id,
					const int id_extra)
{
  struct s_fork	*fork_data;
  
  fork_data = new s_fork();
  fork_data->type = type;
  fork_data->id = id;
  fork_data->id_extra = id_extra;
  fork_data->buf_log = (char *)malloc (sizeof (char) * buf_log_size);
  fork_data->buf_lenght = buf_log_size;
  fork_data->buf_log[0] = '\0';
  fork_data->buf_max = 0;
  return fork_data;
}


static void		close_fils_pipe(int *out_pipe)
{
  dup2(out_pipe[1], STDOUT_FILENO);
  close(out_pipe[0]);
  close(out_pipe[1]);
}


static void		close_pere_pipe(int *out_pipe)
{
  close(out_pipe[0]);
  close(out_pipe[1]);
}


/* 
** Fonction qui fork.
** retourne une structe contenan les infos du fork.
*/
struct s_fork	*make_fork(const enum e_type_match type, char **cmd,
			   const int id, const int id_extra)
{
  struct	s_fork *res;
  int		out_pipe[2];
  int		pid;

  res = init_fork_data(2025, type, id, id_extra);
  if (pipe(out_pipe) != 0)
    print_verbose (1, 0, "[Meta] Error pipe: %s.\n", strerror(errno));
  if ((pid = fork()) < 0)
    print_verbose (1, 0, "[Meta] Error fork: %s.\n", strerror(errno));
  if (pid == 0)
  {
    close_fils_pipe(out_pipe);
    execvp(cmd[0], cmd);
    print_verbose(3, 0, "[Meta] Return after execvp.\n");
    perror("");
    exit(126);
  }
  res->fd_out = dup(out_pipe[0]);
  close_pere_pipe(out_pipe);
  res->pid = pid;
  print_verbose(3, 0, "[Meta] fork (pid == %d) done.\n", pid);
  return res;
}


/*
** Fonction qui ajoute un fork
*/
int	add_fork(struct s_fork_list *list, struct s_fork *tmp)
{
  assert (tmp);
  if (list->max >= FORK_MAX_PROSS)
    return 0;
  list->tab[list->max] = tmp;
  list->max += 1;
  return 1;
}

/*
** Fonction qui libere un fork
*/
int			free_fork(struct s_fork_list *list,
				  struct s_fork *tmp)
{
  int		i = 0;

  while (i < list->max && list->tab[i]->pid != tmp->pid)
    i++;
  assert (i < list->max);

  print_verbose(2, 0, "[Meta] Total data: \"%s\".\n", tmp->buf_log);
  free(tmp->buf_log);
  close(tmp->fd_out);
  delete tmp;
  
  list->tab[i] = list->tab[list->max - 1];
  list->max--;
  return 0;
}


struct s_fork		*found_fork(struct s_fork_list *list, int pid)
{
  int			i;

  for (i = 0; i < list->max; i++)
  {
    if (list->tab[i]->pid == pid)
      return list->tab[i];
  }
  abort();
  return NULL;
}
