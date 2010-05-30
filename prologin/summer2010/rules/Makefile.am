# -*- Makefile -*-

lib_LTLIBRARIES = argent.la

argent_la_SOURCES =			\
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

argent_la_LDFLAGS = -module
argent_la_LIBADD =						\
	$(top_builddir)/prologin/common/libstechec_rules.la


argent_la_CXXFLAGS =					\
	-DMODULE_NAME=\"argent\"			\
	-DMODULE_COLOR=C_RED				\
	-DLIB_NAME=\"argent\"				\
	-DPKG_DATA_DIR=\"$(pkgdatadir)/summer2010\"	\
	-DYAML

INCLUDES 	=				\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/prologin/common		\
	-I$(top_srcdir)/stechec/tools

#
# make check, pour verifier que tous les symboles sont definis
#
check_PROGRAMS = summer2010

summer2010_SOURCES  = check.cc $(argent_la_SOURCES)
summer2010_CXXFLAGS = $(argent_la_AM_CXXFLAGS)
summer2010_LDADD    =					\
	$(argent_la_LIBADD)				\
	$(top_builddir)/stechec/tools/libtools.la	\
	$(LIBDL)

