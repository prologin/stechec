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
#include <string.h>

#include "verboseur.hh"

/**
 ** if mysql error, exit program, not used very much::
 ** @param s		a message wrote by the developper
 ** @param mysql	the MYSQL structure to get the error
 ** @return		1 meaning error
 */ 
void            my_exit(char *s, MYSQL *mysql)
{
  print_verbose(1, 1, "[Main]%s : %s\n", s, mysql_error(mysql));
  exit(1);
}

/**
 ** if mysql error, return 1 and print the error:
 ** @param s		a message wrote by the developper
 ** @param mysql	the MYSQL structure to get the error
 ** @return		1 meaning error
 */ 
int            my_return(char *s, MYSQL *mysql)
{
  print_verbose(1, 1, "[Main]%s : %s\n", s, mysql_error(mysql));
  return 1;
}

/**
 ** if memory error, return 1 meaning error and print the cause.
 ** @return	1 meaning error
 */ 
int            mem_return(void)
{
  print_verbose(1, 0, "[Main] No more memory available! Exiting!\n");
  return 1;
}

/**
 ** Test all the retrieved row to check if there are an error
 ** @param row	the MYSQL_ROW structure with all the rows
 ** @param nb	the number of expected row.
 ** @return	1 of error, 0 otherwise
 */
int		test_retrieved_row(MYSQL_ROW	*row, int nb)
{
  int		i;

  for (i = 0; i < nb; i++)
    if (row[i] == NULL)
      return print_verbose(1, 1, "[Main] Retrieved Row error.\n");
  return 0;
}


/**
 ** Convert an int to a string, doesn't accept negative numbers
 ** @param nb	the number
 ** @return	the number converted or NULL if an error occured
 */
char		*inttostr(int nb)
{
  char		tmp_str[20];

  sprintf(tmp_str, "%d", nb);
  return strdup(tmp_str);
}
