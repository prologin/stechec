# -*- Makefile -*-

bin_PROGRAMS 	= tbt

tbt_SOURCES =					\
	client_cx.cc				\
	client_cx.hh				\
	start_arbiter.cc			\
	start_arbiter.hh			\
	rules_loader.cc				\
	rules_loader.hh				\
	champion_loader.cc			\
	champion_loader.hh			\
	main.cc

tbt_CXXFLAGS =					\
	$(PTHREAD_CFLAGS)			\
	$(SDL_CFLAGS)				\
	-DMODULE_NAME=\"main\"			\
	-DMODULE_COLOR=C_YELLOW

tbt_LDFLAGS =					\
	-R $(libdir)				\
	-export-dynamic				\
	$(PTHREAD_CFLAGS)


tbt_LDADD =							\
	$(top_builddir)/stechec/base_rules/libbase_rules.la	\
	$(top_builddir)/stechec/tools/libtools.la		\
	$(PTHREAD_LIBS)						\
        $(SDL_LIBS)

INCLUDES 	=				\
	-I$(top_srcdir)/stechec/tools		\
	-I$(top_srcdir)/stechec/base_rules
