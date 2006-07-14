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

antdatadir = $(pkgdatadir)/ant

dist_antdata_DATA =	\
	basic.map	\
	four_player.map	\
	hell.map	\
	duel.map	\
	labyrinthe.map	\
	prologin.map	\
	castle.map	\
	ant.yml

EXTRA_DIST = texture_2D

dist-hook:
	rm -rf `find $(distdir) -name .svn`

install-data-hook:
	-cp -r $(srcdir)/texture_2D $(DESTDIR)$(antdatadir)

uninstall-hook:
	-chmod -R u+w $(DESTDIR)$(antdatadir)/texture_2D
	rm -rf $(DESTDIR)$(antdatadir)/texture_2D 
