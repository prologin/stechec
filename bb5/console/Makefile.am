# -*- Makefile -*-

noinst_LTLIBRARIES = libbb5_console.la
lib_LTLIBRARIES = bb5_client_console.la
bin_PROGRAMS 	= tbt_console

# common sources
libbb5_console_la_SOURCES =			\
	input.cc				\
	input.hh				\
	interface.cc				\
	interface.hh

libbb5_console_la_LIBADD =					\
	../client/libbb5_client_rules_common.la			\
	$(top_builddir)/stechec/client/libgeneric_client.la	\
	$(top_builddir)/stechec/tools/libtools.la		\
	$(PTHREAD_LIBS)						\
	$(READLINE_LIBS)

# loadable module
bb5_client_console_la_SOURCES =			\
	console_loader.cc
bb5_client_console_la_CXXFLAGS =		\
	-DCONSOLE_MODULE			\
	$(CXXFLAGS)
bb5_client_console_la_LIBADD =			\
	./libbb5_console.la
bb5_client_console_la_LDFLAGS =			\
	-module

# binary
tbt_console_SOURCES =				\
	console_loader.cc
tbt_console_CXXFLAGS = 				\
	-DCONSOLE_BINARY			\
	$(CXXFLAGS)
tbt_console_LDADD =				\
	./libbb5_console.la
tbt_console_LDFLAGS =				\
	-export-dynamic

if USE_RPATH
tbt_console_LDFLAGS += -R $(libdir)
endif

# common flags

CXXFLAGS =					\
	-DMODULE_NAME=\"console\"		\
	-DMODULE_COLOR=C_YELLOW			\
	-DPKGDATADIR=\"$(pkgdatadir)/bb5\"	\
	$(READLINE_CFLAGS)

INCLUDES =					\
	-I$(top_srcdir)/stechec/tools		\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/client		\
	-I$(srcdir)/../common			\
	-I$(srcdir)/../client
