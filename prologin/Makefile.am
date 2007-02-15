# -*- Makefile -*-

SUBDIRS	= common tkrun

SUBDIRS += $(BUILD_ant) $(BUILD_prolo2005) $(BUILD_prolo2006) $(BUILD_prolo2007)

if HAVE_MYSQL
SUBDIRS +=					\
	meta_server				\
	main_server
endif
