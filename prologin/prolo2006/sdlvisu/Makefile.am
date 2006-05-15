# -*- Makefile -*-

lib_LTLIBRARIES		= outatime_sdlvisu.la

outatime_sdlvisu_la_SOURCES	=		\
	OutAGui.cc				\
	OutAGui.hh				\
	Object.hh				\
	Square.hh				\
	Sprite.hh				\
	InfoBox.hh				\
	ResourceCenter.hh			\
	VirtualScreen.cc			\
	VirtualScreen.hh

outatime_sdlvisu_la_CXXFLAGS	=		\
	$(SDL_CFLAGS)				\
	-DMODULE_NAME=\"outagui\"		\
	-DMODULE_COLOR=C_YELLOW			\
	-DPKGDATADIR=\"$(pkgdatadir)/prolo2006/\"

outatime_sdlvisu_la_LDFLAGS	=		\
	-module					\
	$(SDL_LIBS)

INCLUDES        =				\
        -I$(top_srcdir)/stechec/tools		\
        -I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/client		\
	-I$(top_srcdir)/prologin/common		\
	-I$(top_srcdir)/prologin/gui_qt		\
	-I$(srcdir)/../rules
