# -*- Makefile -*-

lib_LTLIBRARIES	 = outatime_sdlvisu.la

outatime_sdlvisu_la_SOURCES	=		\
	OutAGui.cc				\
	OutAGui.hh				\
	Object.hh				\
	Sprite.hh				\
	InfoBox.hh

outatime_sdlvisu_la_CXXFLAGS	=		\
	$(SDL_CFLAGS)				\
	-DMODULE_NAME=\"outagui\"		\
	-DMODULE_COLOR=C_YELLOW			\
	-DPKGDATADIR=\"$(pkgdatadir)/prolo2006/\"

outatime_sdlvisu_la_LIBADD	=		\
	$(top_builddir)/stechec/sdl/libstechec_sdl.la

outatime_sdlvisu_la_LDFLAGS	=		\
	-module					\
	$(SDL_LIBS)

INCLUDES        =				\
        -I$(top_srcdir)/stechec/tools		\
        -I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/client		\
	-I$(top_srcdir)/stechec/sdl		\
	-I$(top_srcdir)/prologin/common		\
	-I$(srcdir)/../rules
