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

int            my_return(char *s, MYSQL *mysql);
void           my_exit(char *s, MYSQL *mysql);
int            mem_return(void);
int	       test_retrieved_row(MYSQL_ROW *row, int nb);
char	       *inttostr(int nb);

#endif /* !SIMPLE_FUNC_H_ */
