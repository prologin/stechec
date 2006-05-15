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

#include <stdlib.h>
#include <mysql.h>
#include <unistd.h>
#include <stdio.h>

#include "init_close_sql.hh"
#include "gest_match.hh"
#include "conf.hh"
#include "argument.hh"
#include "verboseur.hh"


/**
 ** Main fonction.
 ** Initialize, launch principal loop and close:
 ** @return	0 if everything is ok or 1.
 **/
int		main(int argc, char **argv)
{
  MYSQL		mysql;
  struct s_conf	*conf = NULL;
 
  fill_arg(argc, argv, &conf);
  level_verbose = conf->verbose;
  print_verbose(2, 0, "[Main] Initializing Mysql connection.\n");
  if (init_sql(&mysql, conf))
    exit(1);
  print_verbose(2, 0, "[Main] Launching main loop.\n");
  if (boucle_principale(&mysql, conf))
    print_verbose(1, 0, "[Main] Exiting program: possible major error.\n");
  if (close_sql(&mysql))
    exit(1);
  return 0;
}
