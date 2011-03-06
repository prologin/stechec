# -*- Makefile -*-

lib_LTLIBRARIES = tron.la

tron_la_SOURCES =			\
	Actions.cc		\
	Actions.hh		\
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
	load_rules.cc			\
	internal_interface.cc	\
	interface.cc

tron_la_LDFLAGS = -module
tron_la_LIBADD =						\
	$(top_builddir)/prologin/common/libstechec_rules.la


tron_la_CXXFLAGS =					\
	-DMODULE_NAME=\"tron\"			\
	-DMODULE_COLOR=C_RED				\
	-DLIB_NAME=\"tron\"				\
	-DPKG_DATA_DIR=\"$(pkgdatadir)/prolo2011\"	\
	-DYAML

INCLUDES 	=				\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/prologin/common		\
	-I$(top_srcdir)/stechec/tools

#
# make check, pour verifier que tous les symboles sont definis
#
check_PROGRAMS = prolo2011

prolo2011_SOURCES  = check.cc $(tron_la_SOURCES)
prolo2011_CXXFLAGS = $(tron_la_AM_CXXFLAGS)
prolo2011_LDADD    =					\
	$(tron_la_LIBADD)				\
	$(top_builddir)/stechec/tools/libtools.la	\
	$(LIBDL)

