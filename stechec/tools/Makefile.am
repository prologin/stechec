# -*- Makefile -*-

SUBDIRS = . test

pkglib_LTLIBRARIES = libtools.la

libtools_la_SOURCES =				\
	tools.hh

libtools_la_SOURCES +=				\
	misc/Conf.cc				\
	misc/Conf.hh				\
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
	net/Cx.hh				\
	net/Cx.cc				\
	net/CxPool.hh				\
	net/CxPool.hxx				\
	net/TcpCx.hh				\
	net/TcpCx.cc				\
	net/FileCx.hh				\
	net/FileCx.cc				\
	net/Packet.hh				\
	net/Packet.cc

libtools_la_CXXFLAGS	=			\
	-DMODULE_NAME=\"tools\"			\
	-DMODULE_COLOR=C_PURPLE

libtools_la_LIBADD      =			\
	$(LIBDL)
