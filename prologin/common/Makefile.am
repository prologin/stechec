#
#  Stechec project is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  Stechec project is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with the Stechec project; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#  Copyright 2005, 2006 Prologin EPITA
#


pkglib_LTLIBRARIES = libstechec_rules.la

libstechec_rules_la_SOURCES =			\
	Rules.hh				\
	Rules.cc				\
	CRules.cc				\
	CRules.hh				\
	SRules.cc				\
	SRules.hh				\
	Contest.hh				\
	StechecGameData.hh			\
	StechecEntry.hh				\
	StechecApi.hh				\
	StechecApi.cc				\
	StechecServer.cc			\
	StechecClientDiffer.hh			\
	StechecClient.hh			\
	StechecClientEntry.hh			\
	StechecServer.hh			\
	StechecServerResolver.hh		\
	StechecServerResolver.cc		\
	StechecServerEntry.hh			\
	StechecEvent.hh				\
	StechecEvent.cc

libstechec_rules_la_CXXFLAGS =			\
	-DMODULE_NAME=\"stechec\"		\
	-DMODULE_COLOR=C_BPURPLE

libstechec_rules_la_LDFLAGS =			\
	$(LT_NO_UNDEFINED)			\
	-version-info 0:0:0

libstechec_rules_la_LIBADD =			\
	$(top_builddir)/stechec/base_rules/libbase_rules.la

INCLUDES =					\
	-I$(top_srcdir)/stechec/tools		\
	-I$(top_srcdir)/stechec/base_rules	\
	-I$(top_srcdir)/stechec/client		\
	-I$(top_srcdir)/stechec/server

## eof ##
