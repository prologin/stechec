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

#include "conf.hh"

enum e_parse_state { 
  init = 0, 
  db,
  internal,
  query,
  cmd
};


static void	read_db_conf(const char *line, int num_line, struct s_conf *conf);
static void	read_intern_conf(const char *line, int num_line, struct s_conf *conf);
static void	read_cmd_conf(const char *line, int num_line, struct s_conf *conf);

typedef void (*ptr_read_conf)(const char *line, int num_line, struct s_conf *conf);

struct parse_state {
  char			*key;
  enum e_parse_state	state;
  ptr_read_conf		fct;
};

static struct parse_state	tab_conf[] =
{
  {"database:",	db, 		read_db_conf},
  {"internal:",	internal,	read_intern_conf},
  {"command:", 	cmd, 		read_cmd_conf},
  {NULL, 	init, 		NULL}
};

/*
** Check if the conf enter in a new state
*/
static int	change_state(const char *line, enum e_parse_state *state)
{
  int 		i;

  for (i = 0; tab_conf[i].key; ++i)
    if (strncmp(line, tab_conf[i].key, strlen(tab_conf[i].key)) == 0)
    {  
      *state = tab_conf[i].state;
      return 1;
    }
  return 0;
}

/*
** Read vonfiguration for "Database" 
*/
static void	read_db_conf(const char *line, int num_line, struct s_conf *conf)
{
  if (strncmp(line, "  ip: ", 6) == 0)
    conf->db_ip = strdup(line + 6);
  else if (strncmp(line, "  login: ", 9) == 0)
    conf->db_login = strdup(line + 9);
  else if (strncmp(line, "  pass: ", 8) == 0)
    conf->db_pass = strdup(line + 8);
  else if (strncmp(line, "  database: ", 12) == 0)
    conf->db_database = strdup(line + 12);
  else if (strncmp(line, "  port: ", 8) == 0)
    conf->db_port = atoi(line + 8);
  else
    fprintf(stderr, "./meta_server: Error in conf file line %d\n", num_line);
}

/*
** Read vonfiguration for "Internal" 
*/
static void	read_intern_conf(const char *line, int num_line, struct s_conf *conf)
{
  if (strncmp(line, "  wait_sleep: ", 14) == 0)
    conf->wait_sleep = atoi(line + 14);
  else if (strncmp(line, "  max process: ", 15) == 0)
    conf->nb_max_process = atoi(line + 15);
  else if (strncmp(line, "  ip local: ", 12) == 0)
    conf->local_ip = strdup(line + 12);
  else if (strncmp(line, "  verbose: ", 11) == 0)
    conf->verbose = atoi(line + 11);
  else
    fprintf(stderr, "./meta_server: Error in conf file line %d\n", num_line);
}


/*
** Read vonfiguration for "Command" 
*/
static void	read_cmd_conf(const char *line, int num_line, struct s_conf *conf)
{
  if (strncmp(line, "  compil: ", 10) == 0)
    conf->compil_cmd = strdup(line + 10);
  else if (strncmp(line, "  triche: ", 10) == 0)
    conf->triche_cmd = strdup(line + 10);
  else if (strncmp(line, "  client: ", 10) == 0)
    conf->client_cmd = strdup(line + 10);
  else if (strncmp(line, "  server: ", 10) == 0)
    conf->server_cmd = strdup(line + 10);
  else
    fprintf(stderr, "./meta_server: Error in conf file line %d\n", num_line);
}


/*
** Read the configuration file and return all parameters
*/
int			read_conf(const char *conf_file, struct s_conf **conf)
{
  enum e_parse_state 	state;
  FILE			*file;
  char			line[101];
  int			num_line, i;

  if ((file = fopen(conf_file, "r")) == NULL) 
  {
    perror(conf_file); 
    return 1;
  }
  if (!*conf)
    *conf = init_conf();
  num_line = 0;
  state = init;
  while (fgets(line, 100, file) != NULL)
  {
    ++num_line;
    line[strlen(line) - 1] = '\0';
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\0')
      continue ;
    if (change_state(line, &state))
      continue ;
    for (i = 0; tab_conf[i].key; ++i)
      if (state == tab_conf[i].state)
      {
        tab_conf[i].fct(line, num_line, *conf);
        continue;
      }
  }
  return 0;
}

/*
** Create an default struct s_conf
*/
struct s_conf		*init_conf(void)
{
  struct s_conf		*conf;
  
  conf = new s_conf();

  /* Database*/
  conf->db_ip = NULL;
  conf->db_login = NULL;
  conf->db_pass = NULL;
  conf->db_database = NULL;
  conf->db_port = 0;

  /* Internal */
  conf->wait_sleep = 0;
  conf->nb_max_process = 0;
  conf->local_ip = NULL;
  conf->verbose = 0;
  
  /* Command */
  conf->compil_cmd = NULL;
  conf->triche_cmd = NULL;
  conf->client_cmd = NULL;
  conf->server_cmd = NULL;

  return conf;
}

