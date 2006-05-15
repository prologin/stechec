# -*- Makefile -*-

lib_LTLIBRARIES		= outatime_virchor.la

CXXFLAGS =					\
	-DMODULE_NAME=\"virchor\"		\
	-DMODULE_COLOR=C_YELLOW			\
	-DPKGDATADIR=\"$(pkgdatadir)/prolo2006/\"


outatime_virchor_la_SOURCES	=		\
	VirchorGui.cc				\
	VirchorGui.hh				\
	VirObject.hh				\
	VirUdp.hh

outatime_virchor_la_LDFLAGS	=		\
	-module

INCLUDES        =				\
        -I$(top_srcdir)/stechec/tools		\
        -I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/client		\
	-I$(top_srcdir)/prologin/common		\
	-I$(srcdir)/../rules
