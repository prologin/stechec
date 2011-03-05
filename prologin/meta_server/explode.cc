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
#include <string.h>
#include <assert.h>

#include "verboseur.hh"

#define	MAX_SPLIT_WORDS		255


/*
** Split a string in mulple string
** @arg		char*	str: initial string
** @arg		char	sep: separator
** @return	char**	array oh multiple string (last case is NULL)
*/
char		**explode(char *str, const char sep)
{
  char		**res = NULL;
  int		ind = 0, nb_words = 0;
  bool		need_sep;
  
  print_verbose(3, 0, "[String] Exploding (sep == \'%c\'): \"%s\".\n", sep, str);
  res = (char **)malloc (sizeof (char *) * (MAX_SPLIT_WORDS + 1));
  res[0] = NULL;
  do
  {
    assert (nb_words < MAX_SPLIT_WORDS);
    need_sep = false;
    while (str[ind] && (str[ind] == sep || str[ind] == '\n'))
      ++ind;
    if (!str[ind])
      break ;
    res[nb_words] = str + ind;
    while (str[ind] && (str[ind] != sep && str[ind] != '\n'))
      ++ind;
    if (str[ind])
    {
      need_sep = true;
      str[ind] = '\0';
    }
    res[nb_words] = strdup(res[nb_words]);
    res[++nb_words] = NULL;
    if (need_sep)
      str[ind] = sep;
  } while (str[ind]);
  print_verbose(3, 0, "[String] Exploding: find %d word(s).\n", nb_words);
  return res;
}


