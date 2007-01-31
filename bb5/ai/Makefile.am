# -*- Makefile -*-

bin_PROGRAMS 	= tbt_ai

tbt_ai_SOURCES =				\
	ai.cc					\
	ai.hh

tbt_ai_CXXFLAGS = 				\
	-DMODULE_NAME=\"ai\"			\
	-DMODULE_COLOR=C_RED			\
	-DPKGDATADIR=\"$(pkgdatadir)/bb5\"

tbt_ai_LDADD =							\
	../client/libbb5_client_rules_common.la			\
	$(top_builddir)/stechec/client/libgeneric_client.la	\
	$(top_builddir)/stechec/tools/libtools.la		\
	$(PTHREAD_LIBS)

tbt_ai_LDFLAGS =				\
	-export-dynamic

if USE_RPATH
tbt_ai_LDFLAGS += -R $(libdir)
endif

INCLUDES =					\
	-I$(top_srcdir)/stechec/tools		\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/client		\
	-I$(srcdir)/../common			\
	-I$(srcdir)/../client
