# -*- Makefile -*-

noinst_PROGRAMS = simple scroll

simple_SOURCES = simple.cc
scroll_SOURCES = scroll.cc

CXXFLAGS = 					\
	-DMODULE_NAME=\"sdltest\"		\
	-DMODULE_COLOR=C_GREEN			\
	$(SDL_CFLAGS)

INCLUDES =					\
	-I../					\
	-I$(top_srcdir)/stechec/tools

LDADD =							\
	$(top_builddir)/stechec/tools/libtools.la	\
	$(top_builddir)/stechec/sdl/libstechec_sdl.la

LDFLAGS = $(SDL_LIBS)
