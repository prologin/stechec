# -*- Makefile -*-

pkglib_LTLIBRARIES = libbase_rules.la

libbase_rules_la_SOURCES =			\
	BaseApi.hh				\
	BaseRules.cc				\
	BaseRules.hh				\
	BaseCRules.hh				\
	BaseCRules.hxx				\
	BaseCRules.cc				\
	BaseSRules.hh				\
	BaseSRules.cc				\
	NutsBarrier.cc				\
	NutsBarrier.hh				\
	EventProcess.cc				\
	EventProcess.hh				\
	PacketHandler.hh			\
	PacketSender.hh

libbase_rules_la_CXXFLAGS =			\
	-DMODULE_NAME=\"common\"		\
	-DMODULE_COLOR=C_BLUE

libbase_rules_la_LDFLAGS =			\
	$(LT_NO_UNDEFINED)			\
	-version-info 0:0:0

INCLUDES	=				\
	-I$(top_srcdir)/stechec/tools
