# -*- Makefile -*-

bin_PROGRAMS	= tbt_lightmenu #tbt_visu

noinst_LTLIBRARIES = libbb5_visu_common.la

# # common sources (the gui)
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

# # build main menu along with the gui as a binary
# tbt_visu_SOURCES =				\
# 	main.cc					\
# 	Colors.hh				\
# 	Widget.cc Widget.hh			\
# 	Focus.cc Focus.hh			\
# 	GuiError.cc GuiError.hh			\
# 	Button.cc Button.hh			\
# 	DialogBox.cc DialogBox.hh		\
# 	Global.cc Global.hh			\
# 	Menu.cc Menu.hh				\
# 	Label.cc Label.hh			\
# 	Entry.cc Entry.hh			\
# 	LabelEntry.cc LabelEntry.hh

# tbt_visu_LDADD =						\
# 	$(top_builddir)/stechec/tools/libtools.la		\
# 	$(top_builddir)/stechec/sdl/libstechec_sdl.la		\
# 	../client/libbb5_client_rules.la			\
# 	libbb5_visu_common.la

# tbt_visu_LDFLAGS	=			\
# 	$(SDL_LIBS)


# build a light menu along with the gui as a binary
tbt_lightmenu_SOURCES =				\
	MenuLight.cc

# 	DlgBox.hh				\
# 	DlgBox.cc				\
# 	ActionPopup.hh				\
# 	ActionPopup.cc				\
# 	Panel.hh				\
# 	Panel.cc				\
# 	Field.hh				\
# 	Field.cc				\
# 	VisuPlayer.hh				\
# 	VisuPlayer.cc				\
# 	Game.hh					\
# 	Game.cc

tbt_lightmenu_LDADD =						\
	libbb5_visu_common.la					\
	../client/libbb5_client_rules.la			\
	$(top_builddir)/stechec/client/libgeneric_client.la	\
	$(top_builddir)/stechec/sdl/libstechec_sdl.la		\
	$(top_builddir)/stechec/tools/libtools.la		\
	$(SDL_LIBS)

tbt_lightmenu_LDFLAGS	=			\
	-export-dynamic


if USE_RPATH
tbt_lightmenu_LDFLAGS += -R $(libdir)
endif

# common flags
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
