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


#ifndef EXPLODE_H_
# define EXPLODE_H_

/*
** Split a string in mulple string
** @arg		char*	str: initial string
** @arg		char	sep: separator
** @return	char**	array oh multiple string (last case is NULL)
*/
char	**explode(char *str, const char sep);

#endif /*! EXPLODE_H_ */
