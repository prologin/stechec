# -*- Makefile -*-

lib_LTLIBRARIES	 = argent_sdlvisu.la

argent_sdlvisu_la_SOURCES	= \
		interface.cc	  \
		show.cc

argent_sdlvisu_la_CXXFLAGS	=		\
	-W -Wall -ansi $(SDL_CFLAGS) -DPKGDATADIR=\"$(pkgdatadir)/summer2010/\"

argent_sdlvisu_la_LDFLAGS	=		\
	-module					\
	$(SDL_LIBS)

INCLUDES        =					\
	-I$(top_srcdir)/prologin/summer2010/common
