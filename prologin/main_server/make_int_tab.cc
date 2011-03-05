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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "verboseur.hh"
#include "Match.hh"
#include "simple_func.hh"

/**
 ** Checked the validity of the retieved players string;
 ** It has to be "number1,number2,number3,...,numbern"
 ** If it isn't like this, an error occured 
 ** and the query will be reported as containing an error to the admin
 ** @param players	the string of players
 ** @param type		for the debug, indicating the row witch failed
 ** @return		-1 if error, otherwise the number of player found
 */
int		check_validity(char *, char *)
{
  /*
  int		i = 0;
  int		nbplayer = 1;

  if (strlen(players) == 0)
    return 0;
  if (players[0] < '0' || players[0] > '9')
    return print_verbose(1, -1, "[Main] SQL database error, entry will be delete.\n");
  for (i = 0; players[i] != '\0'; i++)
    if (players[i] == ',')
      {
	if (players[i + 1] == ',' || players[i + 1] == '\0')
	  return print_verbose(1, -1, "[Main] SQL database error, entry will be delete.\n");
	else
	  nbplayer++;
      }
    else if (players[i] < '0' || players[i] > '9')
      return print_verbose(1, -1, "[Main] SQL database error, entry will be delete.\n");
  print_verbose(2, 0, "[Main] Found %d %s(s).\n", nbplayer, type);
  return nbplayer;
  */
  return 0;
}

/**
 ** Parse the player string and fill the player tab in the match structure
 ** @param s	the player string, void here because of compatibility:
 ** @param tab	the pointer on the player tab of the match structure
 ** @param type	For the debug,
 ** @param size	the lenght of the final tab
 ** @return	1 if error, 0 otherwise
 */
int		make_int_tab(void *, int **, char *, int )
{
/*
  int		i = 0;
  int		nbplayer = size;
  char		*players = (char *)s;
  int		start = 0;
  int		wait = 0;
  char		*tmp;
  
  printf("---Validity Checked: %s\n", players);
  if ((tab[0] = new int[nbplayer]) == NULL)
    return mem_return();
  for (i = 0, nbplayer = 0; ; i++)
    if (players[i] == ',' || players[i] == '\0')
      {
	wait = start;
	if ((tmp = new char[i - start + 1]) == NULL)
	  mem_return();
	for(; start < i; start++)
	  tmp[start - wait] = players[start];
	tmp[start - wait] = '\0';
	start = i + 1;
	tab[0][nbplayer++] = atoi(tmp);
	print_verbose(3, 0, "[Main] Found %s id: %d.\n", type, tab[0][nbplayer - 1]);
	free(tmp);
	if (players[i] == '\0')
	  break;
      }
  return 0;
*/
  return 0;
}


