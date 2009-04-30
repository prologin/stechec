# -*- Makefile -*-

lib_LTLIBRARIES	 = simcity_sdlvisu.la

simcity_sdlvisu_la_SOURCES	= \
		main.cpp	\
		display.cpp	\
		sfcfield.cpp	\
		surface.cpp	\
		sprite.cpp	\
		displaymap.cpp	\
		thread.cpp	\
		gameengine.cpp	\
		gamecase.cpp	\
		font.cpp	\
		infopanel.cpp	\
		infopanel2.cpp	\
		turnpanel.cpp

simcity_sdlvisu_la_CXXFLAGS	=		\
	-W -Wall -ansi $(SDL_CFLAGS)

simcity_sdlvisu_la_LDFLAGS	=		\
	-module					\
	$(SDL_LIBS)

INCLUDES        =					\
        -I$(top_srcdir)/stechec/tools			\
        -I$(top_srcdir)/stechec/base_rules		\
	-I$(top_srcdir)/stechec/client			\
	-I$(top_srcdir)/stechec/sdl			\
	-I$(top_srcdir)/prologin/common			\
	-I$(top_srcdir)/prologin/prolo2009/rules	\
	-I$(top_srcdir)/prologin/prolo2009/gui/include
