# -*- Makefile -*-

bin_PROGRAMS	= bb5_visu

lib_LTLIBRARIES	= bb5_visu.la

noinst_LTLIBRARIES = libbb5_visu_common.la

libbb5_visu_common_la_SOURCES =			\
	DlgBox.hh				\
	DlgBox.cc				\
	ActionPopup.hh				\
	ActionPopup.cc				\
	Panel.hh				\
	Panel.cc				\
	Field.hh				\
	Field.cc				\
	VisuPlayer.hh				\
	VisuPlayer.cc				\
	Game.hh					\
	Game.cc

bb5_visu_la_SOURCES = 				\
	main_library.cc

bb5_visu_SOURCES =				\
	main.cc					\
	Colors.hh				\
	Widget.cc Widget.hh			\
	Focus.cc Focus.hh			\
	GuiError.cc GuiError.hh			\
	Button.cc Button.hh			\
	DialogBox.cc DialogBox.hh		\
	Global.cc Global.hh			\
	Menu.cc Menu.hh				\
	Label.cc Label.hh			\
	Entry.cc Entry.hh			\
	LabelEntry.cc LabelEntry.hh

bb5_visu_LDADD = 					\
	$(top_builddir)/stechec/tools/libtools.la	\
	$(top_builddir)/stechec/sdl/libstechec_sdl.la	\
	libbb5_visu_common.la

bb5_visu_LDFLAGS	=			\
	$(SDL_LIBS)

bb5_visu_la_LIBADD 	=				\
	$(top_builddir)/stechec/sdl/libstechec_sdl.la	\
	libbb5_visu_common.la

bb5_visu_la_LDFLAGS	=			\
	-module					\
	$(SDL_LIBS)

CXXFLAGS +=					\
	-DMODULE_NAME=\"visu2d\"		\
	-DMODULE_COLOR=C_YELLOW			\
	-DPKGDATADIR=\"$(datadir)/$(PACKAGE)\"	\
	$(SDL_CFLAGS)

INCLUDES        =				\
        -I$(top_srcdir)/stechec/tools		\
        -I$(top_srcdir)/stechec/base_rules	\
        -I$(top_srcdir)/stechec/client		\
	-I$(top_srcdir)/stechec/sdl		\
        -I$(srcdir)/../client			\
	-I$(srcdir)/../common
