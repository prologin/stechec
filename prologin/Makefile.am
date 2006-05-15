# -*- Makefile -*-

SUBDIRS	= common tkrun

if HAVE_QT
SUBDIRS += gui_qt
endif

if BUILD_ant
SUBDIRS += ant
endif

if BUILD_prolo2005
SUBDIRS += prolo2005
endif

if BUILD_prolo2006
SUBDIRS += prolo2006
endif

if HAVE_MYSQL
SUBDIRS +=					\
	meta_server				\
	main_server
endif
