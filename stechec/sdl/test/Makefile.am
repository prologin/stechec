# -*- Makefile -*-

noinst_PROGRAMS = simple scroll text

simple_SOURCES = simple.cc
scroll_SOURCES = scroll.cc
text_SOURCES = text.cc

CXXFLAGS = 					\
	-DMODULE_NAME=\"sdltest\"		\
	-DMODULE_COLOR=C_GREEN			\
	$(SDL_CFLAGS)

INCLUDES =					\
	-I$(srcdir)/../				\
	-I$(top_srcdir)/stechec/tools

LDADD =							\
	$(top_builddir)/stechec/tools/libtools.la	\
	$(top_builddir)/stechec/sdl/libstechec_sdl.la	\
	$(SDL_LIBS)
