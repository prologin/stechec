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

rubydir = $(pkglibdir)/ruby/mapeditor_prolo2005
prolo2005datadir = $(pkgdatadir)/prolo2005

dist_prolo2005data_DATA =			\
	full.bitmap				\
	plasma.bitmap				\
	water.bitmap				\
	ore.bitmap

dist_ruby_DATA =	\
	map_editor.rb	\
	opt.rb		\
	map.rb		\
	menu.rb		\
	new.rb		\
	image_path.rb

bin_SCRIPTS =		\
	map_editor

CLEANFILES = $(bin_SCRIPTS)

map_editor : Makefile
	echo '#! /bin/sh' > $@
	echo -n 'exec @RUBY@ -I$(RUBY_INC)/mapeditor_prolo2005' >> $@
	echo -n ' -I$(RUBY_INC)' >> $@ 
	echo ' $(RUBY_INC)/mapeditor_prolo2005/map_editor.rb $$@' >> $@
