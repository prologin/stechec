# -*- Makefile -*-

lib_LTLIBRARIES	= libbb5_client_rules.la

libbb5_client_rules_la_SOURCES	=		\
	Api.hh					\
	Api.hxx					\
	CBall.hh				\
	CBall.cc				\
	CField.cc				\
	CField.hh				\
	CPlayer.cc				\
	CPlayer.hh				\
	CRules.cc				\
	CRules.hh				\
	CRules.hxx				\
	CTeam.hh				\
	CTeam.cc				\
	Event.hh				\
	Event.cc

libbb5_client_rules_la_CXXFLAGS	=		\
	-DMODULE_NAME=\"clrules\"		\
	-DMODULE_COLOR=C_GREEN			\
	-DMSG_COLOR

libbb5_client_rules_la_LIBADD =			\
	../common/libbb5_common.la

INCLUDES 	=				\
	-I$(top_srcdir)/stechec/tools		\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(srcdir)/../common
