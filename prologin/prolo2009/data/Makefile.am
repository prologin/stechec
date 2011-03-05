# -*- Makefile -*-

prolo2009datadir = $(pkgdatadir)/prolo2009
prolo2009graphicsdir = $(pkgdatadir)/prolo2009/graphics

dist_prolo2009data_DATA =			\
	prolo2009.yml

dist_prolo2009graphics_DATA = graphics/*
	
dist-hook:
	rm -rf `find $(distdir) -name .svn`
