# -*- Makefile -*-

SUBDIRS	= common tkrun

if HAVE_QT
SUBDIRS += gui_qt
endif

SUBDIRS += $(BUILD_ant) $(BUILD_prolo2005) $(BUILD_prolo2006)

if HAVE_MYSQL
SUBDIRS +=					\
	meta_server				\
	main_server
endif
