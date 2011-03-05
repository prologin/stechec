#
#  Stechec project is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  Stechec project is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with the Stechec project; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#  Copyright (C) 2005 Prologin
#

include $(top_srcdir)/build-aux/check-rules.mk

YAML_FILE = ant
YAML_TEST_POOL = $(srcdir)/test_pool.yml

# tests name from test_pool.yml to launch
MY_TESTS =					\
	initialization				\
	game_c					\
	game_java				\
	game_pascal

# champion sources that should go into the tarball.
EXTRA_DIST += 					\
	c/init.c				\
	c/simple_game.c				\
	java/Prologin.java			\
	pascal/prologin.pas
