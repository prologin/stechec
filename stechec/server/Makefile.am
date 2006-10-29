# -*- Makefile -*-

bin_PROGRAMS 	= tbt_server

tbt_server_SOURCES	=			\
	client.hh				\
	game_hosting.cc				\
	game_hosting.hh				\
	server.hh				\
	server.cc				\
	GameClient.cc				\
	GameClient.hh				\
	WaitingClient.cc			\
	WaitingClient.hh			\
	main.cc

tbt_server_CXXFLAGS = 				\
	-DMODULE_COLOR=C_BLUE			\
	-DMODULE_NAME=\"server\"

tbt_server_LDFLAGS = 				\
	-export-dynamic

if USE_RPATH
tbt_server_LDFLAGS += -R $(libdir)
endif

tbt_server_LDADD =						\
	$(top_builddir)/stechec/base_rules/libbase_rules.la	\
	$(top_builddir)/stechec/tools/libtools.la

INCLUDES	=				\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/tools
