# -*- Makefile -*-

lib_LTLIBRARIES		= ant_sdlvisu.la

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

INCLUDES        =				\
        -I$(top_srcdir)/stechec/tools		\
        -I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/client		\
	-I$(top_srcdir)/prologin/common		\
	-I$(srcdir)/../rules
