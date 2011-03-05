# -*- Makefile -*-

TESTS 		= 				\
	parse_formation				\
	parse_team				\
	parse_bad


check_PROGRAMS	= $(TESTS)

parse_formation_SOURCES = parse_formation.cc
parse_team_SOURCES 	= parse_team.cc
parse_bad_SOURCES	= parse_bad.cc

LDADD 		=				\
	$(top_builddir)/stechec/tools/libtools.la

INCLUDES	= 				\
	-I$(top_srcdir)/stechec/tools

EXTRA_DIST	= 				\
	formations.xml				\
	team.xml				\
	formations-bad.xml			\
	formation.dtd				\
	team.dtd
