# -*- Makefile -*-

lib_LTLIBRARIES = bb5_arbiter.la

bb5_arbiter_la_SOURCES =			\
	STeam.cc				\
	STeam.hh				\
	STeamMsg.cc				\
	STeamMsg.hh				\
	SPlayer.hh				\
	SPlayer.cc				\
	SPlayerMsg.cc				\
	SPlayerMsg.hh				\
	SActionHandler.cc			\
	SActionHandler.hh			\
	SRules.cc				\
	SRules.hh				\
	SRules.hxx				\
	SWeather.cc				\
	SWeather.hh				\
	SBall.cc				\
	SBall.hh

bb5_arbiter_la_CXXFLAGS =			\
	-DMODULE_COLOR=C_RED			\
	-DMODULE_NAME=\"arbiter\"

bb5_arbiter_la_LDFLAGS = 			\
	-module

bb5_arbiter_la_LIBADD =				\
	../common/libbb5_common.la

INCLUDES	=				\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/tools		\
	-I$(srcdir)/../common
