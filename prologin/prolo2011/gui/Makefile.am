lib_LTLIBRARIES = tron_gui.la

tron_gui_la_SOURCES = \
	interface.cc \
	main.cc

prolo2011guidir = $(pkgdatadir)/prolo2011/gui

dist_prolo2011gui_DATA = \
	api.py \
	paths.py \
	images.py \
	game.py \
	state_reader.py \
	surface.py \
	field_surface.py \
	team_surface.py \
	state_surface.py \
	detail_surface.py \
	help_surface.py \
	actions_surface.py \
	graphics.py \
	prologin.py \
	read_dump.py

dist_EXTRA = paths.py.in

tron_gui_la_CXXFLAGS = $(PYTHON_CFLAGS) -DCHAMPION_PATH=\"$(prolo2011guidir)\"
tron_gui_la_LDFLAGS = -module $(PYTHON_LIBS)

INCLUDES = \
        -I$(top_srcdir)/stechec/tools                   \
        -I$(top_srcdir)/stechec/base_rules              \
        -I$(top_srcdir)/stechec/client                  \
        -I$(top_srcdir)/prologin/common                 \
        -I$(top_srcdir)/prologin/prolo2011/rules
