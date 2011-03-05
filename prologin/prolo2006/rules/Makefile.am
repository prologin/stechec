# -*- Makefile -*-

lib_LTLIBRARIES = outatime.la

outatime_la_SOURCES =			\
	Api.cc				\
	Api.hh				\
	Client.cc			\
	Client.hh			\
	ClientEntry.cc			\
	ClientEntry.hh			\
	ClientDiffer.cc			\
	ClientDiffer.hh			\
	Constant.hh			\
	GameData.cc			\
	GameData.hh			\
	Server.hh			\
	Server.cc			\
	ServerResolver.cc		\
	ServerResolver.hh		\
	ServerEntry.cc			\
	ServerEntry.hh			\
	Astar.cc			\
	Astar.hh			\
	Event.hh			\
	load_rules.cc			\
	interface.cc

outatime_la_LDFLAGS = -module
outatime_la_LIBADD =						\
	$(top_builddir)/prologin/common/libstechec_rules.la


outatime_la_CXXFLAGS =					\
	-DMODULE_NAME=\"outatime\"			\
	-DMODULE_COLOR=C_RED				\
	-DLIB_NAME=\"OUTATIME\"				\
	-DPKG_DATA_DIR=\"$(pkgdatadir)/prolo2006\"

INCLUDES 	=				\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/prologin/common		\
	-I$(top_srcdir)/stechec/tools

#
# make check, pour verifier que tous les symboles sont definis
#
check_PROGRAMS = prolo2006

prolo2006_SOURCES  = check.cc $(outatime_la_SOURCES)
prolo2006_CXXFLAGS = $(outatime_la_AM_CXXFLAGS)
prolo2006_LDADD    =					\
	$(outatime_la_LIBADD)				\
	$(top_builddir)/stechec/tools/libtools.la	\
	$(LIBDL)
