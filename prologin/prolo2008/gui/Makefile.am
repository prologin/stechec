# -*- Makefile -*-

lib_LTLIBRARIES	 = robotus_sdlvisu.la

robotus_sdlvisu_la_SOURCES	=		\
	graphics.c	\
	main.c		\
	players.c	\
	camera.c	\
	next_turn.cc
	
robotus_sdlvisu_la_CFLAGS	=		\
	$(SDL_CFLAGS)

robotus_sdlvisu_la_LDFLAGS	=		\
	-module					\
	$(SDL_LIBS)

INCLUDES        =				\
        -I$(top_srcdir)/stechec/tools		\
        -I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/client		\
	-I$(top_srcdir)/stechec/sdl		\
	-I$(top_srcdir)/prologin/common
