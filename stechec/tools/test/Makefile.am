# -*- Makefile -*-

TESTS = test_cx.sh

check_PROGRAMS = cx pool

cx_SOURCES = test_tcp.cc
pool_SOURCES = test_pool.cc

LDADD 		=				\
	$(top_builddir)/stechec/tools/libtools.la

INCLUDES	= 				\
	-I$(top_srcdir)/stechec/tools
