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
#  Copyright (C) 2005, 2006 Prologin
#

noinst_LTLIBRARIES = libstechec_guiqt.la

libstechec_guiqt_la_MOC =	\
	Gui.moc.cc		\
	MainWindow.moc.cc	\
	Object2D.moc.cc		\
	QTObject.moc.cc		\
	QTWorld.moc.cc		\
	GLWorld.moc.cc

libstechec_guiqt_la_SOURCES =			\
	Gui.cc					\
	Gui.hh					\
	MainWindow.cc				\
	MainWindow.hh				\
	Object2D.hh				\
	World.cc				\
	World.hh				\
	GLWorld.cc				\
	GLWorld.hh				\
	QTObject.cc				\
	QTObject.hh				\
	QTPixmap.cc				\
	QTPixmap.hh				\
	QTWorld.cc				\
	QTWorld.hh				\
	$(libstechec_guiqt_la_MOC)

libstechec_guiqt_la_CXXFLAGS =				 \
	-DPIXMAP_BASE=\"$(DESTDIR)$(pkgdatadir)/pixmap\" \
	$(QT_CFLAGS)

libstechec_guiqt_la_LIBADD =	\
	$(QT_LIBS)

libstechec_guiqt_la_LDFLAGS = -module

INCLUDES        =				\
        -I$(top_srcdir)/stechec/tools		\
        -I$(top_srcdir)/stechec/base_rules	\
        -I$(top_srcdir)/stechec/client		\
        -I$(srcdir)/../common			\
        -I$(srcdir)/../rules			\
	-I/usr/X11R6/include

BUILT_SOURCES = $(libstechec_guiqt_la_MOC)

CLEANFILES = $(libstechec_guiqt_la_MOC)

%.moc.cc: %.hh
	@MOC@ -o $@ $<


EXTRA_DIST = sprites

install-data-hook:
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/pixmap
	-chmod -R u+w $(DESTDIR)$(pkgdatadir)/pixmap
	cp -r $(srcdir)/sprites/* $(DESTDIR)$(pkgdatadir)/pixmap

uninstall-hook:
	rm -rf $(DESTDIR)$(pkgdatadir)/pixmap
