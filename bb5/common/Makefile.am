# -*- Makefile -*-

noinst_LTLIBRARIES = libbb5_common.la

libbb5_common_la_SOURCES =			\
	Module.cc				\
	Constants.cc				\
	Constants.hh				\
	Ball.hh					\
	Dice.cc					\
	Dice.hh					\
	Field.hh				\
	Player.hh				\
	Player.hxx				\
	Team.hh					\
	Team.hxx				\
	Weather.cc				\
	Weather.hh

libbb5_common_la_CXXFLAGS =			\
	-DMODULE_NAME=\"common\"		\
	-DMODULE_COLOR=C_BLUE

libbb5_common_la_LIBADD =					\
	$(top_builddir)/stechec/base_rules/libbase_rules.la

INCLUDES	=				\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/tools
