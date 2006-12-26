# -*- Makefile -*-

bin_PROGRAMS 	= tbt_console

tbt_console_SOURCES =				\
	input.cc				\
	input.hh				\
	interface.cc				\
	interface.hh				\
	console_loader.cc

tbt_console_CXXFLAGS =				\
	-DMODULE_NAME=\"console\"		\
	-DMODULE_COLOR=C_YELLOW			\
	-DMSG_COLOR				\
	$(READLINE_CFLAGS)

tbt_console_LDFLAGS =				\
	-export-dynamic

tbt_console_LDADD =						\
	../client/libbb5_client_rules.la			\
	$(top_builddir)/stechec/client/libgeneric_client.la	\
	$(top_builddir)/stechec/tools/libtools.la		\
	$(PTHREAD_LIBS)						\
	$(READLINE_LIBS)

if USE_RPATH
tbt_console_LDFLAGS += -R $(libdir)
endif

INCLUDES 	=				\
	-I$(top_srcdir)/stechec/tools		\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/client		\
	-I$(srcdir)/../common			\
	-I$(srcdir)/../client
