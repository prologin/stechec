# -*- Makefile -*-

prolo2011datadir = $(pkgdatadir)/prolo2011
prolo2011graphicsdir = $(pkgdatadir)/prolo2011/graphics

dist_prolo2011data_DATA =			\
	prolo2011.yml

dist_prolo2011graphics_DATA = graphics/*

dist-hook:
	rm -rf `find $(distdir) -name .svn`
