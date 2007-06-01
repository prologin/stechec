# -*- Makefile -*-

SUBDIRS = . xml_test test

pkglib_LTLIBRARIES = libtools.la

libtools_la_SOURCES =				\
	tools.hh

libtools_la_SOURCES +=				\
	misc/Library.cc				\
	misc/Library.hh				\
	misc/Stl.hh				\
	misc/Timer.hh				\
	misc/Timer.cc				\
	misc/Position.hh			\
	misc/Point.hh				\
	misc/Rect.hh				\
	misc/Exception.hh			\
	misc/ExceptionDefine.hh			\
	misc/os_wrapper.hh			\
	misc/os_wrapper.cc			\
	misc/compose.hpp

libtools_la_SOURCES +=				\
	logger/log.cc				\
	logger/log.hh

libtools_la_SOURCES +=				\
	datatfs/Cx.hh				\
	datatfs/Cx.cc				\
	datatfs/CxPool.hh			\
	datatfs/CxPool.hxx			\
	datatfs/TcpCx.hh			\
	datatfs/TcpCx.cc			\
	datatfs/Direct.hh			\
	datatfs/Direct.cc			\
	datatfs/FileCx.hh			\
	datatfs/Packet.hh			\
	datatfs/Packet.cc

libtools_la_SOURCES +=				\
	xml/xml.cc				\
	xml/xml.hh				\
	xml/xml_config.cc			\
	xml/xml_config.hh			\
	xml/xml_team.cc				\
	xml/xml_team.hh				\
	xml/xml_formation.cc			\
	xml/xml_formation.hh			\
	xml/xml_internal.cc			\
	xml/xml_internal.hh			\
	xml/strings.hh

libtools_la_CPPFLAGS	=			\
	$(XERCES_CPPFLAGS)

libtools_la_CXXFLAGS	=			\
	-DMODULE_NAME=\"tools\"			\
	-DMODULE_COLOR=C_PURPLE

libtools_la_LDFLAGS 	= 			\
	$(XERCES_LDFLAGS)

libtools_la_LIBADD      =			\
	$(LIBDL)
