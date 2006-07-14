# -*- Makefile -*-

noinst_LTLIBRARIES = libstechec_sdl.la

libstechec_sdl_la_SOURCES =			\
	ResourceCenter.hh			\
	ResourceCenter.cc			\
	Surface.hh				\
	Surface.cc				\
	VirtualSurface.hh			\
	VirtualSurface.cc			\
	VirtualScrollableSurface.hh		\
	VirtualScrollableSurface.cc		\
	TextSurface.hh				\
	TextSurface.cc				\
	InputTextSurface.hh			\
	InputTextSurface.cc			\
	Sprite.hh				\
	Sprite.cc				\
	Input.hh				\
	Input.cc				\
	SDLWindow.hh				\
	SDLWindow.cc

libstechec_sdl_la_CXXFLAGS =			\
	-DMODULE_NAME=\"visu2d\"		\
	-DMODULE_COLOR=C_YELLOW			\
	-DPKGDATADIR=\"$(datadir)/$(PACKAGE)\"	\
	$(SDL_CFLAGS)

INCLUDES        =				\
        -I$(top_srcdir)/stechec/tools
