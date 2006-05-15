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

#ifndef MAKE_COMPUTER_H_
# define MAKE_COMPUTER_H_

# include "Match.hh"
# include "conf.hh"

// FIXME: Fonction must go in Computer class

int	make_computer_tab(Match *match, MYSQL *mysql, struct s_conf *conf);

#endif // !MAKE_PLAYER_H_
