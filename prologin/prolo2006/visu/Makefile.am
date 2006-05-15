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
#  Copyright (C) 2006 Prologin
#

lib_LTLIBRARIES = outatime_visu.la

outatime_visu_la_SOURCES =			\
	OutAGui.cc				\
	OutAGui.hh				\
	main.cc

outatime_visu_la_CXXFLAGS =				\
	-DPKGDATADIR=\"$(pkgdatadir)/prolo2006\"	\
	$(QT_CFLAGS)

outatime_visu_la_LIBADD =					\
	$(top_builddir)/prologin/gui_qt/libstechec_guiqt.la	\
	$(QT_LIBS)

outatime_visu_la_LDFLAGS =				\
	-module

INCLUDES        =				\
        -I$(top_srcdir)/stechec/tools		\
        -I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/client		\
	-I$(top_srcdir)/prologin/common		\
	-I$(top_srcdir)/prologin/gui_qt		\
	-I$(srcdir)/../rules
