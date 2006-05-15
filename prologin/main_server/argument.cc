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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "conf.hh"

extern char *optarg;
extern int optind;


/*!
** The static option table.
** Contains 2 important fields : the long option and its short equivalent.
** 
** @param p	the port used for connections with main server
** @param c	the configuration file
** @param v	the verbose level
** @param i	the local ip
** @param h	display help
*/
static  struct option longopts[] =
  {
    { "configuration-file",	required_argument,	0,	'c' },
    { "local-ip",		required_argument,	0,	'i' },
    { "db-ip",			required_argument,	0,	'd' },
    { "server-port",		required_argument,	0,	'p' },
    { "verbose-level",     	required_argument,	0,	'v' },
    { "help",			no_argument,		0,	'h' },
    { NULL,			0,			0,	0   }
  };

/*!
** Print usage on stderr
*/
static void	display_help(void)
{
  fprintf (stderr, "usage : main_server [-cdipv ...]\n");
  fprintf (stderr, "Options are:\n");
  fprintf (stderr, " -h, --help                  : display this help\n");
  fprintf (stderr, " -c, --configuration-file    : configuraton file\n");
  fprintf (stderr, " -d, --db-ip                 : ip of the database\n");
  fprintf (stderr, " -i, --local-ip              : ip of the machine\n");
  fprintf (stderr, " -p, --server-port           : connection port\n");
  fprintf (stderr, " -v, --verbose-level         : level of verbose\n");
}

/*!
** Print usage on stderr
*/
static void	print_usage(void)
{
  fprintf (stderr, "usage : main_server [-cipv ...]\n");
  fprintf (stderr, "Try \'main_server -h\' for more informations\n");
  exit (1);
}

/**
** says if a char is an option.
** returns the position of the option 'c' in the option tab,
** or -1 if not found.
** @param c	the character we want to find out about
** @return	the position of that caracter in the tab 'action_tab'
** @see		action_tab
*/
static int	is_option(char c)
{
  int		i;

  for (i = 0; longopts[i].name && c != longopts[i].val; i++)
    ;
  if (longopts[i].name)
    return i;
  return -1;
}


/**
** fills the structure in function of the options.
** @param argument	the argument structure
** @param ch		the option character
** @see			argument.h
*/
static void	fill_struct(struct s_conf **conf, int ch)
{
  if (ch == 'h')
    display_help();
  else if (optarg == NULL)
    print_usage();
  else if (ch == 'c')
    read_conf(optarg, conf);
  else if (ch == 'd')
    (*conf)->db_ip = strdup(optarg);
  else if (ch == 'i')
    (*conf)->local_ip = strdup(optarg);
  else if (ch == 'p')
    (*conf)->db_port = atoi(optarg);
  else if (ch == 'v')
    (*conf)->verbose = atoi(optarg);
}

/**
** parses the command line using getopt.
** @param argc		the number of argument on the command line
** @param argv		the string array of arguments
** @param conf		the argument structure
*/
void	fill_arg(int argc, char **argv, struct s_conf **conf)
{
  int	ch;
  int	i;

  if (!*conf)
    *conf = init_conf();
  while ((ch = getopt_long(argc, argv, "c:i:p:v:d:h", longopts, NULL)) != -1)
    {
      i = is_option(ch);
      if (i == -1 && ch == '?')
	print_usage();
      fill_struct(conf, ch);
    }
  if ((*conf)->local_ip == NULL || (*conf)->db_port == 0)
    print_usage();
}
