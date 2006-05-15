# -*- Makefile -*-

bin_PROGRAMS 	= tbt_server

tbt_server_SOURCES	=			\
	client.hh				\
	Client.hxx				\
	game_hosting.cc				\
	game_hosting.hh				\
	main.cc					\
	server.hh				\
	server.cc

tbt_server_CXXFLAGS = 				\
	-DMODULE_COLOR=C_BLUE			\
	-DMODULE_NAME=\"server\"

tbt_server_LDFLAGS = 				\
	-R $(libdir)				\
	-export-dynamic

tbt_server_LDADD =						\
	$(top_builddir)/stechec/base_rules/libbase_rules.la	\
	$(top_builddir)/stechec/tools/libtools.la

INCLUDES	=				\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/tools
