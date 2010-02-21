lib_LTLIBRARIES = toons.la

toons_la_SOURCES =		\
	Api.cc				\
	Api.hh				\
	Constant.hh			\
	GameData.cc			\
	GameData.hh			\
	interface.cc

toons_la_LDFLAGS = -module
toons_la_LIBADD = $(top_builddir)/prologin/common/libstechec_rules.la

toons_la_CXXFLAGS = 			\
	-DMODULE_NAME=\"toons\"		\
	-DMODULE_COLOR=C_RED		\
	-DLIB_NAME=\"toons\"		\
	-DPKG_DATA_DIR=\"$(pkgdatadir)/prolo2010\"	\
	-DYAML

INCLUDES =									\
	-I$(top_srcdir)/stechec/base_rules		\
	-I$(top_srcdir)/prologin/common			\
	-I$(top_srcdir)/stechec/tools

check_PROGRAMS = prolo2010

prolo2010_SOURCES = check.cc $(toons_la_SOURCES)
prolo2010_CXXFLAGS = $(toons_la_AM_CXXFLAGS)
prolo2010_LDADD =								\
	$(toons_la_LIBADD)							\
	$(top_builddir)/stechec/tools/libtools.la	\
	$(LIBDL)
