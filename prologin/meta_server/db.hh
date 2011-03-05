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

#ifndef DB_H_
# define DB_H_

# include <mysql.h>
# include "conf.hh"
# include "fork.hh"

// Init SQL connection to Database.
int	init_sql(MYSQL *mysql, const t_conf *conf);

// Close SQL connection to Database.
int	close_sql(MYSQL *mysql);


// Update machine table with new value.
int     db_update_computer_data(MYSQL *mysql, int status, int exec_server, struct s_conf *conf);

// Update match status
int	db_update_match_status(MYSQL *mysql, int id_match, int status);

// Add this meta-server on database.
int	db_send_own_ip(MYSQL *mysql, struct s_conf *conf);

// Decrement number of processus on db.
int	db_dec_nb_proc_computer(MYSQL *mysql, char *ip);

// Add result of match on db.
int	db_update_score(MYSQL *mysql, int id_competiteur, int add_score, char *log);

// Update data of competiteur.
int	db_update_competiteur_data(MYSQL* mysql, int id_competiteur, int level, int status);

// Return type of match
enum e_type_match	db_get_type_match(MYSQL *mysql, int id_match);


#endif /* !DB_H_ */
