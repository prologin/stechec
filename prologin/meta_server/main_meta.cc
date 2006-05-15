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
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>

#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/select.h>

#include <string.h>
#include <mysql.h>


#include "connection.hh"
#include "db.hh"

#include "command.hh"
#include "verboseur.hh"

#include "argument.hh"

#include "fork.hh"
#include "conf.hh"




/*!
** Ajoute les files descripteur dans le "fd_set"
** Utiliser dans le select general
*/
int			make_fd_set(fd_set *rfds, int listen_fd,
				    struct s_fork_list *list)
{
  int			i = 0;
  int			max_fd = 0;

  FD_ZERO(rfds);
  FD_SET(listen_fd, rfds);
  max_fd = listen_fd;
  for (i = 0; i < list->max ; i++)
  {
    FD_SET(list->tab[i]->fd_out, rfds);
    if (max_fd < list->tab[i]->fd_out)
      max_fd = list->tab[i]->fd_out;
  }
  return max_fd + 1;
}

/*!
** Test si une connection a ete demande
** si c'est le cas parse la commande reseau et ferme la connection
*/
static void	test_accept_connet(int lisen_fd, 
				   fd_set *rfds,
				   struct s_fork_list *list,
				   struct s_conf *conf)
{
  int	tmp_fd;
  
  if (FD_ISSET(lisen_fd, rfds))
  {
    tmp_fd = accept(lisen_fd, NULL, NULL);
    if (dispatch_command(tmp_fd, list, conf))
      db_update_computer_data(list->mysql, 1, 0, conf);
    close(tmp_fd);
  }
}

static void	read_fork_data(struct s_fork *elt)
{
  char		buf[2025];
  int		tail;

  assert(elt);
  if (elt->type == match_real_match_server)
    do
    {
      tail = read(elt->fd_out, buf, 2024);
      buf[tail] = '\0';
      if (elt->buf_lenght < strlen(elt->buf_log) + tail)
      {
        realloc(elt->buf_log, elt->buf_lenght * 2);
        elt->buf_lenght *= 2;
      }
      strcat(elt->buf_log, buf);
    }
    while (tail == 2024);
}

/*!
** Test l'ecriture de tout les fils.
** Mise en memoire de ce qui est ecrit dans un buf.
*/
int		test_read_fork(fd_set *rfds, struct s_fork_list *list)
{
  assert (list->max < FORK_MAX_PROSS);
  for (int i = 0; i < list->max; i++)
    if (FD_ISSET(list->tab[i]->fd_out, rfds))
      read_fork_data(list->tab[i]);
  return 0;
}


/*!
** test la fin d'execution des prossesus fils
*/
int			test_end_pross(struct s_fork_list *list,
				       struct s_conf *conf)
{
  struct s_fork		*tmp;
  int			status;
  int			pid;

  if ((pid = wait3(&status, WNOHANG, NULL)) > 0)
  {
    print_verbose(2, 0, "[Meta] Process finish: %d\n", pid);
    tmp = found_fork(list, pid);
    read_fork_data(tmp);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
      print_verbose(3, 0, "[Meta] Process finish normaly.\n");
    else if (WIFEXITED(status))
      print_verbose(3, 0, "[Meta] Process finish with error: %d.\n", WEXITSTATUS(status));
    else
      print_verbose(3, 0, "[Meta] Process finish with big probleme.\n");
    dispatch_finish_command(list->mysql, list->ip, tmp, status);
    if (tmp->type != match_real_match_client)
      db_update_computer_data(list->mysql, 1, 0, conf);
    free_fork(list, tmp);
  }
  return 0;
}


/*!
** Boucle principle
*/
static void		main_boucle(int lisen_fd, MYSQL *mysql, t_conf *conf)
{
  fd_set		rfds;
  int			max_fd;
  struct s_fork_list	list;
  
  list.max = 0;
  list.mysql = mysql;
  list.ip = conf->local_ip;
  list.tab = new s_fork*[FORK_MAX_PROSS];
  while (1)
  {
    max_fd = make_fd_set(&rfds, lisen_fd, &list);
    if (select(max_fd, &rfds, NULL, NULL, NULL))
    {
      test_accept_connet(lisen_fd, &rfds, &list, conf);
      test_read_fork(&rfds, &list);
    }
    test_end_pross(&list, conf);
  }
}

static int		close_all(int fd, MYSQL *mysql, t_conf *conf)
{
  close(fd);
  db_update_computer_data(mysql, 0, 1, conf);
  printf("-Closing Meta Server\n");
  if (close_sql(mysql))
    exit(1);
  return 0;
}

int			main(int argc, char **argv)
{
  int			lisen_fd;
  MYSQL			mysql;
  t_conf		*conf = NULL;

  fill_arg(argc, argv, &conf); 
  level_verbose = conf->verbose;
  if ((lisen_fd = lisen_connect(&conf->db_port)) < 0)
    return 2;
  if (init_sql(&mysql, conf))
    return 3;
  if (db_send_own_ip(&mysql, conf))
    return close_all(lisen_fd, &mysql, conf);
  /* == AJOUT DEBUT D_BASE == */
  main_boucle(lisen_fd, &mysql, conf);
  close_all(lisen_fd, &mysql, conf);
  return 0;
}

