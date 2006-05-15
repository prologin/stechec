# -*- Makefile -*-

lib_LTLIBRARIES = libzavie.la

libzavie_la_SOURCES =			\
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

libzavie_la_LDFLAGS = -module
libzavie_la_LIBADD =						\
	$(top_builddir)/prologin/common/libstechec_rules.la


libzavie_la_CXXFLAGS =					\
	-DMODULE_NAME=\"libzavie\"			\
	-DMODULE_COLOR=C_YELLOW				\
	-DLIB_NAME=\"LIBZAVIE\"				\
	-DPKG_DATA_DIR=\"$(pkgdatadir)/prolo2005\"

INCLUDES 	=				\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/tools		\
	-I$(top_srcdir)/prologin/common

#
# make check, pour verifier que tous les symboles sont definis
#
check_PROGRAMS = libzavie
libzavie_SOURCES = check.cc $(libzavie_la_SOURCES)
libzavie_CXXFLAGS = $(libzavie_la_CXXFLAGS)
libzavie_LDADD 	=					\
	$(libzavie_la_LIBADD)				\
	$(top_builddir)/stechec/tools/libtools.la	\
	$(LIBDL)
