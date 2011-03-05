# -*- Makefile -*-

lib_LTLIBRARIES	 = globulus_sdlvisu.la

globulus_sdlvisu_la_SOURCES	=		\
	GlobulusGui.cc				\
	GlobulusGui.hh				\
	Square.hh

globulus_sdlvisu_la_CXXFLAGS	=		\
	$(SDL_CFLAGS)				\
	-DMODULE_NAME=\"globulusgui\"		\
	-DMODULE_COLOR=C_YELLOW			\
	-DPKGDATADIR=\"$(pkgdatadir)/prolo2007/\"

globulus_sdlvisu_la_LIBADD	=		\
	$(top_builddir)/stechec/sdl/libstechec_sdl.la

globulus_sdlvisu_la_LDFLAGS	=		\
	-module					\
	$(SDL_LIBS)

INCLUDES        =				\
        -I$(top_srcdir)/stechec/tools		\
        -I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/client		\
	-I$(top_srcdir)/stechec/sdl		\
	-I$(top_srcdir)/prologin/common		\
	-I$(srcdir)/../rules
