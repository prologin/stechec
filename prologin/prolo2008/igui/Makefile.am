# -*- Makefile -*-

lib_LTLIBRARIES	 = hamster_sdlvisu_interactive.la

hamster_sdlvisu_interactive_la_SOURCES	=		\
	graphics.c	\
	main.c		\
	players.c	\
	camera.c	\
	run.c		\
	orders.c	\
	input.c
	
hamster_sdlvisu_interactive_la_CFLAGS	=		\
	$(SDL_CFLAGS)

hamster_sdlvisu_interactive_la_LDFLAGS	=		\
	-module					\
	$(SDL_LIBS)

INCLUDES        =				\
        -I$(top_srcdir)/stechec/tools		\
        -I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/client		\
	-I$(top_srcdir)/stechec/sdl		\
	-I$(top_srcdir)/prologin/common
