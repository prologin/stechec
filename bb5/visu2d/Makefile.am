# -*- Makefile -*-

bin_PROGRAMS	= bb5_visu

lib_LTLIBRARIES	= bb5_visu.la


bb5_visu_SOURCES =				\
	main.cc					\
	Button.cc				\
	Button.hh				\
	Colors.hh				\
	DialogBox.cc				\
	DialogBox.hh				\
	Global.cc				\
	Global.hh				\
	GuiError.cc				\
	GuiError.hh				\
	Menu.cc					\
	Menu.hh

bb5_visu_la_SOURCES =				\
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
	Sprite.hh				\
	Sprite.cc				\
	Input.hh				\
	Input.cc				\
	SDLWindow.hh				\
	SDLWindow.cc				\
	ActionPopup.hh				\
	ActionPopup.cc				\
	Game.hh					\
	Game.cc					\
	Panel.hh				\
	Panel.cc				\
	Field.hh				\
	Field.cc				\
	VisuPlayer.hh				\
	VisuPlayer.cc

CXXFLAGS +=					\
	-DMODULE_NAME=\"visu2d\"		\
	-DMODULE_COLOR=C_YELLOW			\
	-DPKGDATADIR=\"$(datadir)/$(PACKAGE)\"	\
	$(SDL_CFLAGS)

# bb5_visu_LDADD =					\
#         ../common/libbb5_common.la			\
#         ../client/bb5_client_rules.la			\
#         ../arbiter/bb5_arbiter.la			\
#         $(top_builddir)/stechec/tools/libtools.la

# FIXME: lala
bb5_visu_LDADD = $(top_builddir)/stechec/tools/libtools.la

bb5_visu_LDFLAGS	=			\
	$(SDL_LIBS)

bb5_visu_la_LDFLAGS	=			\
	-module					\
	$(SDL_LIBS)

INCLUDES        =				\
        -I$(top_srcdir)/stechec/tools		\
        -I$(top_srcdir)/stechec/base_rules	\
        -I$(top_srcdir)/stechec/client		\
        -I$(srcdir)/../client			\
	-I$(srcdir)/../common
