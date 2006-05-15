# -*- Makefile -*-

lib_LTLIBRARIES = ant.la

ant_la_SOURCES =			\
	    Event.hh			\
	    Api.cc			\
	    Api.hh			\
	    Client.hh			\
	    ClientDiffer.cc		\
	    ClientDiffer.hh		\
	    ClientEntry.cc		\
	    ClientEntry.hh		\
	    Constant.hh			\
	    GameData.cc			\
	    GameData.hh			\
	    Server.hh			\
	    ServerResolver.cc		\
	    ServerResolver.hh		\
	    ServerEntry.cc		\
	    ServerEntry.hh		\
	    interface.cc		\
	    load_rules.cc

ant_la_LDFLAGS = -module
ant_la_LIBADD =					\
	$(top_builddir)/prologin/common/libstechec_rules.la

ant_la_CXXFLAGS =	 			\
	-DMODULE_NAME=\"ant\"			\
	-DMODULE_COLOR=C_YELLOW			\
	-DLIB_NAME=\"ANT\"			\
	-DPKG_DATA_DIR=\"$(pkgdatadir)/ant\"

INCLUDES 	=				\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/tools		\
	-I$(top_srcdir)/prologin/common

# link a main() with our library, to check for missing symbols.
check_PROGRAMS 	= ant
ant_SOURCES	= check.cc $(ant_la_SOURCES)
ant_CXXFLAGS	= $(ant_la_CXXFLAGS)
ant_LDADD 	=					\
	$(ant_la_LIBADD)				\
	$(top_builddir)/stechec/tools/libtools.la	\
	$(LIBDL)
