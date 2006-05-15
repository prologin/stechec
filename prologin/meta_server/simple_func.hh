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

#ifndef SIMPLE_FUNC_H_
# define SIMPLE_FUNC_H_

/*
** Print Message error and exit after.
*/
int            my_return(const char *s, MYSQL *mysql);

/*
** Print Message error and return -1.
*/
void           my_exit(const char *s, MYSQL *mysql);

/*
** Print Not enought memory.
*/
int            mem_return(void);

/*
** Check one row is NULL
*/
int	       test_retrieved_row(MYSQL_ROW *row, const int nb);

#endif /* !SIMPLE_FUNC_H_ */
