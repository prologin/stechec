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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*
** Print Message error and exit after.
*/
void            my_exit(const char *s, MYSQL *mysql)
{
  printf("%s : %s\n", s, mysql_error(mysql));
  exit(1);
}

/*
** Print Message error and return -1.
*/
int            my_return(const char *s, MYSQL *mysql)
{
  printf("%s : %s\n", s, mysql_error(mysql));
  return 1;
}

/*
** Print Not enought memory.
*/
int            mem_return(void)
{
  printf("------No more memory available! Exiting!\n");
  return 1;
}

/*
** Check one row is NULL
*/
int		test_retrieved_row(MYSQL_ROW *row, const int nb)
{
  int		i;

  for (i = 0; i < nb; i++)
    if (row[i] == NULL)
      {
	printf("------Retrieved Row error: %d.\n", i);
	return 1;
      }
  return 0;
}
