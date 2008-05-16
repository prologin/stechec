# -*- Makefile -*-

lib_LTLIBRARIES = hamster.la

hamster_la_SOURCES =			\
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

hamster_la_LDFLAGS = -module
hamster_la_LIBADD =						\
	$(top_builddir)/prologin/common/libstechec_rules.la


hamster_la_CXXFLAGS =					\
	-DMODULE_NAME=\"hamster\"			\
	-DMODULE_COLOR=C_RED				\
	-DLIB_NAME=\"HAMSTER\"				\
	-DPKG_DATA_DIR=\"$(pkgdatadir)/prolo2008\"	\
	-DYAML

INCLUDES 	=				\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/prologin/common		\
	-I$(top_srcdir)/stechec/tools

#
# make check, pour verifier que tous les symboles sont definis
#
check_PROGRAMS = prolo2008

prolo2008_SOURCES  = check.cc $(hamster_la_SOURCES)
prolo2008_CXXFLAGS = $(hamster_la_AM_CXXFLAGS)
prolo2008_LDADD    =					\
	$(hamster_la_LIBADD)				\
	$(top_builddir)/stechec/tools/libtools.la	\
	$(LIBDL)
