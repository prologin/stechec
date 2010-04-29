# -*- Makefile -*-

prolo2010datadir = $(pkgdatadir)/prolo2010
prolo2010graphicsdir = $(pkgdatadir)/prolo2010/graphics

dist_prolo2010data_DATA =			\
	prolo2010.yml

dist_prolo2010graphics_DATA = graphics/*
	
dist-hook:
	rm -rf `find $(distdir) -name .svn`
