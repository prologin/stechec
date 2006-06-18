# -*- Makefile -*-

lib_LTLIBRARIES 	= bb5_client_console.la

bb5_client_console_la_SOURCES =			\
	input.cc				\
	input.hh				\
	interface.cc				\
	interface.hh				\
	console_loader.cc

bb5_client_console_la_CXXFLAGS =		\
	-DMODULE_NAME=\"console\"		\
	-DMODULE_COLOR=C_YELLOW			\
	-DMSG_COLOR				\
	$(READLINE_CFLAGS)

bb5_client_console_la_LDFLAGS =			\
	-module					\
	$(READLINE_LIBS)

INCLUDES 	=				\
	-I$(top_srcdir)/stechec/tools		\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/client		\
	-I$(srcdir)/../common			\
	-I$(srcdir)/../client
