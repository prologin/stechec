# -*- Makefile -*-

bin_PROGRAMS 	= stechec_server

stechec_server_SOURCES	=			\
	Client.hh				\
	GameHosting.cc				\
	GameHosting.hh				\
	Server.hh				\
	Server.cc				\
	Main.cc

stechec_server_CXXFLAGS =			\
	-DMODULE_COLOR=C_BLUE			\
	-DMODULE_NAME=\"server\"

stechec_server_LDFLAGS =			\
	-export-dynamic

if USE_RPATH
stechec_server_LDFLAGS += -R $(libdir)
endif

stechec_server_LDADD =						\
	$(top_builddir)/stechec/base_rules/libbase_rules.la	\
	$(top_builddir)/stechec/tools/libtools.la

INCLUDES	=				\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/tools
