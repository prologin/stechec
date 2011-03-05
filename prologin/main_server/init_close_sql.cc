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

#include <mysql.h>

#include "verboseur.hh"
#include "conf.hh"

/*!
** Initialize mysql structure
** 
** @param mysql	the pointer on the MYSQL structure
** @param conf		the configuration information
**
** @return 1		if error, otherwise 0
*/
int		init_sql(MYSQL *mysql, struct s_conf *conf)
{
  print_verbose(3, 0, "[Main] SQL init.\n");
  if (!(mysql_init(mysql)))
    return print_verbose(1, 1, "[Main] SQL Init Failed: %s.\n", mysql_error(mysql));
  if (!mysql_real_connect(mysql, conf->db_ip, conf->db_login,
			  conf->db_pass, conf->db_database, 
			  0, NULL, 0))
    return print_verbose(1, 1, "[Main] SQL Connect Failed: %s.\n", mysql_error(mysql));
  return 0;
}

/*!
** Close MYSQL connection
** 
** @param mysql	the pointer on the MYSQL structure
** 
** return		1 if error, otherwise 0
*/
int		close_sql(MYSQL *mysql)
{
  mysql_close(mysql);
  print_verbose(3, 0, "[Main] SQL connection close.\n");
  return 0;
}
