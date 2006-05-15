# -*- Makefile -*-

lib_LTLIBRARIES		= ant_sdlvisu.la ant_qtvisu.la

CXXFLAGS =					\
	-DMODULE_NAME=\"ant_visu\"		\
	-DMODULE_COLOR=C_YELLOW			\
	-DPKGDATADIR=\"$(pkgdatadir)/ant/\"


ant_sdlvisu_la_SOURCES	=			\
	sdl_visu.hh				\
	sdl_visu.cc

ant_sdlvisu_la_CXXFLAGS	=			\
	$(SDL_CFLAGS)

ant_sdlvisu_la_LDFLAGS	=			\
	-module					\
	$(SDL_LIBS)


ant_qtvisu_la_SOURCES	=			\
	qt_visu.cc				\
	qt_visu.hh

ant_qtvisu_la_LDFLAGS 	= 			\
	-module

ant_qtvisu_la_CXXFLAGS 	=			\
	$(QT_CFLAGS)

ant_qtvisu_la_LIBADD	=					\
	$(top_builddir)/prologin/gui_qt/libstechec_guiqt.la	\
	$(QT_LIBS)

INCLUDES        =				\
        -I$(top_srcdir)/stechec/tools		\
        -I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/client		\
	-I$(top_srcdir)/prologin/common		\
	-I$(top_srcdir)/prologin/gui_qt		\
	-I$(srcdir)/../rules
