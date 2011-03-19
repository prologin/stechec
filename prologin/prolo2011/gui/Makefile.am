lib_LTLIBRARIES = tron_gui.la

tron_gui_la_SOURCES = \
	interface.cc \
	main.cc

prolo2011guidir = $(pkgdatadir)/prolo2011/gui

dist_prolo2011gui_DATA = \
	api.py \
	prologin.py

tron_gui_la_CXXFLAGS = $(PYTHON_CFLAGS) -DCHAMPION_PATH=\"$(prolo2011guidir)\"
tron_gui_la_LDFLAGS = -module $(PYTHON_LIBS)
