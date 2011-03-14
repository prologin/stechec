# -*- Makefile -*-

bin_PROGRAMS = stechec

noinst_LTLIBRARIES = libgeneric_client.la

libgeneric_client_la_SOURCES =			\
	ClientCx.cc				\
	ClientCx.hh				\
	RulesLoader.cc				\
	RulesLoader.hh				\
	ChampionLoader.cc			\
	ChampionLoader.hh			\
	ClientApp.cc				\
	ClientApp.hh

libgeneric_client_la_CXXFLAGS =			\
	$(PTHREAD_CFLAGS)			\
	$(SDL_CFLAGS)				\
	-DMODULE_NAME=\"main\"			\
	-DMODULE_COLOR=C_YELLOW

stechec_SOURCES =				\
	Stechec.hh				\
	Stechec.cc

stechec_LDFLAGS =				\
	-export-dynamic				\
	$(PTHREAD_CFLAGS)

if USE_RPATH
stechec_LDFLAGS += -R $(libdir)
endif

stechec_LDADD =							\
	libgeneric_client.la					\
	$(top_builddir)/stechec/base_rules/libbase_rules.la	\
	$(top_builddir)/stechec/tools/libtools.la		\
	$(PTHREAD_LIBS)						\
        $(SDL_LIBS)

INCLUDES 	=				\
	-I$(top_srcdir)/stechec/tools		\
	-I$(top_srcdir)/stechec/base_rules
