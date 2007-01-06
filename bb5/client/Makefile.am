# -*- Makefile -*-

# we have to create an extra noinst_ library and have our installed library
# and module depend on it, or automake is unable to compute correct dependency
# on install.
noinst_LTLIBRARIES = libbb5_client_rules_common.la
pkglib_LTLIBRARIES = libbb5_client_rules_lib.la
lib_LTLIBRARIES = bb5_client_rules.la

# noinst common lib
libbb5_client_rules_common_la_SOURCES =		\
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

libbb5_client_rules_common_la_LIBADD =		\
	../common/libbb5_common.la

# installed shared library
libbb5_client_rules_lib_la_SOURCES =
libbb5_client_rules_lib_la_LIBADD =		\
	./libbb5_client_rules_common.la

# installed module library
bb5_client_rules_la_SOURCES =
bb5_client_rules_la_LIBADD =			\
	./libbb5_client_rules_common.la
bb5_client_rules_la_LDFLAGS =			\
	-module


# common flags
CXXFLAGS =					\
	-DMODULE_NAME=\"clrules\"		\
	-DMODULE_COLOR=C_GREEN			\
	-DMSG_COLOR

INCLUDES 	=				\
	-I$(top_srcdir)/stechec/tools		\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(srcdir)/../common
