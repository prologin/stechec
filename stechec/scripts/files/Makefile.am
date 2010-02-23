# -*- Makefile -*-

lib_LTLIBRARIES = !!rule_lib!!''.la

!!rule_lib!!_la_SOURCES =			\
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

!!rule_lib!!_la_LDFLAGS = -module
!!rule_lib!!_la_LIBADD =						\
	$(top_builddir)/prologin/common/libstechec_rules.la


!!rule_lib!!_la_CXXFLAGS =					\
	-DMODULE_NAME=\"!!rule_lib!!\"			\
	-DMODULE_COLOR=C_RED				\
	-DLIB_NAME=\"!!rule_lib!!\"				\
	-DPKG_DATA_DIR=\"$(pkgdatadir)/!!package_name!!\"	\
	-DYAML

INCLUDES 	=				\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/prologin/common		\
	-I$(top_srcdir)/stechec/tools

#
# make check, pour verifier que tous les symboles sont definis
#
check_PROGRAMS = !!package_name!!

!!package_name!!_SOURCES  = check.cc $(!!rule_lib!!_la_SOURCES)
!!package_name!!_CXXFLAGS = $(!!rule_lib!!_la_AM_CXXFLAGS)
!!package_name!!_LDADD    =					\
	$(!!rule_lib!!_la_LIBADD)				\
	$(top_builddir)/stechec/tools/libtools.la	\
	$(LIBDL)

