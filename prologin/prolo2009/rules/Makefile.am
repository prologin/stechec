# -*- Makefile -*-

lib_LTLIBRARIES = simcity.la

simcity_la_SOURCES =			\
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
	interface.cc

simcity_la_LDFLAGS = -module
simcity_la_LIBADD =						\
	$(top_builddir)/prologin/common/libstechec_rules.la


simcity_la_CXXFLAGS =					\
	-DMODULE_NAME=\"simcity\"			\
	-DMODULE_COLOR=C_RED				\
	-DLIB_NAME=\"simcity\"				\
	-DPKG_DATA_DIR=\"$(pkgdatadir)/prolo2009\"	\
	-DYAML

INCLUDES 	=				\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/prologin/common		\
	-I$(top_srcdir)/stechec/tools

#
# make check, pour verifier que tous les symboles sont definis
#
check_PROGRAMS = prolo2009

prolo2009_SOURCES  = check.cc $(simcity_la_SOURCES)
prolo2009_CXXFLAGS = $(simcity_la_AM_CXXFLAGS)
prolo2009_LDADD    =					\
	$(simcity_la_LIBADD)				\
	$(top_builddir)/stechec/tools/libtools.la	\
	$(LIBDL)
