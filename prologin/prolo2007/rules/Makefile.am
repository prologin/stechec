# -*- Makefile -*-

lib_LTLIBRARIES = globulus.la

globulus_la_SOURCES =			\
	Api.cc				\
	Api.hh				\
	Client.cc			\
	Client.hh			\
	ClientEntry.cc			\
	ClientEntry.hh			\
	ClientDiffer.cc			\
	ClientDiffer.hh			\
	Constant.hh			\
	Event.hh			\
	GameData.cc			\
	GameData.hh			\
	Server.hh			\
	Server.cc			\
	ServerResolver.cc		\
	ServerResolver.hh		\
	ServerEntry.cc			\
	ServerEntry.hh			\
	Object.hh			\
	bacterias.cc			\
	bacterias.hh			\
	cellule.cc			\
	cellule.hh			\
	nutriments.cc			\
	nutriments.hh			\
	leucocyte.cc			\
	leucocyte.hh			\
	virus.cc			\
	virus.hh			\
	load_rules.cc			\
	interface.cc

globulus_la_LDFLAGS = -module
globulus_la_LIBADD =						\
	$(top_builddir)/prologin/common/libstechec_rules.la


globulus_la_CXXFLAGS =					\
	-DMODULE_NAME=\"globulus\"			\
	-DMODULE_COLOR=C_RED				\
	-DLIB_NAME=\"GLOBULUS\"				\
	-DPKG_DATA_DIR=\"$(pkgdatadir)/prolo2007\"	\
	-DYAML

INCLUDES 	=				\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/prologin/common		\
	-I$(top_srcdir)/stechec/tools

#
# make check, pour verifier que tous les symboles sont definis
#
check_PROGRAMS = prolo2007

prolo2007_SOURCES  = check.cc $(globulus_la_SOURCES)
prolo2007_CXXFLAGS = $(globulus_la_AM_CXXFLAGS)
prolo2007_LDADD    =					\
	$(globulus_la_LIBADD)				\
	$(top_builddir)/stechec/tools/libtools.la	\
	$(LIBDL)
