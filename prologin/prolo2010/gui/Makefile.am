# -*- Makefile -*-

lib_LTLIBRARIES	 = toons_sdlvisu.la

toons_sdlvisu_la_SOURCES	= \
		main.cpp	\
		display.cpp	\
		sfcfield.cpp	\
		surface.cpp	\
		sprite.cpp	\
		displaymap.cpp	\
		thread.cpp	\
		gameengine.cpp	\
		gamecase.cpp	\
		font.cpp

toons_sdlvisu_la_CXXFLAGS	=		\
	-W -Wall -ansi $(SDL_CFLAGS) -O3 -DPKGDATADIR=\"$(pkgdatadir)/prolo2010/\"

toons_sdlvisu_la_LDFLAGS	=		\
	-module					\
	$(SDL_LIBS)

INCLUDES        =					\
        -I$(top_srcdir)/stechec/tools			\
        -I$(top_srcdir)/stechec/base_rules		\
	-I$(top_srcdir)/stechec/client			\
	-I$(top_srcdir)/stechec/sdl			\
	-I$(top_srcdir)/prologin/common			\
	-I$(top_srcdir)/prologin/prolo2010/rules	\
	-I$(top_srcdir)/prologin/prolo2010/gui/include
